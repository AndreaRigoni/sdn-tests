
#include <Core/Options.h>
#include <Math/ScalarArray.h>

#include "dancc.h"
#include "dancc_data_source.h"
#include "dancc_stream.h"
#include "dancc_archive_client.h"


int main(int argc, char *argv[])
{

    ///////////////////////////////////////////
    //  OPTIONS  //////////////////////////////
    //
    std::string opt_server_name("localhost");
    std::string opt_server_port("9999");
    size_t bd_size = 128;
    size_t n_threads = 1;
    size_t bd_per_thread = 1024;
    std::string src_type = "Vector1f";
    Options opt;
    opt.AddOptions()
            ("server", &opt_server_name, "server name or ip")
            ("port",   &opt_server_port, "port")
            ("bd_size [KB]", &bd_size )
            ("threads", &n_threads, "number of threads")
            ("bd_per_thread",&bd_per_thread,"number of block sent per thread")
            ("src_type",&src_type,"source type: Vector3f,Vector1f ")
            ;
    opt.Parse(argc,argv);
    bd_size *= 1024;



    // SOURCE //
    dancc::DataSource ds("test2_src");
    if(src_type == "Vector1f") {
        std::vector<float> wave;
        size_t bd_sample_size = bd_size / sizeof(float);
        for(size_t i=0; i<bd_sample_size; ++i) {
            wave.push_back(sin((double)i*2*M_PI/bd_sample_size));
        }
        ds.initDataBlock(wave);
    }
    if(src_type == "Vector3f") {
        dancc::D0WAVE<float,3> wave;
        size_t bd_sample_size = bd_size / sizeof(float) / 3;
        for(size_t i=0; i<bd_sample_size; ++i) {
            Vector3f v;
            v(0) = i;
            v(1) = sin((double)i*2*M_PI/bd_sample_size);
            v(2) = sin((double)i*(double)i*2*M_PI/bd_sample_size/bd_sample_size*4);
            if(i%1000 == 0) std::cout << ".";
            wave << v;
        }
        ds.initDataBlock(wave);
    }

    // stream //
    dancc::Stream stream;
    stream.setDataSource(ds);

    dancc::ArchiveClient ar("ar");
    ar.setPulse("P1");
    ar.setServer(opt_server_name.c_str(),opt_server_port.c_str());

    // add the same stream per each thread //
    for(size_t i=0; i<n_threads; ++i)
        ar.addDataStream(stream);

    // set profiler //
    for(size_t i=0; i<ar.getStreamInfoSize(); ++i) {
        profiler_data_init(&ar.getStreamInfo(i)->profiler, 100000);
        profiler_data_add(&ar.getStreamInfo(i)->profiler, 0);
    }

    // ARCHIVE //
    std::cout << "\n\n" << ar.group() << "\n";
    ar.writeMultiThreads(bd_per_thread);
    // ar.writeAllBlocksPerStream(1000000);

    for(size_t i=0; i<ar.getStreamInfoSize(); ++i) {
        std::stringstream ns;
        ns << "test2_perf_" << i;
        profiler_data_save(&ar.getStreamInfo(i)->profiler,ns.str().c_str());
    }

    return 0;
}

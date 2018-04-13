
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
    size_t bd_size = 200*12;
    Options opt;
    opt.AddOptions()
            ("server", &opt_server_name, "server name or ip")
            ("port",   &opt_server_port, "port")
            ("bd_size [KB]", &bd_size )
            ;
    opt.Parse(argc,argv);
    bd_size *= 1024;


    // SOURCE //
    dancc::D0WAVE<float,3> wave;

    size_t bd_sample_size = bd_size / sizeof(float) / 3;
    std::cout << "bd_sample_size: " << bd_sample_size << "\n";
    for(size_t i=0; i<bd_sample_size; ++i) {
        Vector3f v;
        v(0) = i;
        v(1) = sin((double)i*2*M_PI/bd_sample_size);
        v(2) = sin((double)i*(double)i*2*M_PI/bd_sample_size/bd_sample_size*4);
        if(i%1000 == 0) std::cout << ".";
        wave << v;
    }


    dancc::DataSource ds("provaW0");
    ds.initDataBlock(wave);

    dancc::Stream stream;
    stream.setDataSource(ds);

    dancc::ArchiveClient ar("ar");
    ar.setPulse("PULSE1");
    ar.setServer(opt_server_name.c_str(),opt_server_port.c_str());

    ar.addDataStream(stream);

    std::cout << "\n\n" << ar.group() << "\n";
    // ar.writeMultiThreads(1);
    ar.writeAllBlocksPerStream(10);

    return 0;
}

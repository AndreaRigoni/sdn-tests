
#include <Core/Options.h>

#include "dancc.h"
#include "dancc_data_source.h"
#include "dancc_stream.h"
#include "dancc_archive_client.h"


int main(int argc, char *argv[])
{

    std::string opt_server_name("localhost");
    std::string opt_server_port("9999");
    size_t bd_size = 24;
    Options opt;
    opt.AddOptions()
            ("server", &opt_server_name, "server name or ip")
            ("port",   &opt_server_port, "port")
            ("bd_size", &bd_size )
            ;
    opt.Parse(argc,argv);

    std::vector<float> vf(bd_size);

    dancc::DataSource test1("test1");
    dancc::DataSource test2("test2");


    for(size_t i=0; i<vf.size(); ++i)
//        vf[i] = tan((double)i*M_PI_2/vf.size());
        vf[i] = i;
    test1.initDataBlock(vf);

    for(size_t i=0; i<vf.size(); ++i)
        vf[i] = sin((double)i*2*M_PI/vf.size());
    test2.initDataBlock(vf,6);

//    std::cout << " ---------------------------- test1: \n";
//    std::cout << "\n --- Source Info ---- \n"
//              << test1.sourceInfo();
//    std::cout << "\n --- Publisher Info ---- \n"
//              << test1.publisherInfo();
//    std::cout << " ---------------------------- test2: \n";
//    std::cout << "\n --- Source Info ---- \n"
//              << test2.sourceInfo();
//    std::cout << "\n --- Publisher Info ---- \n"
//              << test2.publisherInfo();


    dancc::Stream stream1;
    stream1.setDataSource(test1);

    dancc::Stream stream2;
    stream2.setDataSource(test2);

    dancc::ArchiveClient ar("ar");

    ar.setPulse("PULSE1");
    ar.setServer(opt_server_name.c_str(),opt_server_port.c_str());

    ar.addDataStream(stream1);
    ar.addDataStream(stream2);
    ar.addDataStream(stream1);
    ar.addDataStream(stream2);

    std::cout << "\n\n" << ar.group() << "\n";
    ar.writeAllBlocksPerStream(1);
    // ar.writeSingleBlockPerStream(10);
    //    ar.writeMultiThreads(10);

    return 0;
}

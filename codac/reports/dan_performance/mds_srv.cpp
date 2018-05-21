#include <iostream>
#include <string>
#include <mdsobjects.h>

#include <Core/unique_ptr.h>
#include <Core/Options.h>


#include "dan/common_defs.h"
#include "dan/timestamp.h"
#include "dan/prof_service.h"
#include "dan/dan_log_service.h"
#include "dan/archive/archive_api.h"

#include "dan_performance.h"


using namespace MDSplus;

template <>
class Deleter<MDSplus::Data> {
public:
    static void _delete(Data * ptr) {  deleteData(ptr); }
};




class mdsclt_Options : public Options {
  public:
    char file_name[PATH_MAX];
    char prof_file_name[PATH_MAX];
    std::string m_buffer_size;
    std::string m_block_size;
    std::string m_size;
    std::string m_server_name;
    std::string m_server_port;

    mdsclt_Options(int argc, char *argv[])
    {
        // init defaults //
        strcpy(file_name,"mdssrv_data");
        strcpy(prof_file_name,"mdssrv");
        m_block_size = "4K";
        m_size = "1G";
        m_buffer_size = "8K";
//        m_server_name = "localhost";
//        m_server_port = "8000";

        // setup //
        this->SetUsage("usage: mds-srv [options]");
        this->AddOptions()
                ("file",  &file_name,  "output file name")
                ("prof-file",  &prof_file_name,  "profiler output file name")
                ("size",    &m_size,        "output data size")
                ("db-size", &m_block_size,  "data block size")
                ("bf-size", &m_buffer_size, "server buffer size")
//                ("server_name", &m_server_name, "server name")
//                ("server_port", &m_server_port, "server port")
                ;
        this->Parse(argc,argv);
    }

    size_t size() const { return options_get_size(m_size); }
    size_t block_size() const { return options_get_size(m_block_size); }
};




int main(int argc, char *argv[])
{
    mdsclt_Options opt(argc,argv);

    // Profiler instance //
//    InitInstanceCounting(argv[0]);
//    GetInstanceNumber();

    size_t bsize = opt.block_size();
    size_t bsize_int = bsize/sizeof(int32_t);
    size_t bsize_int16 = bsize/sizeof(int16_t);
    size_t ndb   = opt.size()/opt.block_size();
    float  dt = 1.E-6;

    ProfilerData prof;
    profiler_data_init(&prof, ndb+1);


    std::string path = opt.file_name + std::string("_path");
    setenv(path.c_str(),".",1);
    unique_ptr<Tree> tree = new Tree(opt.file_name,1,"NEW");
    unique_ptr<TreeNode> node = tree->addNode("test_node","ANY");
    tree->write();
    tree->edit(true);

    int *array = (int*)malloc(bsize);
    for(size_t i=0; i<bsize_int; ++i) array[i]=i;

    unique_ptr<Int32Array> array_data = new Int32Array(array,bsize_int);

    // init profiles with first chunk //
    profiler_data_add(&prof, 0);

    for(size_t i=0; i<ndb; ++i) {
        float len = bsize_int*dt;
        unique_ptr<Range> array_time = new Range(new Float32(i*len), new Float32((i+1)*len), new Float32(dt));
        node->makeSegment( unique_ptr<Data>(array_data->getElementAt(0)),
                           unique_ptr<Data>(array_data->getElementAt(bsize_int-1)),
                           array_time, array_data );
        profiler_data_add(&prof, bsize);
    }


    profiler_data_save(&prof, opt.prof_file_name);

    profiler_data_release(&prof);
//    ReleaseInstance();

    return 0;
}




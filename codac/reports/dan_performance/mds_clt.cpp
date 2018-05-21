#include <iostream>
#include <string>
#include <sstream>
#include <mdsobjects.h>

#include <Core/unique_ptr.h>
#include <Core/Options.h>


#include "dan/common_defs.h"
#include "dan/timestamp.h"
#include "dan/prof_service.h"
#include "dan/dan_log_service.h"
#include "dan/archive/archive_api.h"

#include "dan_performance.h"
#include "MdsipInstancer.h"


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
    int m_server_port;

    mdsclt_Options(int argc, char *argv[])
    {
        // init defaults //
        strcpy(file_name,"mdsclt_data");
        strcpy(prof_file_name,"mdsclt");
        m_block_size = "4K";
        m_size = "1G";
        m_buffer_size = "8K";
        m_server_name = "localhost";
        m_server_port = 8000;

        // setup //
        this->SetUsage("usage: mds-clt [options]");
        this->AddOptions()
                ("file",  &file_name,  "output file name")
                ("prof-file",  &prof_file_name,  "profiler output file name")
                ("size",    &m_size,        "output data size")
                ("db-size", &m_block_size,  "data block size")
                ("bf-size", &m_buffer_size, "server buffer size")
                ("server_name", &m_server_name, "server name")
                ("server_port", &m_server_port, "server port")
                ;
        this->Parse(argc,argv);
    }

    size_t size() const { return options_get_size(m_size); }
    size_t block_size() const { return options_get_size(m_block_size); }
};





int main(int argc, char *argv[])
{
    mdsclt_Options opt(argc,argv);

    size_t bsize = opt.block_size();
    size_t bsize_int = bsize/sizeof(int32_t);
    size_t bsize_int16 = bsize/sizeof(int16_t);
    size_t ndb   = opt.size()/opt.block_size();
    float  dt = 1.E-6;

    ProfilerData prof;
    profiler_data_init(&prof, ndb+1);

    std::string path = opt.file_name + std::string("_path");
    setenv(path.c_str(),".",1);

    // START MDSIP //
    std::string addr;
    testing::MdsIpInstancer *mdsip = 0;
    if(opt.m_server_name == "localhost") {
        std::cout << "Instancing local mdsip server ... \n";
        mdsip = new testing::MdsIpInstancer("tcp",opt.m_server_port);
        addr = mdsip->getAddress();
    }
    else {
        std::cout << "Connecting remote mdsip server ... \n";
        std::stringstream ss;
        ss << "tcp" << "://" << opt.m_server_name << ":" << opt.m_server_port;
        addr = ss.str();
    }

    std::cout << "attempt to connect to: " << addr << "\n" << std::flush;
    unique_ptr<Connection> cnx = NULL;
    int retry = 5;
    for (;!cnx;) try {
        cnx = new Connection(const_cast<char*>(addr.c_str()));
    } catch (...) {
        if (--retry<0) throw;
        std::cout << "retry\n" << std::flush;
        usleep(500000);
    }

    // connection //
    cnx = new Connection(const_cast<char*>(addr.c_str()));

    { // create tree //
        Data * args[1];
        args[0] = new String(opt.file_name);
        cnx->get(
                    "write(*,' ------ CREATE TREE  ---------');"
                    " _status = TreeOpenNew($1,1);"
                    " _status = TreeShr->TreeAddNode('test','ANY');"
                    " _status = TreeShr->TreeWriteTree($1,val(1));"
                    " _status = TreeClose($1,1);"
                    ,args,1);
        deleteData(args[0]);
    }


    // open tree
    cnx->openTree(opt.file_name,1);

    // prepare data //
    int *array = (int*)malloc(bsize);
    for(size_t i=0; i<bsize_int; ++i) array[i]=i;
    unique_ptr<Int32Array> array_data = new Int32Array(array,bsize_int);


    // init profiles with first chunk //
    profiler_data_add(&prof, 0);
    for(size_t i=0; i<ndb; ++i) {
        float len = bsize_int*dt;
        unique_ptr<Range> array_time = new Range(new Float32(i*len), new Float32((i+1)*len), new Float32(dt));
        char * begin = array_time->getBegin()->getString();
        char * end   = array_time->getEnding()->getString();
        char * delta = array_time->getDeltaVal()->getString();
        //
        // TDI: public fun MakeSegment(as_is _node, in _start, in _end,
        //          as_is _dim, in _array, optional _idx, in _rows_filled)
        std::stringstream ss;
        ss << "MakeSegment("
           << "test" << ","
           << begin << ","
           << end << ","
           << "make_range(" << begin << "," << end << "," << delta << ")" << ","
           << "$1" << ",,"
           << array_data->getSize() << ")";
        //        ss << "size($1);";
        Data *args[] = { array_data };
        cnx->get(ss.str().c_str(),args,1);

        profiler_data_add(&prof, bsize);
        delete[] begin;
        delete[] end;
        delete[] delta;
    }

    // colsing tree //
    cnx->closeTree((char*)"t_connect",1);

    // close profiler //
    profiler_data_save(&prof, opt.prof_file_name);
    profiler_data_release(&prof);

    if(mdsip) { delete mdsip; }
    return 0;
}




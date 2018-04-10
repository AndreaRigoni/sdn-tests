
#include <iostream>
#include <string.h>
#include <cstdlib>

#include "dan/common_defs.h"
#include "dan/timestamp.h"
#include "dan/prof_service.h"
#include "dan/dan_log_service.h"
#include "dan/archive/archive_api.h"

#include <Core/Options.h>

#include "dan_performance.h"



static time_sec_t ts_secs_0 = 0;
static time_nanosec_t ts_nanos_0 = 1460000000000000000UL;

////////////////////////////////////////////////////////////////////////////////
//  MAIN  //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


class danclt_Options : public Options {
  public:
    char file_name[PATH_MAX];
    char prof_file_name[PATH_MAX];
    std::string m_buffer_size;
    std::string m_block_size;
    std::string m_size;

    std::string m_server_name;
    std::string m_server_port;

    danclt_Options(int argc, char *argv[])
    {
        // init defaults //
        strcpy(file_name,"danclt.h5");
        strcpy(prof_file_name,"danclt");
        m_buffer_size = "8K";
        m_block_size = "4K";
        m_size = "1G";
        m_server_name = "localhost";
        m_server_port = "9999";

        // setup //
        this->SetUsage("usage: danraw [options]");
        this->AddOptions()
                ("file",  &file_name,  "output file name")
                ("prof-file",  &prof_file_name,  "profiler output file name")
                ("size",    &m_size,        "output data size")
                ("bf-size", &m_buffer_size, "server buffer size")
                ("db-size", &m_block_size,  "data block size")

                ("server_name", &m_server_name, "server name")
                ("server_port", &m_server_port, "server port")
                ;
        this->Parse(argc,argv);
    }

    size_t size() const { return options_get_size(m_size); }
    size_t buffer_size() const { return options_get_size(m_buffer_size); }
    size_t block_size() const { return options_get_size(m_block_size); }

};


int main(int argc, char *argv[])
{
    INIT_ERR;
    err_t res;
    danclt_Options opt(argc,argv);

    // Profiler instance //
    InitInstanceCounting("DAN_RAW_0001");
    GetInstanceNumber();

    ProfilerData prof;
    profiler_data_init(&prof, opt.buffer_size());

    // Metadata Cash init //
    dan_MetadataCash stream_meta;
    dan_metadata_cash_init(&stream_meta);

    // DEVICE SETUP //
    device_init(&stream_meta);
    config_init(&stream_meta);


    // pulse_init(&stream_meta);


    // DATA //
    char * data;
    size_t data_size;
    generate_data(&stream_meta,opt.block_size(),data,&data_size);

    size_t NDatablock = opt.size() / data_size;
    size_t n_samples = data_size/sizeof(int);
    double SamplingRate = n_samples;

    std::cout << " -- config -- \n";
    std::cout << "| size         = " << opt.m_size << "\n";
    std::cout << "| block size   = " << opt.m_block_size << "\n";
    std::cout << "| buf size     = " << opt.m_buffer_size << "\n";
    std::cout << "| N_blocks     = " << NDatablock << "\n";
    std::cout << "| n_samples    = " << n_samples << "\n";
    std::cout << "| SamplingRate = " << SamplingRate << "\n";

    ////////////////////////////////////////////////////////////////////////////////
    //  CLT STREAM  ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    // INIT SUBSCRIBER //
    subscriber_init(&stream_meta);

    DAN_CLIENT_CONFIG config_clt;
    init_client_config(&config_clt, "DANCLT");
    strcpy(config_clt.serverName, opt.m_server_name.c_str());
    //    strcpy(config_clt.serverName, "spilds");
    strcpy(config_clt.serverPort, opt.m_server_port.c_str());
    config_clt.serverBufferSize = opt.buffer_size();

    std::cout
            << "\n"
            << " ---- CLIENT ------ \n"
            << config_clt
            << "\n"
            << "\n"
               ;

    ARCHIVE_STREAM_CLIENT *p_stream_0;
    ARCHIVE_STREAM_CLIENT *p_stream_1;
    ARCHIVE_STREAM_CLIENT_GROUP group;
    dan_create_stream_group(&group, 2, "GROUP1", "Pulse1");

    int meta_size;
    void *p_meta = dan_metadata_cash_get_all_data(&stream_meta, &meta_size);
    res = dan_create_archive_stream_in_group(&group, 0, meta_size, p_meta);
    res = dan_create_archive_stream_in_group(&group, 1, meta_size, p_meta);

    dan_open_stream_group ( &group, &config_clt );


    p_stream_0 = dan_get_archive_stream(&group,0);
    p_stream_1 = dan_get_archive_stream(&group,1);

    // init profiles with first chunk //
    profiler_data_add(&prof, 0);

    //////////////////////////////////////////////////////////////////////////////////
    // LOOP //
    for(size_t i=0; i<NDatablock; ++i) {


        DAN_CLT_DATA_BLOCK_HEADER header;
        header.pbl_data.dim1 = 1;
        header.pbl_data.dim2 = 1;
        header.pbl_data.operational_mode = 0; // TODO: what is this?
        header.pbl_data.reserve = 0;
        header.pbl_data.sampling_rate = SamplingRate; // Important to rebuild timestamps
        header.start_sample_id = i; // TODO: Comprehend why not i * n_samples ??
        dan_set_publisher_header(p_stream_0, sizeof(PUBLISHER_DATA_BLOCK_HEADER), &header.pbl_data);
        dan_set_publisher_header(p_stream_1, sizeof(PUBLISHER_DATA_BLOCK_HEADER), &header.pbl_data);

        time_nanosec_t ts_nanos;
        ts_nanos = ts_secs_0 * 1e9 + ts_nanos_0 + i * n_samples * 1e9 / SamplingRate;

        // NOTICE: you have to commit before any other write attempt //
        //
        dan_write_archive_datablock(p_stream_0, ts_nanos, i, n_samples, data);
        dan_commit_datablock(p_stream_0,0);
        profiler_data_add(&prof, data_size);

        dan_write_archive_datablock(p_stream_1, ts_nanos, i, n_samples, data);
        dan_commit_datablock(p_stream_1,0);
        profiler_data_add(&prof, data_size);
    }
    //////////////////////////////////////////////////////////////////////////////////


    dan_close_stream_group(&group);

    // profiler_data_save(Prof, FileName);
    profiler_data_save(&prof, opt.prof_file_name);
    profiler_data_release(&prof);

    // Release profiler instance //
    ReleaseInstance();

    RETURN_ERR;
}





























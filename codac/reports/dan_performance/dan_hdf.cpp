
#include <iostream>
#include <string.h>
#include <cstdlib>

#include "dan/common_defs.h"
#include "dan/timestamp.h"
#include "dan/prof_service.h"
#include "dan/dan_log_service.h"


// file I/O
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

// #include "dan/dan_cMetadataCash.h"
// #include "dan_metadata_cache.h"
#include "dan_performance.h"

#include <Core/Options.h>

#include "dan/archive/archive_api.h"

#include "dan/archive/hdf_api.h"
#include "dan/archive/hdf_common_api.h"

#include <Core/Timer.h>


static time_sec_t ts_secs_0 = 0;
static time_nanosec_t ts_nanos_0 = 1460000000000000000UL;


////////////////////////////////////////////////////////////////////////////////
//  MAIN  //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


class danhdf_Options : public Options {
  public:
    char file_name[PATH_MAX];
    char prof_file_name[PATH_MAX];
    std::string m_buffer_size;
    std::string m_block_size;
    std::string m_size;

    danhdf_Options(int argc, char *argv[])
    {
        // init defaults //
        strcpy(file_name,"danhdf.h5");
        strcpy(prof_file_name,"danhdf");
        m_buffer_size = "8K";
        m_block_size = "4K";
        m_size = "1G";
        // setup //
        this->SetUsage("usage: danraw [options]");
        this->AddOptions()
                ("file",  &file_name,  "output file name")
                ("prof-file",  &prof_file_name,  "profiler output file name")
                ("size",    &m_size,        "output data size")
                ("bf-size", &m_buffer_size, "buffer size")
                ("db-size", &m_block_size,  "data block size")
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
    danhdf_Options opt(argc,argv);

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

    // INIT SUBSCRIBER //
    subscriber_init(&stream_meta);

    // pulse_init(&stream_meta);


    // DATA //
    char * data;
    size_t data_size;
    generate_data(&stream_meta,opt.block_size(),data,&data_size);
    size_t NDatablock = opt.size() / data_size;

    std::cout << " -- config -- \n";
    std::cout << "| size        = " << opt.size() << "\n";
    std::cout << "| block size  = " << opt.block_size() << "\n";
    std::cout << "| N_blocks    = " << NDatablock << "\n";

    ////////////////////////////////////////////////////////////////////////////////
    //  HDF STREAM  ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    dansrv_set_server_id("TEST_DAN_PERFORMANCE");

    // CREATE_STREAM_GROUP //
    ARCHIVE_STREAM_GROUP group;
    GROUP_HEADER group_header;
    group_header.n_streams = 1;
    strcpy(group_header.subscriber_id, "TEST1");
    strcpy(group_header.group_name, "StreamGroup1");
    strcpy(group_header.session_id, "Pulse1");

    res = dansrv_create_stream_group(&group, &group_header);
    if(res < 0) std::cerr << "| error creating stream group \n";

    // CREATE STREAM //
    // TODO: explicitate the overall group creation within the hdf file //
    int meta_size;
    void *p_meta = dan_metadata_cash_get_all_data(&stream_meta, &meta_size);
    ARCHIVE_STREAM *p_stream = dansrv_create_stream_in_group(&group, 0, meta_size, p_meta);
    if (!p_stream) {
        std::cerr << "| error creating stream \n"; return 1;
    }

    // OPEN STREAM GROUP //
    // open
    dansrv_open_stream_group(&group, 1); // 1=write


    // /////////////////////////////////////// //
    // PROBLEMA: buffer molto piccolo pochi KB //
    // /////////////////////////////////////// //
    //
    HDF_DATASET *samples_ds = &p_stream->samples_dataset;
    HDF_DATASET *block_ds   = &p_stream->block_dataset;

    std::cout << std::endl
              << std::endl
              << " == HDF_DATASET == \n"
              << "1) BLOCK: \n"
              << block_ds
              << std::endl
              << "2) SAMPLES: \n"
              << samples_ds
              << "\n";

    size_t n_samples = data_size/sizeof(int);
    double SamplingRate = n_samples;

    // init profiles with first chunk //
    profiler_data_add(&prof, 0);

    Timer time;
    time.Start();

    dansrv_set_error_tag(p_stream, 0);

    for(size_t i=0; i<NDatablock; ++i) {

        DAN_CLT_DATA_BLOCK_HEADER header;
        header.pbl_data.dim1 = 1;
        header.pbl_data.dim2 = 1;
        header.pbl_data.operational_mode = 0; // TODO: what is this?
        header.pbl_data.reserve = 0;
        header.pbl_data.sampling_rate = SamplingRate; // Important to rebuild timestamps
        header.start_sample_id = i; // TODO: Comprehend why not i * n_samples ??

        dansrv_set_client_datablock_header(p_stream, sizeof(header), &header);

        //  RAW DATA ... NO HEADER
        //
        //    hdfsrv_write_data(samples_ds, data_size, data);
        //
        //  RAW DATA IN CHUNKS ... NO HEADER
        //
        //    int is_new_chunk;
        //    int is_flushed;
        //    hdfsrv_write_data_chunk(p_samples_dataset, size_db_items, data,
        //                            &is_new_chunk, &is_flushed);

        //
        // Build sequential samples using nanoseconds ...
        //
        // TODO: Think about the use of nanoseconds for timestamps and the
        // difference between seconds and nanoseconds provided by compile time
        // definition
        //
        //
        time_sec_t     ts_secs;
        time_nanosec_t ts_nanos;
        ts_secs  = ts_secs_0;
        ts_nanos = ts_secs_0 * 1e9 + ts_nanos_0 + i * n_samples * 1e9 / SamplingRate;

        dansrv_write_datablock(p_stream,
                               ts_secs,
                               ts_nanos,
                               n_samples, data);

        // WARNING: Seems that when some payloads are not properly sent data
        // looses the correct timestamps

        profiler_data_add(&prof, data_size);
    }

    // CLOSE STREAM GROUP //
    res = dansrv_close_stream_group(&group);

    // RELEASE STREAM CROUP //
    dansrv_release_stream_group(&group);

    //////////////////////////////////////////////////////////////////////////////////

    // profiler_data_save(Prof, FileName);
    profiler_data_save(&prof, opt.prof_file_name);
    profiler_data_release(&prof);

    // Release profiler instance //
    ReleaseInstance();

    RETURN_ERR;
}






























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
#include "dan/archive/archive_api.h"

#include "dan_performance.h"
#include <Core/Options.h>

////////////////////////////////////////////////////////////////////////////////
//  MAIN  //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class danraw_Options : public Options {
  public:
    char file_name[PATH_MAX];
    char prof_file_name[PATH_MAX];
    std::string m_buffer_size;
    std::string m_block_size;
    std::string m_size;

    danraw_Options(int argc, char *argv[])
    {
        // init defaults //
        strcpy(file_name,"danraw.out");
        strcpy(prof_file_name,"danraw");
        m_buffer_size = "512K";
        m_block_size = "512K";
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
    danraw_Options opt(argc,argv);

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
    // dummy_pulse_init(&stream_meta);

    // create directory based on file_name //
    TRY
    CHECK_ERR_LOG(archsrv_create_dir_for_file(opt.file_name),
                  "Cannot create folder for the file |%s|",
                  opt.file_name
                  );
    FINALLY;


    int hfile = open(opt.file_name, O_CREAT | O_TRUNC | O_RDWR);
    // if (hfile < 0) THROW_LOG("Cannot open file |%s|", opt.file_name);

    char * data;
    size_t data_size;
    generate_data(&stream_meta,opt.block_size(),data,&data_size);
    size_t NDatablock = opt.size() / data_size;

    std::cout << "| size        = " << opt.m_size << "\n";
    std::cout << "| block sizez = " << opt.m_block_size << "\n";
    std::cout << "| N_blocks    = " << NDatablock << "\n";


    // init profiles with first data //
    profiler_data_add(&prof, 0);
    for(size_t i=0; i < NDatablock; i++) {
        // FOR LOOP //
        block_t res_len = write(hfile, data, data_size);
        if (res_len < data_size) {
            std::cerr << "error writing file";
            break;
        }
        profiler_data_add(&prof, res_len);
    }

    profiler_data_save(&prof,opt.prof_file_name);

    close(hfile);

    // profiler_data_save(Prof, FileName);
    profiler_data_release(&prof);

    // Release profiler instance //
    ReleaseInstance();

    RETURN_ERR;
}





























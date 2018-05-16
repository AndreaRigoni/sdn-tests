// ////////////////////////////////////////////////////////////////////////// //
//
// This file is part of the dan-tests project.
// Copyright 2018 Andrea Rigoni Garola <andrea.rigoni@igi.cnr.it>.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ////////////////////////////////////////////////////////////////////////// //


// "sys-headers.h" //
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>



#include <math.h>
#include <iostream>

// "ccs-headers.h" //
#include <log.h>
#include <sdn.h>
#include <tcn.h>


#include <dan/dan_Admin.h>
#include <dan/dan_DataCore.h>
#include <dan/dan_Monitor.h>

#include <pthread.h>



// ccStickers //
#include <Core/Thread.h>
#include <Core/ConditionVar.h>
#include <Core/WaitSubscribe.h>


#include <stdarg.h>


// "sdd-dan.h" //
#include "dan/dan_DataCore.h"
#include "dan/dan_Source.h"
static const char DummySrc[] = "DummySrc";
static const char TERM_CHAR_UP[] = "\e[A";


////////////////////////////////////////////////////////////////////////////////
// MONITOR  ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



/////
///// \brief The MonitorThread class
/////
//class MonitorThread : public Thread {
//    dan_DataCore &dc;
//    static const size_t update_val = 10;
//    size_t pos;
//    ConditionVar wc;
//    dan_Monitor monitor;

//public:
//    MonitorThread(dan_DataCore &dc) :
//        dc(dc),
//        pos(0),
//        wc(),
//        monitor(NULL)
//    {
//        int nsub = dc->datacoreInfo->nSubscribers;
//        int nsrc = dc->datacoreInfo->nSources;

//        for (int i=0; i<nsub; ++i) {
//            st_dan_Subscriber_Info &si = dc->datacoreInfo->subscribers_info[i];
//            std::cout << "sub: " << si.name << "\n";
//            si.ovfErrors = 0;
//            // initialize profilers //
//            dan_initProfiler(&si.latency,update_val);
//            dan_initProfiler(&si.throu,update_val);
//        }
//        for (int i=0; i<nsrc; ++i) {
//            st_dan_Source_Info &si = dc->datacoreInfo->sources_info[i];
//            std::cout << "src: " << si.name << "\n";
//            // initialize profilers //
//            dan_initProfiler(&si.wlat,update_val);
//            dan_initProfiler(&si.throu,update_val);
//        }
//    }

//    ///
//    /// \brief add_monitor
//    /// \param src_name   Monitor Source (and set NULL the other)
//    /// \param subscrib_name Monitor Subscriber (and set NULL the other)
//    ///
//    void set_monitor(const char *src_name, const char *subscrib_name) {
//        this->monitor = dan_monitor_initMonitor(dc,src_name,subscrib_name);
//    }

//    MonitorThread& operator ++() { if(pos++%update_val == 0) wc.notify(); return *this; }

//    virtual void InternalThreadEntry() {
//        while(1) {
//            wc.wait(); // wait for external call to wake up //
//            int nsub = dc->datacoreInfo->nSubscribers;
//            int nsrc = dc->datacoreInfo->nSources;
//            int line_waste = nsub + nsrc;
//            std::cout << ",-----------------------\n"; line_waste += 1;
//            for (int i=0; i<nsub; ++i) {
//                st_dan_Subscriber_Info &si = dc->datacoreInfo->subscribers_info[i];
//                double lat = dan_profiler_getAverageValue(&si.latency);
//                double tpt = dan_profiler_getAverageValue(&si.throu)/1024/1024;
//                std::cout << "s: " << si.name << " "
//                          << "l: " << lat << " "
//                          << "t: " << tpt << "[MBps] "
//                          << "e: " << si.ovfErrors << " "
//                          << "        \n";
//            }
//            for (int i=0; i<nsrc; ++i) {
//                st_dan_Source_Info &si = dc->datacoreInfo->sources_info[i];
//                double lat = dan_profiler_getAverageValue(&si.wlat);
//                double tpt = dan_profiler_getAverageValue(&si.throu)/1024/1024;
//                std::cout << "s: " << si.name << " "
//                          << "l: " << lat << "[ms] "
//                          << "t: " << tpt << "[MBps] "
//                          << "        \n";
//            }
//            if(this->monitor) {
//                dan_Monitor mon = this->monitor;
//                std::cout
//                          // Number of free bytes that are available in the DAQBuffer
//                          << "sr dataFree   " << dan_monitor_source_dataFree(mon)/1024 << " [KB]        \n"

//                          // Number of free datablock reference that are available in the queue
//                          << "sr queueFree  " << dan_monitor_source_queueFree(mon) << " [bref]       \n";

//                          // total throughput in bytes/second associated to the source that is monitored by dMon
//                          // << "sr throughput " << dan_monitor_source_thoughput(mon) << "        \n";
//                line_waste += 2;
//            }
//            std::cout << "`-----------------------\n"; line_waste += 1;

//            for(int i=0; i<line_waste;++i) {
//                std::cout << TERM_CHAR_UP;
//            }
//        }
//    }
//};


////////////////////////////////////////////////////////////////////////////////
//  BUFFER FILL  ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// "dan_MetadataTypes.h" //
#include "dan/common_defs.h"
typedef struct
{
    PUBLISHER_DATA_BLOCK_HEADER std_header;
    short pos_x;
    short pos_y;
} __attribute__((packed)) DAN_DATA_BLOCK_HEADER_TYPE_IMG;

// #include "pv_access.h"
#include "assert.h"

#ifndef MAX
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a > _b ? _a : _b; })
#endif

#define SAMPLE_TYPE float

extern int configureSDN();
extern int cleanupSDN();

volatile sig_atomic_t __terminate = 0;


void signal_handler (int signal)
{
    log_info("Received signal '%d' to terminate", signal);
    __terminate = true;
};


///
/// \brief initBufferWithData
/// \param buffer data buffer
/// \param size size of chunk
/// \param pos current position to generation start
/// \param R number of samples per second ( Sampling Rate )
/// \param T time period for sine
///
static void initBufferWithSineData(char *buffer, size_t size, size_t pos,
                                   double R, double T=1)
{
    SAMPLE_TYPE *data=(SAMPLE_TYPE *)buffer;
    pos  /= sizeof(SAMPLE_TYPE);
    size /= sizeof(SAMPLE_TYPE);
    for(size_t i=0;i<size;i++)
        data[i]=sin(2 * M_PI * (i+pos) / R  / T);
}

static SAMPLE_TYPE * alloc_static_sine(size_t N) {
    SAMPLE_TYPE *data = (SAMPLE_TYPE*)malloc(N*sizeof(SAMPLE_TYPE));
    for(size_t i=0;i<N;i++)
        data[i]=sin(2 * M_PI * (double)i / N);
    return data;
}

///
/// \brief initBufferWithStaticSineData
/// \param buffer data buffer to fill
/// \param size size of buffer in char
/// \param ipos initial position in char
///
static void initBufferWithStaticSineData(char *buffer, size_t size, size_t ipos)
{
    static SAMPLE_TYPE *data = NULL;
    static const size_t N = 1024000;
    if(!data) data = alloc_static_sine(N/sizeof(SAMPLE_TYPE));

    //  NAIVE FILL IN  //
    //
    //    pos  /= sizeof(SAMPLE_TYPE);
    //    size /= sizeof(SAMPLE_TYPE);
    //    SAMPLE_TYPE *buffer_t = (SAMPLE_TYPE *)buffer;
    //    for(size_t i=0;i<size;i++)
    //        buffer_t[i]=data[(i+pos)%N];

    //  EFFICIENT COPY  //
    //
    assert(size >= N);
    ipos %= N;
    memcpy(buffer, data+ipos, N-ipos);
    buffer += N-ipos;
    size_t stps = (size-ipos)/N;
    for(size_t i=0; i < stps; ++i ) {
        memcpy(buffer,data,N);
        buffer += N;
    }
    memcpy(buffer,data,(size-ipos)%N);
}



////////////////////////////////////////////////////////////////////////////////
//  MAIN  //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    // example of code to demonstrate a DAN source
    // DummySrc - which publishes 100 sample very 1 ms.

    dan_DataCore dc = NULL; // DAN Datacore reference declaration
    dan_Source   ds = NULL; // DAN source declaration

    int exi_err = 0;
    int tcn_err = 0;
    int dan_err = 0;
    size_t counter = 0;
    SAMPLE_TYPE *values = NULL;

    // DATA BLOCK HEADER //
    DAN_DATA_BLOCK_HEADER_TYPE_IMG blockHead;

    // SIGNALS FOR GRACEFUL TERMINATION //
    sigset(SIGTERM, signal_handler);
    sigset(SIGINT,  signal_handler);
    sigset(SIGHUP,  signal_handler);

    // LOG INIT //
    InitializeLOG();

    //  SYNC USING PVs //
    //    pon_err = ca_context_create(ca_disable_preemptive_callback);
    //    if(pon_err != ECA_NORMAL) {
    //        log_error("Error during PON init");
    //        exi_err = 1;
    //    }
    //
    //    PV<long> gen_rate_KB ("EC-DAN-ACQ:GEN_FREQ");    // KBps
    //    PV<long> gen_csiz_KB ("EC-DAN-ACQ:GEN_PKTSIZE"); // KB
    //    size_t gen_rate  = gen_rate_KB * 1024;
    //    size_t gen_csiz  = gen_csiz_KB * 1024;

    size_t gen_trate  = 10;   // [MB/s]
    size_t gen_csize  = 12;   // [KB]
    size_t gen_bsize  = 64;   // [KB]
    try { gen_trate = atoi(getenv("DAN_TESTS_TRATE")); } catch(...){}
    try { gen_csize = atoi(getenv("DAN_TESTS_CSIZE")); } catch(...){}
    try { gen_bsize = atoi(getenv("DAN_TESTS_BSIZE")); } catch(...){}

    // Block size constant in Bytes
    gen_trate *= 1024 * 1024;
    gen_csize *= 1024;
    gen_bsize *= 1024;

    size_t chunkSize  = gen_csize;
    size_t bufferSize = gen_bsize;
    double sampleRate = gen_trate / sizeof(SAMPLE_TYPE);

    std::cout << "| --- Current setup ---" << std::endl
              << "| srate: " << sampleRate << std::endl
              << "| csize: " << chunkSize << std::endl
              << "| bsize: " << bufferSize << std::endl
              ;

    // test duration [s]
    uint64_t tot_duration = 10E9; // 10s
    try { tot_duration = atoi(getenv("DAN_TESTS_DURATION"))*1E9; } catch(...){}

    log_info("[GEN] rate: %d, chunk size: %d", (long)gen_trate, chunkSize);

    // DAN DATACORE INIT ///////////////////////////////////////////////////////
    dc = dan_initLibrary();
    if (dc == NULL) {
        log_error("[GEN] Error during DAN init ");
        exi_err = 1;
        __terminate = true;
    }

    // TCN INIT ////////////////////////////////////////////////////////////////
    tcn_err = tcn_init();
    if (tcn_err != TCN_SUCCESS) {
        log_error( "[GEN] TCN init NOK : %s",tcn_strerror(tcn_err));
        exi_err =1;
        __terminate = true;
    }

    // DAN SOURCE PUBLISH //////////////////////////////////////////////////////
    // Initialize source with a internal DAQBuffer allocating a size in bytes
    // of ( blockSize * nBlocks * sampleSize )
    ds = dan_publisher_publishSource_withDAQBuffer( dc,
                                                    (const char *) DummySrc,
                                                    bufferSize );
    if (ds == NULL) {
        log_final("[GEN] Error while publishing DAN source %s ", DummySrc);
        exi_err =1;
        __terminate = true;
    }
    log_info("[GEN] DAN PUBLISH SOURCE OK ");


    // OPEN STREAM /////////////////////////////////////////////////////////////
    // Open a new stream in the source previously initialized with 1000
    // samples/second of sampling rate and 0 bytes of initial offset into
    // DAQBuffer.
    dan_publisher_openStream(ds,sampleRate,0);

    uint64_t till;
    uint64_t init_acq = 0;

    // TCN get time //
    tcn_err = tcn_get_time(&till);
    if (tcn_err != TCN_SUCCESS) {
        log_error( "[GEN] TCN gettime NOK : %s\n", tcn_strerror(tcn_err));
        exi_err =1;
        __terminate = true;
    } else {
        init_acq=till;
    }

    // FILL DATA ///////////////////////////////////////////////////////////////
    values = (SAMPLE_TYPE *) malloc ( 2 * MAX(gen_trate, chunkSize) );
    initBufferWithSineData((char *)values,
                           2 * MAX(gen_trate, chunkSize), // size Bytes
                           0,             // pos  Bytes
                           sampleRate,    // R
                           1);            // T (1s period)


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    /// LOOP ///

    // MONITOR //
//    MonitorThread mt(dc);
//    mt.set_monitor(DummySrc,0);
//    mt.StartThread();

    size_t pos = 0;
    while(!__terminate && ( tcn_wait_until(till,0) == TCN_SUCCESS )
                       && ( till-init_acq ) < tot_duration )
        // WAIT UNTIL ABSOLUTE TIME OR MAX DURATION //
    {

        // -- DATA BLOCK HEADER ---- //
        blockHead.std_header.dim1=1;
        blockHead.std_header.dim2=1;
        blockHead.std_header.operational_mode = 0;
        blockHead.std_header.sampling_rate    = sampleRate;
        //   blockHead.pos_x += 1;
        //   blockHead.pos_y += 1;

        // PUBLISHER PUT ///////////////////////////////////////////////////////
        //
        // Push new data block and its modified header into source
        // put NULL if you didn’t define blockHead
        dan_err = dan_publisher_putDataBlock (ds,
                                              till,
                                              ((char *)values) + pos,
                                              chunkSize,
                                              (char *)&blockHead);
        pos = (pos+chunkSize)%gen_trate;

        // Detected overflow in pass datablock reference queue
        if (dan_err == -1) {
            log_warning( "[GEN] %s, Queue overflow with policy %d\n",
                      DummySrc, dan_publisher_getCheckPolicy(ds));
        }
        // Detected overflow in DAQBuffer
        else if (dan_err == -2) {
            log_warning("[GEN] %s, DAQ buffer overflow with policy %d\n",
                     DummySrc, dan_publisher_getCheckPolicy(ds));
        }

        // log_info("[GEN] pkt-loop ... \n");
        // monitor update //
//        ++mt;

        // ACQ interval
        double dt_ns = (double)chunkSize / gen_trate * 1E9;
        till += (int)dt_ns;
        // ++counter;

    } // LOOP END //////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    log_info( "[GEN] DAN end of acquisition, closing all resources");

    // CLOSE STREAM //
    dan_publisher_closeStream(ds);

    if (values != NULL) { free(values); }

    // UNPUBLISH SOURCE //
    if (ds != NULL) {
        dan_publisher_unpublishSource(dc, ds);
    }

    // CLOSE DAN API //
    if (dc != NULL) {
        dan_closeLibrary(dc);
    }

    // CLOSE LOG //
    log_info( "[GEN] DAN end of acquisition, resources are now closed");
    log_flush_queue();
    log_finalize();
    TerminateLOG();

    return (exi_err );
}

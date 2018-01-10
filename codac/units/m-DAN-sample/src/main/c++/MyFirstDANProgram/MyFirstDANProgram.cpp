/******************************************************************************
 * $HeadURL: https://svnpub.iter.org/codac/iter/codac/dev/units/m-codac-unit-templates/branches/codac-core-5.3/templates/cpp-sdn/main/c++/prog/prog.cpp $
 * $Id: prog.cpp 63347 2016-02-21 00:27:22Z zagara $
 *
 * Project	: CODAC Core System
 *
 * Description	: Program template
 *
 * Author        : DAN team
 *
 * Copyright (c) : 2010-2016 ITER Organization,
 *				  CS 90 046
 *				  13067 St. Paul-lez-Durance Cedex
 *				  France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

/* Global header files */

/* Local header files */

/* sys-headers.h -- Part of the program template to host all OS-specific
   program header files */
#include "sys-headers.h"

/* ccs-headers.h -- Part of the program template to host all CCS-specific
   program header files */
#include "ccs-headers.h"

/* includetopics.h -- Created and maintained by SDD. It includes the list of
   header files for all the topics the application is participating to. */
#include "includetopics.h" 

/* topicvars.h -- Created and maintained by SDD. Contains the declaration of
   the application-specific topics and participants. The file is optional and
   associated to the configureSDN, cleanupSDN helper functions which can be
   replaced by explicit class instantiation */
#include "topicvars.h"

#include "sdd-dan.h"
#include "dan_MetadataTypes.h"
using namespace std; 

#define SAMPLE_TYPE float


extern int configureSDN();
extern int cleanupSDN(); 

volatile sig_atomic_t __terminate = 0;


void signal_handler (int signal)
{
    log_info("Received signal '%d' to terminate", signal);
    __terminate = true;
};

void initBufferWithData(char *buffer, long size, long initValue)
{
    long i;
    float *vals=(float *)buffer;
    for(i=0;i<(long)floor(size/sizeof(float));i++){
        vals[i]=sin((i+initValue)/1E6);
    }
}


int main(int argc, char **argv)
{
    // example of code to demonstrate a DAN source
    // DummySrc - which publishes 100 sample very 1 ms.

    dan_DataCore dc = NULL; // DAN Datacore reference declaration
    dan_Source   ds = NULL; // DAN source declaration

    int result;
    int exValue=0;
    int tcn_err=0;

    int blockSize  = 100;  // Block size constant
    int nBlocks    = 200;  // Number of blocks constant

    int sampleSize = sizeof(SAMPLE_TYPE);
    long    sampleCounter = 0;
    int     samplingRate = 10;
    float * values = NULL;
    int     counter = 0;

    uint64_t max_duration = 600000000;
    uint64_t till;
    uint64_t tBase;
    uint64_t init_acq = 0;

    // DATA BLOCK HEADER //
    DAN_DATA_BLOCK_HEADER_TYPE_IMG blockHead;

    // SIGNALS FOR GRACEFUL TERMINATION //
    sigset(SIGTERM, signal_handler);
    sigset(SIGINT,  signal_handler);
    sigset(SIGHUP,  signal_handler);

    // LOG INIT //
    InitializeLOG();

    // DAN DATACORE INIT //
    dc = dan_initLibrary();
    if (dc == NULL) {
        log_error("Error during DAN init ");
        exValue=1;
        goto END;
    }

    // TCN INIT //
    tcn_err = tcn_init();
    if (tcn_err != TCN_SUCCESS)
    {
        log_error( "TCN init NOK : %s",tcn_strerror(tcn_err));
        exValue=1;
        goto END;
    }

    // DAN SOURCE PUBLISH //
    // Initialize source with a internal DAQBuffer allocating a size in bytes
    // of ( blockSize * nBlocks * sampleSize )
    ds = dan_publisher_publishSource_withDAQBuffer( dc, (const char *) DummySrc,
                                                    blockSize * nBlocks * sampleSize);
    if (ds == NULL) {
        log_final("Error while publishing DAN source %s ", DummySrc);
        exValue=1;
        goto END;
    }
    log_info( "DAN PUBLISH SOURCE OK ");
    values = (SAMPLE_TYPE *)malloc( blockSize * sampleSize );

    // OPEN STREAM //
    // Open a new stream in the source previously initialized with 1000
    // samples/second of sampling rate and 0 bytes of initial offset into
    // DAQBuffer.
    dan_publisher_openStream(ds,samplingRate,0);


    // TCN get time //
    tcn_err = tcn_get_time(&till);
    if (tcn_err != TCN_SUCCESS)
    {
        log_error( "TCN gettime NOK : %s\n", tcn_strerror(tcn_err));
        exValue=1;
        goto END;
    }else{
        init_acq=till;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    while(!__terminate && ( tcn_wait_until_hr(till,&tBase,0) == TCN_SUCCESS )
                       && ( till-init_acq ) < max_duration )
        // WAIT UNTIL ABSOLUTE TIME OR MAX DURATION //
    {
        // To calculate first timeStamp (only first time) //
        if (tBase==0) {
            tcn_err = tcn_get_time(&tBase) - (blockSize * 1000000000 / samplingRate);
        }
        if (tcn_err != TCN_SUCCESS)
        {
            log_error( "TCN gettime NOK : %s\n", tcn_strerror(tcn_err));
            exValue=1;
            goto END;
        }

        // FILL DATA //
        initBufferWithData((char *)values, blockSize*sampleSize, sampleCounter);

        // -- DATA BLOCK HEADER ---- //
        blockHead.std_header.dim1=1;
        blockHead.std_header.dim2=1;
        blockHead.std_header.operational_mode=0;
        blockHead.std_header.sampling_rate=samplingRate;
        blockHead.pos_x +=1;
        blockHead.pos_y +=1;

        // PUBLISHER PUT ///////////////////////////////////////////////////////
        //
        // Push new data block and its modified header into source
        // put NULL if you didn’t define blockHead
        result=dan_publisher_putDataBlock (ds,
                                           till,
                                           (char *) values,
                                           blockSize*sampleSize,
                                           (char *)&blockHead);

        // Detected overflow in pass datablock reference queue
        if (result == -1) {
            log_info( "%s, Queue overflow with policy %d\n",
                      DummySrc, dan_publisher_getCheckPolicy(ds));
        }
        // Detected overflow in DAQBuffer
        else if (result == -2) {
            log_info("%s, DAQ buffer overflow with policy %d\n",
                     DummySrc, dan_publisher_getCheckPolicy(ds));
        }
        sampleCounter += blockSize;
        till = till + 1000000L ;

        counter++;
        if(counter%1000){
            log_info("logging 1000 sample \n");
        }
        // wait_for(100000000L);

    } // LOOP END //////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    log_info( "DAN end of acquisition, closing all resources");

    // CLOSE STREAM //
    dan_publisher_closeStream(ds);

END:
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
    log_info( "DAN end of acquisition, resources are now closed");
    log_flush_queue();
    log_finalize();    
    TerminateLOG();

    return (exValue);
}

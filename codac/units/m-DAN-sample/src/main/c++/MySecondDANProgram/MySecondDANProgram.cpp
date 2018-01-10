/******************************************************************************
* $HeadURL: https://svnpub.iter.org/codac/iter/codac/dev/units/m-codac-unit-templates/tags/CODAC-CORE-5.4.0/templates/cpp-sdn/main/c++/prog/prog.cpp.template $
* $Id: prog.cpp.template 75208 2017-02-03 09:19:23Z cesnikt $
*
* Project	: CODAC Core System
*
* Description	: MyFirstDANProgram program
*
* Author        : andrea
*
* Copyright (c) : 2010-2017 ITER Organization,
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

/* sys-headers.h -- Part of the program template to host all OS-specific program header files */
#include "sys-headers.h"

/* ccs-headers.h -- Part of the program template to host all CCS-specific program header files */
#include "ccs-headers.h"

/* includetopics.h -- Created and maintained by SDD. It includes the list of header files for all the topics the application is participating to. */
#include "includetopics.h"

/* topicvars.h -- Created and maintained by SDD. Contains the declaration of the application-specific topics and participants. */
#include "topicvars.h" /* The file is optional and associated to the configureSDN, cleanupSDN helper functions which can be replaced by explicit class instantiation */

#include "sdd-dan.h"
#include "sdd-data.h"

#include "dan_MetadataTypes.h"

#define SAMPLE_TYPE float

using namespace std;

/* Constants */

/* Type definition */

/* Global variables */

static bool __terminate = false;

/* Function declaration */

/* configure_sdn.cpp -- Created and maintained by SDD. It includes the definition of the application-specific objects and the registration of the topics and participants. */
extern int configureSDN(); /* These functions are optional and can be replaced by bespoke code in case explicit object creation and registration of the participants to the appropriate topic name is favoured. */
extern int cleanupSDN();

/* Internal function definition */

void signal_handler (int signal)
{

    log_info("Received signal '%d' to terminate", signal);
    __terminate = true;

};

int main(int argc, char **argv)
{

    /* Install signal handler to support graceful termination */
    sigset(SIGTERM, signal_handler);
    sigset(SIGINT,  signal_handler);
    sigset(SIGHUP,  signal_handler);

    /* Initialize logging library. */
    InitializeLOG();

    log_info("START PROGRAM 2");

    if (initializeSDN() != SR_OK)
    {
        log_final("initializeSDN() failed CACCCCCOLAAAA");
        return (1);
    }
    else {
        log_info("#");
        log_info("# SDN OK");
        log_info("#");
    }

    if (configureSDN() != SR_OK) /* This function is optional and can be replaced by bespoke code in case explicit object creation and registration is favoured. */
    {
        terminateSDN();
        log_final("configureSDN() failed");
        return (1);
    }

    dan_DataCore dc = NULL;
    dan_Source ds = NULL;

    dc = dan_initLibrary();
    if(!dc) {
        log_error("Error during DAN init");
        exit(1);
    }


    int blockSize = 100;
    int nBloks = 200;
    int sampleSize = sizeof(SAMPLE_TYPE);
    long sampleCounter = 0;
    int result;

    // PUBLISH SOURCE //
    ds = dan_publisher_publishSource_withDAQBuffer( dc,
                                                    srcSecond,
                                                    blockSize * nBloks * sampleSize);
    if (!ds) {
        log_error("Error publishing source %s",srcSecond);
        exit(1);
    }

    int samplingRate = 1000;
    float *values;
    values = (SAMPLE_TYPE *)malloc(blockSize * sampleSize);
    DAN_DATA_BLOCK_HEADER_TYPE_IMG blockHead;

    // OPEN STREAM //
    dan_publisher_openStream(ds, samplingRate, 0);

    uint64_t till;

    // LOOP //
    while (__terminate != true)
    {
        for (int i=0; i < blockSize; ++i ) values[i] = sin((float)(i+till)/1E3);
        result = dan_publisher_putDataBlock(ds,
                                            till,
                                            (char *) values,
                                            blockSize * sampleSize,
                                            (char *) &blockHead);
        if(result == -1) {
            log_info("Queue OVERFLOW");
        }
        else if (result == -2) {
            log_info("DAQ buffer overflow");
        }

        wait_for(1000L);
        till += 1000L;
        if(till > 1000000L)
            __terminate = true;
    }

    dan_publisher_closeStream(ds);

    dan_publisher_unpublishSource(dc,ds);

    dan_closeLibrary(dc);

    /* Unregister participants and delete application-specific objects. */
    cleanupSDN();

    /* Release SDN resources. */
    terminateSDN();

    /* Terminate logging library. */
    TerminateLOG();

    return (0);

}

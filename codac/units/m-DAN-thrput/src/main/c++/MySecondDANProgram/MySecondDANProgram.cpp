/******************************************************************************
* $HeadURL: https://svnpub.iter.org/codac/iter/codac/dev/units/m-codac-unit-templates/tags/CODAC-CORE-5.4.0/templates/cpp-sdn/main/c++/prog/prog.cpp.template $
* $Id: prog.cpp.template 75208 2017-02-03 09:19:23Z cesnikt $
*
* Project	: CODAC Core System
*
* Description	: MySecondDANProgram program
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

  /* Initialize SDN - Creates the necessary resources and verifies machine-specific configuration. */
  if (initializeSDN() != SR_OK)
    {
      log_final("initializeSDN() failed");
      return (1);
    }
  
  /* Configure SDN application - Creates application-specific participants and register them to use appropriate topic names. */
  if (configureSDN() != SR_OK) /* This function is optional and can be replaced by bespoke code in case explicit object creation and registration is favoured. */
    {
      terminateSDN();
      log_final("configureSDN() failed");
      return (1);
    }
  
  while (__terminate != true) 
    {

      /* -- User shall insert here application-specific code to publish/receive data -- */
      /* -- Refer to SDN Software User Manual for the documentation of the SDN API -- */
      
      /* -- API call to publish data on a topic -- */
      // publisher_xxx.publish(p_userdata_xxx); // Replacing xxx with the appropriate topic name
      
      /* -- API call to receive data on a topic - Blocking call with optional timeout -- */
      // subscriber_yyy.receive(s_userdata_yyy); // Replacing yyy with the appropriate topic name 
      
      /* -- API call to define an application-specific callback to receive data on a topic - Non-blocking mode -- */
      // subscriber_yyy.setReceiveCallback(<callback>); 

      /* Just idle spinning */
      wait_for(100000000L);

    }
      
  /* Unregister participants and delete application-specific objects. */
  cleanupSDN(); /* This function is optional and can be replaced by bespoke code in case explicit object destruction is favoured. */
  
  /* Release SDN resources. */
  terminateSDN();
  
  /* Terminate logging library. */
  TerminateLOG();

  return (0);
  
}

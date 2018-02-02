/******************************************************************************
* $HeadURL: https://svnpub.iter.org/codac/iter/codac/dev/units/m-codac-unit-templates/tags/CODAC-CORE-5.4.0/templates/cpp-sdn/main/c++/prog/prog.cpp.template $
* $Id: prog.cpp.template 75208 2017-02-03 09:19:23Z cesnikt $
*
* Project	: CODAC Core System
*
* Description	: pv_test program
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
#include <stdlib.h>
#include <stdio.h>  /* sscanf, printf, etc. */
#include <unistd.h>
#include <string.h> /* strncpy, etc. */
#include <stdarg.h> /* va_start, etc. */
#include <signal.h> /* sigset, etc. */
#include <math.h>

#include <iostream>

#include <log.h>
#include <sdn.h>

#include "pv_access.h"


using namespace std;
static bool __terminate = false;



// Internal signal handling //
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

  InitializeLOG();

  const char * name = "EC-DAN-F000:T1";

  int status = 0;
  status = ca_context_create(ca_disable_preemptive_callback);
  if(status != ECA_NORMAL) {
      std::cerr << "ca_context_createfailed: " << ca_message(status)
                << std::endl;
      exit(1);
  }

  PV<double> pv(name);

  pv = pv + 1;
  std::cout << "data: " << pv << std::endl;


  // TODO: create a singleton that destoys because channel must be cleared
  // before this...
  //
  //  ca_context_destroy();


  TerminateLOG();
  return (0);

}

// MINIMAL PV CLIENT PROGRAM //
//
//  status = ca_create_channel (name,
//                              NULL,
//                              NULL,
//                              DEFAULT_CA_PRIORITY,
//                              &id);
//  if(status != ECA_NORMAL) {
//      log_error("ca_context_createfailed:\n%s\n", ca_message(status));
//      exit(1);
//  }
//  status = ca_pend_io(5);
//  log_info("hello man! channel opened, status is %s",ca_message(status));
//
//  double data = 0;
//  ca_get(DBR_DOUBLE,id,&data);
//  status = ca_pend_io(1);
//  log_info("ca_pend_io status is %s",ca_message(status));
//
//  log_info("caget value is %f",data);
//
//  ca_clear_channel(id);
//  ca_context_destroy();

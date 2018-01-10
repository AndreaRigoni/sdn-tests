//+======================================================================
// $HeadURL: https://svnpub.iter.org/codac/iter/codac/dev/units/m-sdd-translator-parts/tags/CODAC-CORE-5.4.0/org.iter.codac.sdd.translators/src/main/resources/templates/sdn/configure_sdn.cpp.vm $
// $Id: configure_sdn.cpp.vm 75208 2017-02-03 09:19:23Z cesnikt $
//
// Project       : CODAC Core System
//
// Description   : SDN Program - MySecondDANProgram
//
// Author        : Kirti Mahajan, Lana Abadie and TCS (link with velocity and SDD model)
//
// Copyright (c) : 2010-2017 ITER Organization,
//                 CS 90 046
//                 13067 St. Paul-lez-Durance Cedex
//                 France
//
// This file is part of ITER CODAC software.
// For the terms and conditions of redistribution or use of this software
// refer to the file ITER-LICENSE.TXT located in the top level directory
// of the distribution package.
//
//-======================================================================

#include <string.h>

#include <sdn.h>
#include "includetopics.h"

using namespace std;

/* Create variables for the topics on which application is publishing */
//+P=====================================================================
//-P=====================================================================

/* Create variables for topics on which application is subscribing */
//+S=====================================================================
//-S=====================================================================

int configureSDN()
{

  SR_RET sr_ret = SR_OK; 

  //+P=====================================================================
  //-P=====================================================================

  //+S=====================================================================
  //-S=====================================================================

  return sr_ret; 

}

int cleanupSDN()
{

  SR_RET sr_ret = SR_OK; 

  /* Unregister publisher */
  //+P=====================================================================
  //-P=====================================================================

  /* Unregister subscriber */
  //+S=====================================================================
  //-S=====================================================================

  return sr_ret;
 
}
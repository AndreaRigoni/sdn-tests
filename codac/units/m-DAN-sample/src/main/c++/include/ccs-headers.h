#ifndef CCS_HEADERS_H
#define CCS_HEADERS_H

/******************************************************************************
* $HeadURL: https://svnpub.iter.org/codac/iter/codac/dev/units/m-codac-unit-templates/tags/CODAC-CORE-5.4.0/templates/cpp/main/c++/include/ccs-headers.h $
* $Id: ccs-headers.h 75208 2017-02-03 09:19:23Z cesnikt $
*
* Project	: CODAC Core System
*
* Description	: CODAC Core System - Commonly used program headers
*
* Author        : Bertrand Bauvir
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

/* log.h -- Part of CCS. It includes the declaration of the logging library API. */
#include <log.h> /* This file is mandatory to compile this program against the logging library and API. */

/* sdn.h -- Part of CCS. It includes the declaration of the SDN library API. */
#include <sdn.h> /* This file is mandatory to compile this program against the SDN core library and API. */

/* tcn.h -- Part of CCS. It includes the declaration of the TCN library API. */
#include <tcn.h> /* This file is optional and can be removed if the program does not need to handle ITER time or operate on it. */

#endif /* CCS_HEADERS_H */

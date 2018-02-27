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



#include <danApi.h>
#include <iostream>

#include <log.h>

#include "assert.h"


static const char DummySrc[] = "DummySrc";
static const char mySubscriber[] = "mySubscriber";

int main(void) {
    std::cout << "Monitor utility\n";

    // LOG INIT //
    InitializeLOG();

    // DAN DATACORE INIT //
    dan_DataCore dc = dan_initLibrary();
    if (dc == NULL) {
        std::cerr << "Error during DAN init \n";
        exit(1);
    }

    // SUBSCRIBER //
    int sid = dan_admin_initSubscriber(dc,mySubscriber);
    std::cout << "sid = " << sid << std::endl;

    // MONITOR //
    dan_Monitor mon = dan_monitor_initMonitor(dc,DummySrc,mySubscriber);
    if(mon == NULL) {
        std::cerr << "error init monitor\n";
        exit(1);
    }

   const char anim[] = "-/|\\";
   int  acnt = 0;

   while(1) {
   std::cout << " ----- MONITOR (" << anim[acnt++%4] << ") ----- \n";
   std::cout << "s dataFree   " << dan_monitor_source_dataFree(mon) << "\n"
             << "s queueFree  " << dan_monitor_source_queueFree(mon) << "\n"
             << "s throughput " << dan_monitor_source_thoughput(mon) << "\n"
                ;
   std::cout << "\e[A" << "\e[A" << "\e[A" << "\e[A";

   }


    return 0;
}

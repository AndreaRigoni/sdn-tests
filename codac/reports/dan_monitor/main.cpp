#include "mainwindow.h"
#include <QApplication>

#include <iostream>

#include <dan.h>
#include <dan/dan_Admin.h>
#include <dan/dan_Source.h>
#include <dan/dan_DataCore.h>

extern "C" {
#include "dan_DataCore_st.h"
#include "dan_Source_st.h"
#include "dan_Subscriber_st.h"
#include "dan_Admin_st.h"
}


const char *DummySrcName = "DummySrc";
int64_t DummySrcBufferSize = 1024;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.show();
    return a.exec();
}

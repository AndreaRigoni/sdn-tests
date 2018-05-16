#include "icbstreamer.h"
#include "ui_icbstreamer.h"

extern "C" {
#include "dan_DataCore_st.h"
//#include "dan_Source_st.h"
//#include "dan_Subscriber_st.h"
//#include "dan_Admin_st.h"
}


ICBStreamer::ICBStreamer(const char *name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICBStreamer)
{
    ui->setupUi(this);
}

ICBStreamer::~ICBStreamer()
{
    delete ui;
}

#include <iostream>
#include <sstream>

#include "icbrowser.h"
#include "ui_icbrowser.h"

#include "mainwindow.h"
#include "ddcmodel.h"

#include <qvariant.h>

#include <dan/dan_Admin.h>
#include <dan/dan_Source.h>
#include <dan/dan_DataCore.h>

extern "C" {
#include "dan_DataCore_st.h"
#include "dan_Source_st.h"
#include "dan_Subscriber_st.h"
#include "dan_Admin_st.h"
}




////////////////////////////////////////////////////////////////////////////////
//  ICBROWSER  /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


ICBrowser::ICBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICBrowser)
{
    ui->setupUi(this);    
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(print_current_index()));

    m_model = new DDCTreeModel();
    ui->treeView->setModel(m_model);
    ui->treeView->reset();

    update(200);

}

ICBrowser::~ICBrowser()
{
    delete ui;
    foreach (dan_DataCore ddc, m_dclist) {
        dan_closeLibrary(ddc);
    }
}

void ICBrowser::update(u_int32_t delay)
{
    this->startTimer(delay);
}

void ICBrowser::icLoadFromXML(const char *xml_file)
{
    m_model->load_from_file(xml_file);
    ui->treeView->reset();
}

void ICBrowser::timerEvent(QTimerEvent *e)
{
    static int count = 0;
    QStatusBar * status = ((QMainWindow*)this->window())->statusBar();
    status->showMessage(QString("update :") + QString::number(count++));
    this->m_model->update();
    // ui->treeView->update();
}

void ICBrowser::print_current_index()
{
    QStatusBar * status = ((QMainWindow*)this->window())->statusBar();
    QModelIndex index = ui->treeView->currentIndex();

    // PRINT DATA TO STATUS BAR //
    // QVariant data = ui->treeView->model()->data(index);
    // QVariant data = index.data();
    QVariant data = index.sibling(index.row(),0).data();
    QString text = data.toString();
    status->showMessage(text);

    DDCTreeModelItem *it = (DDCTreeModelItem *)index.internalPointer();
    status->showMessage(it->getICName());

}

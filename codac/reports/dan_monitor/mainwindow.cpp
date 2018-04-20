#include "mainwindow.h"
#include "ui_mainwindow.h"


////////////////////////////////////////////////////////////////////////////////
//  MAIN WINDOW  ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    

    QVBoxLayout *layout = new QVBoxLayout( ui->frame );
    plot = new CpuPlot(ui->frame);
    layout->addWidget( plot );
    ui->statusBar->showMessage("qwt widget set!");
}

MainWindow::~MainWindow()
{    
    delete plot;
    delete ui;
}

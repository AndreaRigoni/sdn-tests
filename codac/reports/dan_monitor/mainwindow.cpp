#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include "icbrowser.h"
#include "ddcmodel.h"

////////////////////////////////////////////////////////////////////////////////
//  MAIN WINDOW  ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_plot_cpu(0),
    m_plot_stremers(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStatusBar(ui->statusBar);

    m_plot_layout = new QVBoxLayout( ui->frame );

    // DEFAULT VIEWPORT //
    showCPULoad();

    { // TOOLBOX //
        ui->toolBox->removeItem(0);
        browser = new ICBrowser(ui->toolBox);
        ui->toolBox->addItem(browser,"ICBrowser");
        browser->update(1000);
    }

    connect(ui->actionLoad_from_file,SIGNAL(triggered(bool)),this,SLOT(loadFromFile()));
    connect(ui->actioncpu_load,SIGNAL(triggered(bool)),this,SLOT(showCPULoad()));
    connect(ui->actionstremers_throughput,SIGNAL(triggered(bool)),this,SLOT(showStreamers()));
}

MainWindow::~MainWindow()
{    
    delete m_plot_cpu;
    delete ui;
    delete browser;
}

QStatusBar *MainWindow::getStatusBar()
{
    return ui->statusBar;
}

void MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open DAN XML configuration file"),
                                                    ".", tr("XML Files (*.xml)"));
    ui->statusBar->showMessage(QString("loading.. ")+ fileName);
    browser->icLoadFromXML(fileName.toLatin1().data());
}

void MainWindow::hideAllPlots()
{
    QLayoutItem *item = NULL;
    while ((item = m_plot_layout->takeAt(0)) != 0) {
        item->widget()->hide();
        m_plot_layout->removeItem(item);
        // delete item->widget();
    }
}

void MainWindow::showCPULoad()
{
    hideAllPlots();
    if(!m_plot_cpu) m_plot_cpu = new CpuPlot(ui->frame);
    m_plot_layout->addWidget( m_plot_cpu );
    m_plot_cpu->show();
    ui->statusBar->showMessage("qwt widget set!");
}

void MainWindow::showStreamers()
{
    hideAllPlots();
    if(!m_plot_stremers) m_plot_stremers = new VPStreamers(ui->frame);
    m_plot_layout->addWidget( m_plot_stremers );
    m_plot_stremers->show();
    m_plot_stremers->updateCurves(browser->getModel());
    ui->statusBar->showMessage("qwt streamers set!");
}


//QMainWindow *getMainWindow()
//{
//    foreach (QWidget *w, qApp->topLevelWidgets())
//        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
//            return mainWin;
//    return NULL;
// }

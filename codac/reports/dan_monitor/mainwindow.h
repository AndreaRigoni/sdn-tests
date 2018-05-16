#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>

#include "cpustat.h"
#include "vpstreamers.h"

////////////////////////////////////////////////////////////////////////////////
//  MAIN WINDOW  ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStatusBar *getStatusBar();

public slots:
    void loadFromFile();

    void hideAllPlots();
    void showCPULoad();
    void showStreamers();

private:
    Ui::MainWindow *ui;
    QLayout     *m_plot_layout;
    CpuPlot     *m_plot_cpu;
    VPStreamers *m_plot_stremers;
    class ICBrowser *browser;
};




#endif // MAINWINDOW_H

#include <qstringlist.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <QStatusBar>
#include <qstring.h>
#include <string>


#define QWT_NO_OPENGL 1
#ifndef QWT_NO_OPENGL
#define USE_OPENGL 1
#endif

#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>

#if USE_OPENGL
#include <QtOpenGL/qgl.h>
#include <qwt_plot_glcanvas.h>
#else
#include <qwt_plot_canvas.h>
#endif

#include "cpustat.h"

////////////////////////////////////////////////////////////////////////////////
//  CPUPLOT  ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


CpuPlot::CpuPlot( QWidget *parent ):
    QwtPlot( parent )
{
    setAutoReplot( false );

#if USE_OPENGL
#if QT_VERSION >= 0x040600 && QT_VERSION < 0x050000
    QGL::setPreferredPaintEngine( QPaintEngine::OpenGL );
#endif
    QwtPlotGLCanvas *canvas = new QwtPlotGLCanvas();
    setCanvas( canvas );
    canvas->setFrameStyle( QFrame::NoFrame );
#else
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setBorderRadius( 5 );
    setCanvas( canvas );
#endif

    plotLayout()->setAlignCanvasToScales( true );

    setAxisTitle( QwtPlot::xBottom, " System Uptime [h:m:s]" );
    //  setAxisScale( QwtPlot::xBottom, 0, 10 );
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );

    setAxisTitle( QwtPlot::yLeft, "Cpu Usage [%]" );
    setAxisScale( QwtPlot::yLeft, 0, 100 );


    this->curve = new QwtPlotCurve("user");

    x.push_back(0);
    y.push_back(0);
    curve->setSamples(x,y);
    curve->attach(this);
    curve->setVisible( true );

    replot();
    timer.Start();
    (void)startTimer(20);
}

void CpuPlot::timerEvent(QTimerEvent *e)
{
    static CpuStat cpu;
    double user,system;
    cpu.statistic(user,system);

    (void)e;
    x.push_back(timer.StopWatch());
    y.push_back(user + system);

    if(x.size() > 100) {
        x.pop_front();
        y.pop_front();
    }
    curve->setSamples(x,y);
    setAxisScale( QwtPlot::xBottom, x.front(), x.back() );

    QStatusBar * status = ((QMainWindow*)this->window())->statusBar();
    status->showMessage("running...");

    replot();
}


////////////////////////////////////////////////////////////////////////////////
//  CPUSTAT  ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



CpuStat::CpuStat()
{
    lookUp( procValues );
}

QTime CpuStat::upTime() const
{
    QTime t( 0, 0, 0 );
    for ( int i = 0; i < NValues; i++ )
        t = t.addSecs( int( procValues[i] / 100 ) );

    return t;
}

void CpuStat::statistic( double &user, double &system )
{
    double values[NValues];

    lookUp( values );

    double userDelta = values[User] + values[Nice]
        - procValues[User] - procValues[Nice];
    double systemDelta = values[System] - procValues[System];

    double totalDelta = 0;
    for ( int i = 0; i < NValues; i++ )
        totalDelta += values[i] - procValues[i];

    user = userDelta / totalDelta * 100.0;
    system = systemDelta / totalDelta * 100.0;

    for ( int j = 0; j < NValues; j++ )
        procValues[j] = values[j];
}

void CpuStat::lookUp( double values[NValues] ) const
{
    QFile file( "/proc/stat" );
    if ( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream textStream( &file );
        do
        {
            QString line = textStream.readLine();
            line = line.trimmed();
            if ( line.startsWith( "cpu " ) )
            {
                const QStringList valueList =
                    line.split( " ",  QString::SkipEmptyParts );
                if ( valueList.count() >= 5 )
                {
                    for ( int i = 0; i < NValues; i++ )
                        values[i] = valueList[i+1].toDouble();
                }
                break;
            }
        }
        while( !textStream.atEnd() );
    }
}

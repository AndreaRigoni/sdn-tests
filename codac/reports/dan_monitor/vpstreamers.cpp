
#include <iostream>

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

#include <dan.h>
#include <dan/dan_Monitor.h>

extern "C" {
#include "dan_DataCore_st.h"
#include "dan_Source_st.h"
#include "dan_Subscriber_st.h"
#include "dan_Admin_st.h"
}


#include "vpstreamers.h"




////////////////////////////////////////////////////////////////////////////////
//  VP STREAMERS ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

VPStreamers::VPStreamers(QWidget *parent) :  QwtPlot( parent )
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
    setAxisScale( QwtPlot::xBottom, 0, 10 );
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );

    setAxisTitle( QwtPlot::yLeft, "Stremers Throughput [MBps]" );
    // setAxisScale( QwtPlot::yLeft, 0, 100 );


}

void VPStreamers::updateCurves(DDCTreeModel *model)
{

    //    // m_streamers = model->getStreamersList();
    foreach(Streamer *st, model->getStreamersList()) {
        st_dan_Subscriber_Info *info = (st_dan_Subscriber_Info *)st->m_dataStreamInfo;
        std::cout << " got streamer: " << info->name
                  << " src: " << info->nSources
                  //<< " thr: " << info->throu.average
                  << "\n";
    }



}











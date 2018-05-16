#ifndef VPSTREAMERS_H
#define VPSTREAMERS_H

#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qdatetime.h>

#include "ddcmodel.h"

class VPStreamers : public QwtPlot
{
    Q_OBJECT
public:
    explicit VPStreamers(QWidget *parent = 0);

    void updateCurves(DDCTreeModel *model);
signals:

public slots:

private:
    QList<QwtPlotCurve> m_curves;
    QList<Streamer *>   m_streamers;
};

#endif // VPSTREAMERS_H

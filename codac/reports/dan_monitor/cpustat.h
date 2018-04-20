#include <vector>
#include <QMainWindow>
#include <qwt_plot.h>
#include <qdatetime.h>

#include <Core/Timer.h>

////////////////////////////////////////////////////////////////////////////////
//  CPUSTAT  ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class CpuStat
{
public:
    CpuStat();
    void statistic( double &user, double &system );
    QTime upTime() const;

    enum Value
    {
        User,
        Nice,
        System,
        Idle,

        NValues
    };

private:
    void lookUp( double[NValues] ) const;
    double procValues[NValues];
};


////////////////////////////////////////////////////////////////////////////////
//  CPU PLOT  //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class QwtPlotCurve;

///
/// \brief The CpuPlot class
///
class CpuPlot : public QwtPlot
{
    Q_OBJECT

    enum CpuData
    {
        User,
        System,
        Total,
        Idle,

        NCpuData
    };

    Timer timer;
    QVector<double> x,y;
    QwtPlotCurve *curve;
    double cpu_val[NCpuData];

public:
    CpuPlot( QWidget *parent = 0 );

protected:
    void timerEvent( QTimerEvent *e );

};



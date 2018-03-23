
#include "stdio.h"
#include "unistd.h"
#include "sys/time.h"

#include <string>
#include <iostream>
// #include <regex>

#include <Core/Options.h>
#include <Core/Timer.h>

#include <Math/Accumulator.h>
#include <Math/Histogram.h>

#include <Graphic2D/Plot2D.h>


#include "Math/StatisticUtils.h"



//size_t options_get_size(const char *p_size)
//{
//    int len = strlen(p_size);
//    if (len == 0) return -1;
//    char last_char = p_size[len - 1];
//    if (isdigit(last_char)) return atol(p_size);
//    if (len == 1) return -1;
//    switch (last_char)
//    {
//        case 'K': return atol(p_size) * 1024UL;
//        case 'M': return atol(p_size) * 1048576UL;
//        case 'G': return atol(p_size) * 1073741824UL;
//        case 'T': return atol(p_size) * 1099511627776UL;
//        case 'E': return atol(p_size) * 1125899906842620UL;
//    }
//    return -1;
//}




int main(int argc, char *argv[])
{
    size_t size_G = 1;
    size_t bs_K   = 512;
    size_t bufs_K = 1024;
    std::string logfile = std::string(argv[0])+".log";

    // ARGS PARSE //
    Options opt;
    opt.AddOptions()
            ("size", &size_G, "total write size [GB]")
            ("bs"  , &bs_K,   "block size [KB]")
            ("bufs", &bufs_K, "buffer size [KB]")
            ("logfile", &logfile, "csv log file out")
            ;
    opt.Parse(argc,argv);
    const char *file_out = argv[1];

    char *buffer = (char*)malloc(bufs_K*1024);
    for ( size_t i=0;i<bufs_K*1024;++i)
        buffer[i] = (char)i;

    Histogram<double> lat("lat",400,0,8);
    Histogram<double> bw("bw",400,0,4E3);


    static const size_t MI_WINDOW = 400;
    StatUtils::MI     lat_MI(MI_WINDOW);
    StatUtils::MA     bw_MA(MI_WINDOW);

    Curve2D           bw_MI_curve("bw MI");
    Curve2D           bw_MA_curve("bw MA");
    Curve2D           lat_curve("lat");
    Curve2D           bw_curve("bw");
    Curve2D           bwm_curve("bwm");

    std::cout
            << "size: " << size_G << "\n"
            << "bs  : " << bs_K << "\n"
            << "bufs: " << bufs_K << "\n";

    FILE *file = fopen(file_out,"wb");
    Timer time;
    size_t count=0,tcnt=0;
    double t0=0,t1;
    time.Start();
    for (size_t i=0; i<size_G*1024*1024/bs_K; /*i+=count*/) {
        count = fwrite(buffer,bs_K*1024,bufs_K/bs_K,file);
        i+=count;
        t1 = time.StopWatch_ms();
        if(count>0) {
            double dt = t1-t0; t0=t1;
            lat.Push(dt/count);
            lat_MI.add(dt/count);
            double tp_i   = 1.*bs_K/1024*count/dt*1000;
            double tp_all = 1.*bs_K/1024*i/time.GetElapsed_s();
            double tp_mi  = 1.*bs_K/1024*1000*lat_MI.size()/lat_MI.sum();
            bw_MA.add(tp_mi);
            bw.Push(tp_mi);
            if (time.GetElapsed_ms() > (100)*tcnt) {
                lat_curve.AddPoint(Curve2D::Point(time.GetElapsed_s(),
                                                  dt/count,0));
                bwm_curve.AddPoint(Curve2D::Point(time.GetElapsed_s(),
                                                  tp_all,0));
                bw_curve.AddPoint(Curve2D::Point(time.GetElapsed_s(),
                                                 tp_i,0));
                bw_MI_curve.AddPoint(Curve2D::Point(time.GetElapsed_s(),
                                                    tp_mi,0));
                bw_MA_curve.AddPoint(Curve2D::Point(time.GetElapsed_s(),
                                                    bw_MA.mean(),bw_MA.rms()));
                tcnt++;
            }
        }
    }

    std::cout << " --- latency [ms] ---- \n"
              << "lat min:  " << lat.Min() << std::endl
              << "lat mean: " << lat.MeanAll() << std::endl
              << "lat max:  " << lat.Max() << std::endl;

    std::cout << " --- bw [MBps] ---- \n"
              << "bw min:  " << bw.Min() << std::endl
              << "bw mean: " << bw.MeanAll() << std::endl
              << "bw max:  " << bw.Max() << std::endl;

    // std::cout << lat << "\n";
    // std::cout << bw  << "\n";

    Plot2D plot("lat_histogram");
    // plot.AddCurve(bw);
    plot.AddCurve(lat);
    plot.PrintToGnuplotFile("bw_h");

    Plot2D plot2("bw_curve");
    // plot2.AddCurve(bw_curve);
    plot2.AddCurve(bw_MI_curve);
    plot2.AddCurve(bw_MA_curve);
    plot2.AddCurve(bwm_curve);
    plot2.YAxis().SetScaleType(Plot2D::AxisType::LogScale);
    plot2.CurveFlags(1) = Plot2D::ShowLines | Plot2D::ShowPoints;
    plot2.PrintToGnuplotFile("bw_c",Plot2D::GnuplotStyle2);

    return 0;
}

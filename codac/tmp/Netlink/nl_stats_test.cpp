#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "nl_stats.h"



int main(int argc, char *argv[])
{
    int status;
    struct nlsock_link_stats *sk = nl_link_setup(argv[1]);
    if(!sk) {
        std::cerr << "setup error\n";
        return 1;
    }
    struct rtnl_link_stats A;
    struct rtnl_link_stats B;
    struct rtnl_link_stats64 stats64;


    while(1) {
        {
            status = nl_link_read(sk);
            if(!status) std::cerr << "E read \n";
            status = nl_link_getstats(sk, &A);
            if(!status) {
                std::cerr << "get stats error\n";
                exit(1);
            }
        }
        sleep(1);
        {
            status = nl_link_read(sk);
            if(!status) std::cerr << "E read \n";
            status = nl_link_getstats(sk, &B);
            if(!status) {
                std::cerr << "get stats error\n";
                exit(1);
            }
        }

        struct rtnl_link_stats d;
        for (int i=0; i<sizeof(d)/sizeof(__u32); ++i)
            ((__u32*)&d)[i] = ((__u32*)&B)[i] - ((__u32*)&A)[i];
        std::cout << "rate [KB/s]"
                  << " rx:" << (float)d.rx_bytes/1024
                  << " tx:" << (float)d.tx_bytes/1024
                  << "\n";
    }

    return 0;
}

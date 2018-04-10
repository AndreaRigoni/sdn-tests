
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/netlink.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>



// struct ifinfomsg {
//    unsigned char  ifi_family; /* AF_UNSPEC */
//    unsigned short ifi_type;   /* Device type */
//    int            ifi_index;  /* Interface index */
//    unsigned int   ifi_flags;  /* Device flags  */
//    unsigned int   ifi_change; /* change mask */
// };


// * This struct should be in sync with struct rtnl_link_stats64 */
// struct rtnl_link_stats {
//	__u32	rx_packets;		/* total packets received	*/
//	__u32	tx_packets;		/* total packets transmitted	*/
//	__u32	rx_bytes;		/* total bytes received 	*/
//	__u32	tx_bytes;		/* total bytes transmitted	*/
//	__u32	rx_errors;		/* bad packets received		*/
//	__u32	tx_errors;		/* packet transmit problems	*/
//	__u32	rx_dropped;		/* no space in linux buffers	*/
//	__u32	tx_dropped;		/* no space available in linux	*/
//	__u32	multicast;		/* multicast packets received	*/
//	__u32	collisions;

//	/* detailed rx_errors: */
//	__u32	rx_length_errors;
//	__u32	rx_over_errors;		/* receiver ring buff overflow	*/
//	__u32	rx_crc_errors;		/* recved pkt with crc error	*/
//	__u32	rx_frame_errors;	/* recv'd frame alignment error */
//	__u32	rx_fifo_errors;		/* recv'r fifo overrun		*/
//	__u32	rx_missed_errors;	/* receiver missed packet	*/

//	/* detailed tx_errors */
//	__u32	tx_aborted_errors;
//	__u32	tx_carrier_errors;
//	__u32	tx_fifo_errors;
//	__u32	tx_heartbeat_errors;
//	__u32	tx_window_errors;

//	/* for cslip etc */
//	__u32	rx_compressed;
//	__u32	tx_compressed;

//	__u32	rx_nohandler;		/* dropped, no handler found	*/
// };


//        rta_type         value type         description
//        ──────────────────────────────────────────────────────────
//        IFLA_UNSPEC      -                  unspecified.
//        IFLA_ADDRESS     hardware address   interface L2 address
//        IFLA_BROADCAST   hardware address   L2 broadcast address.
//        IFLA_IFNAME      asciiz string      Device name.
//        IFLA_MTU         unsigned int       MTU of the device.
//        IFLA_LINK        int                Link type.
//        IFLA_QDISC       asciiz string      Queueing discipline.
//        IFLA_STATS       see below          Interface Statistics.

#define NL_LINK_BUF_SIZE 16384

struct nlsock_link_stats {
    int fd;
    int dev_id;
    struct {
        struct nlmsghdr n;
        struct ifinfomsg r;
    } req;
    char buf[NL_LINK_BUF_SIZE];
    size_t len;
};

int nl_link_read(struct nlsock_link_stats *nlsk) {
    memset(&nlsk->req, 0, sizeof(nlsk->req));
    nlsk->req.n.nlmsg_len   = NLMSG_LENGTH(sizeof(struct ifaddrmsg));
    nlsk->req.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT;
    nlsk->req.n.nlmsg_type  = RTM_GETLINK;

    int status;

    // SEND DUMP ROOT REQUEST //
    status = send(nlsk->fd, &nlsk->req, nlsk->req.n.nlmsg_len, 0);
    if (status < 0) { return 0; }

    // RECV DUMP ROOT //
    int len = status = 0;
    do {
        len += status;
        status = recv(nlsk->fd, &nlsk->buf[len], sizeof(nlsk->buf)-len, 0);
        if( status > 0 && len+status > NL_LINK_BUF_SIZE ) {
            perror("nl buffer overflow.. \n");
            return 0;
        }
    } while(status >0);

    //    if(status < 0)
    //        printf("an error: %s\n", strerror(errno));

    nlsk->len = len;
    return 1;
}

struct nlsock_link_stats * nl_link_setup(const char *devname) {
    int status;
    struct nlmsghdr  *nlmp;
    struct ifinfomsg *rtmp;
    struct rtattr    *rtatp;
    struct nlsock_link_stats *nlsk = (struct nlsock_link_stats *)malloc(sizeof(struct nlsock_link_stats));
    if(!nlsk) return NULL;

    nlsk->dev_id = -1;
    nlsk->len = 0;

    // OPEN SOCKET //
    nlsk->fd = socket(PF_NETLINK, SOCK_DGRAM | SOCK_NONBLOCK, NETLINK_ROUTE);

    nl_link_read(nlsk);
    int pos = nlsk->len;
    // Loop MESSAGES //
    for(nlmp = (struct nlmsghdr *)nlsk->buf; pos > sizeof(*nlmp);){
        int msg_len = nlmp->nlmsg_len;
        int req_len = msg_len - sizeof(*nlmp);

        if (req_len<0 || msg_len>pos) { break; }
        if (!NLMSG_OK(nlmp, pos)) { break; }

        rtmp = (struct ifinfomsg *)NLMSG_DATA(nlmp);
        rtatp = (struct rtattr *)IFLA_RTA(rtmp);
        int rtattrlen = IFLA_PAYLOAD(nlmp);

        if(rtmp->ifi_index == 0) { break; }
        // printf("Index Of Iface= %d, %d\n",rtmp->ifi_index,rtattrlen);
        int dev_id = rtmp->ifi_index;

        // LOOP ATTRIBUTES //
        for (; RTA_OK(rtatp, rtattrlen); rtatp = RTA_NEXT(rtatp, rtattrlen)) {
            if(rtatp->rta_type == IFLA_IFNAME){
                if(strcmp((char*)RTA_DATA(rtatp),devname) == 0)
                    nlsk->dev_id = dev_id;
            }
        }
        pos -= NLMSG_ALIGN(msg_len);
        nlmp = (struct nlmsghdr*)((char*)nlmp + NLMSG_ALIGN(msg_len));
    }


    if(nlsk->dev_id == -1) {
        close(nlsk->fd);
        perror("device not found..\n");
        return NULL;
    }
    else return nlsk;
}


void nl_link_release(struct nlsock_link_stats *sock)
{
    if(sock) {
        close(sock->dev_id);
        free(sock);
    }
}



int nl_link_getstats(struct nlsock_link_stats *nlsk, struct rtnl_link_stats *stats) {
    int status;
    struct nlmsghdr *nlmp;
    struct ifinfomsg *rtmp;
    struct rtattr *rtatp;

    int pos = nlsk->len;
    // Loop MESSAGES //
    for(nlmp = (struct nlmsghdr *)nlsk->buf; pos > sizeof(*nlmp);){
        int len = nlmp->nlmsg_len;
        int req_len = len - sizeof(*nlmp);
        if (req_len<0 || len>pos) { break; }
        if (!NLMSG_OK(nlmp, pos)) { break; }

        rtmp = (struct ifinfomsg *)NLMSG_DATA(nlmp);
        rtatp = (struct rtattr *)IFLA_RTA(rtmp);
        int rtattrlen = IFLA_PAYLOAD(nlmp);

        // printf("Index stats= %d, %d\n",rtmp->ifi_index,rtattrlen);
        if(rtmp->ifi_index == 0) { break; }
        // LOOP ATTRIBUTES //
        if(rtmp->ifi_index == nlsk->dev_id) {
            for (; RTA_OK(rtatp, rtattrlen); rtatp = RTA_NEXT(rtatp, rtattrlen)) {
                if(rtatp->rta_type == IFLA_STATS){
                    struct rtnl_link_stats *link_stats = (struct rtnl_link_stats *)RTA_DATA(rtatp);
                    memcpy(stats,link_stats,sizeof(struct rtnl_link_stats));
                    return 1;
                }
            }
        }
        pos -= NLMSG_ALIGN(len);
        nlmp = (struct nlmsghdr*)((char*)nlmp + NLMSG_ALIGN(len));
    }
    perror("not found\n");
    return 0;
}


int nl_link_getstats64(struct nlsock_link_stats *nlsk, struct rtnl_link_stats64 *stats) {
    int status;
    struct nlmsghdr *nlmp;
    struct ifinfomsg *rtmp;
    struct rtattr *rtatp;

    int pos = nlsk->len;
    // Loop MESSAGES //
    for(nlmp = (struct nlmsghdr *)nlsk->buf; pos > sizeof(*nlmp);){
        int len = nlmp->nlmsg_len;
        int req_len = len - sizeof(*nlmp);
        if (req_len<0 || len>pos) { break; }
        if (!NLMSG_OK(nlmp, pos)) { break; }

        rtmp = (struct ifinfomsg *)NLMSG_DATA(nlmp);
        rtatp = (struct rtattr *)IFLA_RTA(rtmp);
        int rtattrlen = IFLA_PAYLOAD(nlmp);

        // printf("Index stats= %d, %d\n",rtmp->ifi_index,rtattrlen);
        if(rtmp->ifi_index == 0) { break; }
        // LOOP ATTRIBUTES //
        if(rtmp->ifi_index == nlsk->dev_id) {
            for (; RTA_OK(rtatp, rtattrlen); rtatp = RTA_NEXT(rtatp, rtattrlen)) {
                if(rtatp->rta_type == IFLA_STATS64){
                    struct rtnl_link_stats64 *link_stats = (struct rtnl_link_stats64 *)RTA_DATA(rtatp);
                    memcpy(stats,link_stats,sizeof(struct rtnl_link_stats64));
                    return 1;
                }
            }
        }
        pos -= NLMSG_ALIGN(len);
        nlmp = (struct nlmsghdr*)((char*)nlmp + NLMSG_ALIGN(len));
    }
    perror("not found\n");
    return 0;
}






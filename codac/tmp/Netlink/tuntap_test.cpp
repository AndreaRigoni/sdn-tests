
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include <iostream>

int tun_alloc(char *dev, int flags) {

  struct ifreq ifr;
  int fd, err;
  const char *clonedev = "/dev/net/tun";

  /* Arguments taken by the function:
   *
   * char *dev: the name of an interface (or '\0'). MUST have enough
   *   space to hold the interface name if '\0' is passed
   * int flags: interface flags (eg, IFF_TUN etc.)
   */

   /* open the clone device */
   if( (fd = open(clonedev, O_RDWR)) < 0 ) {
     return fd;
   }

   /* preparation of the struct ifr, of type "struct ifreq" */
   memset(&ifr, 0, sizeof(ifr));

   ifr.ifr_flags = flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

   if (*dev) {
     /* if a device name was specified, put it in the structure; otherwise,
      * the kernel will try to allocate the "next" device of the
      * specified type */
     strncpy(ifr.ifr_name, dev, IFNAMSIZ);
   }

   /* try to create the device */
   if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
     close(fd);
     return err;
   }

  /* if the operation was successful, write back the name of the
   * interface to the variable "dev", so the caller can know
   * it. Note that the caller MUST reserve space in *dev (see calling
   * code below) */
  strcpy(dev, ifr.ifr_name);

  /* this is the special file descriptor that the caller will use to talk
   * with the virtual interface */
  return fd;
}


int main(int argc, char *argv[])
{

    char tun_name[IFNAMSIZ];
    char tap_name[IFNAMSIZ];
    char *a_name;


    strcpy(tun_name, "tun1");
    int tunfd = tun_alloc(tun_name, IFF_TUN);  /* tun interface */

    std::cout << "tunfd: " << tunfd << "\n";

    strcpy(tap_name, "tap44");
    int tapfd = tun_alloc(tap_name, IFF_TAP);  /* tap interface */

    std::cout << "tapfd: " << tapfd << "\n";

    //    a_name = (char*)malloc(IFNAMSIZ);
    //    a_name[0]='\0';
    //    tapfd = tun_alloc(a_name, IFF_TAP);    /* let the kernel pick a name */

    sleep(40);

    return 0;
}

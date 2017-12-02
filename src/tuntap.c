/*
 * Since the rust ioctl bindings don't have all the structures and constants,
 * it's easier to just write the thing in C and link it in.
 */

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>

/**
 * fd ‒ the fd to turn into TUN or TAP.
 * name ‒ the name to use. If empty, kernel will assign something by itself.
 *   Must be buffer with capacity at least 33.
 * mode ‒ 1 = TUN, 2 = TAP.
 */
int tuntap_setup(int fd, unsigned char *name, int mode) {
	struct ifreq ifr;
	memset(&ifr, 0, sizeof ifr);
	switch (mode) {
		case 1:
			ifr.ifr_flags = IFF_TUN;
			break;
		case 2:
			ifr.ifr_flags = IFF_TAP;
			break;
		default:
			assert(0);
	}
	strncpy(ifr.ifr_name, (char *)name, IFNAMSIZ);
	int ioresult = ioctl(fd, TUNSETIFF, &ifr);
	if (ioresult < 0) {
		return ioresult;
	}
	strncpy((char *)name, ifr.ifr_name, IFNAMSIZ < 32 ? IFNAMSIZ : 32);
	name[32] = '\0';
	return 0;
}
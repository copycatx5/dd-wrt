/*
 * sysinit-wasp.c
 *
 * Copyright (C) 2012 Sebastian Gottschall <gottschall@dd-wrt.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id:
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <sys/klog.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>
#include <cymac.h>
#define SIOCGMIIREG	0x8948	/* Read MII PHY register.  */
#define SIOCSMIIREG	0x8949	/* Write MII PHY register.  */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/mii.h>
#include "devices/wireless.c"
#include "devices/ethtools.c"

#ifdef HAVE_DIR869
struct regiondef {
	char *match;
	char *region;
};
static struct regiondef regions[] = {
	{"AU", "AUSTRALIA"},
	{"NA", "UNITED_STATES"},
	{"US", "UNITED_STATES"},
	{"CA", "CANADA"},
	{"LA", "BRAZIL"},
	{"BR", "BRAZIL"},
	{"EU", "GERMANY"},
	{"GB", "UNITED_KINGDOM"},
	{"CN", "CHINA"},
	{"SG", "SINGAPORE"},
	{"KR", "KOREA_REPUBLIC"},
	{"FR", "FRANCE"},
	{"JP", "JAPAN"},
	{"IL", "ISRAEL"},
	{"RU", "RUSSIA"},
	{"TH", "THAILAND"},
	{"MY", "MALASIA"},
	{"IN", "INDIA"},
	{"EG", "EGYPT"},
	{"TW", "TAIWAN"},
	{NULL, NULL}
};

static void setdlinkcountry(void)
{
	char buf[32];
	char c[32];
	char *set = NULL;
	FILE *fp = popen("cat /dev/mtdblock0|grep countrycode=", "r");
	fread(buf, 1, 27, fp);
	pclose(fp);
	buf[27] = 0;
	memset(c, 0, sizeof(c));
	strncpy(c, &buf[12], 2);
	if (!strlen(c))
		return;
	int cnt = 0;
	while (regions[cnt].match) {

		if (!strcmp(regions[cnt].match, c)) {
			set = regions[cnt].region;
			break;
		}
		cnt++;
	}
	if (set) {
		if (!nvram_get("nocountrysel"))
			nvram_set("nocountrysel", "1");
		nvram_set("ath0_regdomain",set);
		nvram_set("ath1_regdomain",set);
	}
}


#endif


void start_sysinit(void)
{
	time_t tm = 0;
	struct ifreq ifr;
	int s;

	if (!nvram_match("disable_watchdog", "1"))
		eval("watchdog");

	/*
	 * Setup console 
	 */

	cprintf("sysinit() klogctl\n");
	klogctl(8, NULL, atoi(nvram_safe_get("console_loglevel")));
	cprintf("sysinit() get router\n");

	/*
	 * network drivers 
	 */
	fprintf(stderr, "load ATH Ethernet Driver\n");
	system("insmod ag71xx || insmod ag7240_mod");

#ifdef HAVE_WDR3500
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "1");
	eval("swconfig", "dev", "eth0", "vlan", "0", "set", "ports", "0 1 2 3 4");
	eval("swconfig", "dev", "eth0", "set", "apply");
#else
#ifdef HAVE_WDR4300
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "1");
	eval("swconfig", "dev", "eth0", "vlan", "1", "set", "ports", "0t 2 3 4 5");
	eval("swconfig", "dev", "eth0", "vlan", "2", "set", "ports", "0t 1");
#elif defined (HAVE_E325N)
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "0");
	eval("swconfig", "dev", "eth0", "vlan", "1", "set", "ports", "0 1 2 3 4");
#elif defined (HAVE_E355AC)
#elif defined (HAVE_WR615N)
#elif defined (HAVE_E380AC)
#elif defined (HAVE_WR650AC)
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "0");
	eval("swconfig", "dev", "eth0", "vlan", "1", "set", "ports", "0 2 3 4 5");
	eval("swconfig", "dev", "eth0", "vlan", "2", "set", "ports", "1 6");
#elif defined (HAVE_JWAP606)
	// nothing
#elif defined (HAVE_DAP3662)
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "0");
	eval("swconfig", "dev", "eth0", "vlan", "1", "set", "ports", "0 2 3 4 5");
	eval("swconfig", "dev", "eth0", "vlan", "2", "set", "ports", "1 6");
#elif defined (HAVE_DIR862)
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "0");
	eval("swconfig", "dev", "eth0", "vlan", "1", "set", "ports", "0 1 2 3 4");
	eval("swconfig", "dev", "eth0", "vlan", "2", "set", "ports", "5 6");
#elif defined (HAVE_MMS344)
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "1");
	eval("swconfig", "dev", "eth0", "vlan", "1", "set", "ports", "0t 2");
	eval("swconfig", "dev", "eth0", "vlan", "2", "set", "ports", "0t 3");
#elif defined (HAVE_ARCHERC7)
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "0");
	eval("swconfig", "dev", "eth0", "vlan", "1", "set", "ports", "0 2 3 4 5");
	eval("swconfig", "dev", "eth0", "vlan", "2", "set", "ports", "1 6");
#elif defined (HAVE_WZR450HP2) || defined(HAVE_WR1043V2)
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "0");
	eval("swconfig", "dev", "eth0", "vlan", "1", "set", "ports", "0 1 2 3 4");
	eval("swconfig", "dev", "eth0", "vlan", "2", "set", "ports", "5 6");
#else
	eval("swconfig", "dev", "eth0", "set", "reset", "1");
	eval("swconfig", "dev", "eth0", "set", "enable_vlan", "1");
	eval("swconfig", "dev", "eth0", "vlan", "1", "set", "ports", "0t 1 2 3 4");
	eval("swconfig", "dev", "eth0", "vlan", "2", "set", "ports", "0t 5");
#endif
#endif
	eval("swconfig", "dev", "eth0", "set", "apply");
#ifdef HAVE_WNDR3700V4
	FILE *fp = fopen("/dev/mtdblock/5", "rb");
	if (fp) {
		unsigned char buf2[256];
		fread(buf2, 256, 1, fp);
		fclose(fp);
		char mac[32];
		unsigned int copy[256];
		int i;
		for (i = 0; i < 256; i++)
			copy[i] = buf2[i] & 0xff;
		sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", copy[0], copy[1], copy[2], copy[3], copy[4], copy[5]);
		fprintf(stderr, "configure eth0 to %s\n", mac);
		eval("ifconfig", "eth0", "hw", "ether", mac);
	}
#endif
#if defined(HAVE_MMS344) && !defined(HAVE_DAP3662)
	FILE *fp = fopen("/dev/mtdblock/6", "rb");
	if (fp) {
		unsigned char buf2[256];
		fseek(fp, 0x2e010, SEEK_SET);
		fread(buf2, 256, 1, fp);
		fclose(fp);
		if ((!memcmp(buf2, "\xff\xff\xff\xff\xff\xff", 6)
		     || !memcmp(buf2, "\x00\x00\x00\x00\x00\x00", 6)))
			goto out;
		char mac[32];
		unsigned int copy[256];
		int i;
		for (i = 0; i < 256; i++)
			copy[i] = buf2[i] & 0xff;
		sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", copy[0], copy[1], copy[2], copy[3], copy[4], copy[5]);
		fprintf(stderr, "configure eth0 to %s\n", mac);
		eval("ifconfig", "eth0", "hw", "ether", mac);
	      out:;
	}
#endif

#if !defined(HAVE_WR650AC) && !defined(HAVE_E355AC) && !defined(HAVE_E325N) && !defined(HAVE_E380AC) && !defined(HAVE_WR615N)
#ifndef HAVE_JWAP606
	eval("ifconfig", "eth0", "up");
#if defined(HAVE_MMS344) && !defined(HAVE_DIR862)
	eval("vconfig", "set_name_type", "VLAN_PLUS_VID_NO_PAD");
	eval("vconfig", "add", "eth0", "1");
	eval("vconfig", "add", "eth0", "2");
#elif defined(HAVE_WZR450HP2) || defined(HAVE_WDR3500)
	eval("ifconfig", "eth1", "up");
#else
	eval("vconfig", "set_name_type", "VLAN_PLUS_VID_NO_PAD");
	eval("vconfig", "add", "eth0", "1");
	eval("vconfig", "add", "eth0", "2");
#endif
#endif
#endif
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW))) {
		char eabuf[32];

		strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
		ioctl(s, SIOCGIFHWADDR, &ifr);
		nvram_set("et0macaddr", ether_etoa((char *)ifr.ifr_hwaddr.sa_data, eabuf));
		nvram_set("et0macaddr_safe", ether_etoa((char *)ifr.ifr_hwaddr.sa_data, eabuf));
		close(s);
	}
#if defined(HAVE_E355AC)
	FILE *fp = fopen("/dev/mtdblock/0", "rb");
	FILE *out = fopen("/tmp/archerc7-board.bin", "wb");
	if (fp) {
		fseek(fp, 0x10000 + 0x5000, SEEK_SET);
		int i;
		for (i = 0; i < 2116; i++)
			putc(getc(fp), out);
		fclose(fp);
		fclose(out);
		eval("rm", "-f", "/tmp/ath10k-board.bin");
		eval("ln", "-s", "/tmp/archerc7-board.bin", "/tmp/ath10k-board.bin");
	}
#elif defined(HAVE_E380AC)
	FILE *fp = fopen("/dev/mtdblock/0", "rb");
	FILE *out = fopen("/tmp/archerc7-board.bin", "wb");
	if (fp) {
		fseek(fp, 0x20000 + 0x5000, SEEK_SET);
		int i;
		for (i = 0; i < 2116; i++)
			putc(getc(fp), out);
		fclose(fp);
		fclose(out);
		eval("rm", "-f", "/tmp/ath10k-board.bin");
		eval("ln", "-s", "/tmp/archerc7-board.bin", "/tmp/ath10k-board.bin");
	}
#elif defined(HAVE_WR650AC)
	FILE *fp = fopen("/dev/mtdblock/0", "rb");
	FILE *out = fopen("/tmp/archerc7-board.bin", "wb");
	if (fp) {
		fseek(fp, 0x20000 + 0x5000, SEEK_SET);
		int i;
		for (i = 0; i < 2116; i++)
			putc(getc(fp), out);
		fclose(fp);
		fclose(out);
		eval("rm", "-f", "/tmp/ath10k-board.bin");
		eval("ln", "-s", "/tmp/archerc7-board.bin", "/tmp/ath10k-board.bin");
	}
#elif defined(HAVE_ARCHERC7) || defined(HAVE_DIR859) || defined(HAVE_DAP3662)
	FILE *fp = fopen("/dev/mtdblock/5", "rb");
	FILE *out = fopen("/tmp/archerc7-board.bin", "wb");
	if (fp) {
		fseek(fp, 20480, SEEK_SET);
		int i;
		for (i = 0; i < 6; i++)
			putc(getc(fp), out);
		char *mac = "\x00\x01\x02\x03\x04\x05";
		if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW))) {
			char eabuf[32];

			strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
			ioctl(s, SIOCGIFHWADDR, &ifr);
			mac = (char *)ifr.ifr_hwaddr.sa_data;
			close(s);
		}
		for (i = 0; i < 6; i++)
			putc(mac[i], out);
		fseek(fp, 20492, SEEK_SET);
		for (i = 0; i < 2104; i++)
			putc(getc(fp), out);
		fclose(fp);
		fclose(out);
		eval("rm", "-f", "/tmp/ath10k-board.bin");
		eval("ln", "-s", "/tmp/archerc7-board.bin", "/tmp/ath10k-board.bin");
	}
#elif defined(HAVE_DIR862)
	fp = fopen("/lib/ath10k/board.bin", "rb");
	FILE *out = fopen("/tmp/archerc7-board.bin", "wb");
	if (fp) {
		int i;
		for (i = 0; i < 6; i++)
			putc(getc(fp), out);
		char *mac = "\x00\x01\x02\x03\x04\x05";
		if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW))) {
			char eabuf[32];

			strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
			ioctl(s, SIOCGIFHWADDR, &ifr);
			mac = (char *)ifr.ifr_hwaddr.sa_data;
			close(s);
		}
		for (i = 0; i < 6; i++)
			putc(mac[i], out);
		fseek(fp, 12, SEEK_SET);
		for (i = 0; i < 2104; i++)
			putc(getc(fp), out);
		fclose(fp);
		fclose(out);
		eval("rm", "-f", "/tmp/ath10k-board.bin");
		eval("ln", "-s", "/tmp/archerc7-board.bin", "/tmp/ath10k-board.bin");
	}
#endif

	detect_wireless_devices();

#ifdef HAVE_WNDR3700V4
	setWirelessLed(0, 11);
	setWirelessLed(1, 14);
#elif  HAVE_JWAP606
//      setWirelessLed(0, 14);
	setWirelessLed(1, 14);
#elif  HAVE_WR1043V2
	setWirelessLed(0, 12);
#ifdef HAVE_WDR4900V2
	setWirelessLed(1, 17);
#endif
#ifdef HAVE_ARCHERC7
	setWirelessLed(1, 17);
#endif
#elif  HAVE_WZR450HP2
	setWirelessLed(0, 18);
#elif  HAVE_WR615N
	setWirelessLed(0, 12);
#elif  HAVE_E325N
	setWirelessLed(0, 0);
#elif  HAVE_E380AC
	setWirelessLed(0, 0);
	setWirelessLed(1, 2);
#elif  HAVE_E355AC
	setWirelessLed(0, 0);
	setWirelessLed(1, 3);
#elif  HAVE_WR650AC
	setWirelessLed(0, 13);
	setWirelessLed(1, 2);
#elif  HAVE_DIR869
	setdlinkcountry();
#elif  HAVE_DIR859
	setWirelessLed(0, 19);
//      setWirelessLed(1, 32);
#elif  HAVE_DIR825C1
	setWirelessLed(0, 13);
	setWirelessLed(1, 32);
#else
	setWirelessLed(0, 0);
#endif
	/*
	 * Set a sane date 
	 */
	stime(&tm);
	nvram_set("wl0_ifname", "ath0");

	return;
	cprintf("done\n");
}

int check_cfe_nv(void)
{
	nvram_set("portprio_support", "0");
	return 0;
}

int check_pmon_nv(void)
{
	return 0;
}

void start_overclocking(void)
{
}

char *enable_dtag_vlan(int enable)
{
	return "eth0";
}

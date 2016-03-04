/*
 * tor.c
 *
 * Copyright (C) 2013 Sebastian Gottschall <gottschall@dd-wrt.com>
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

#ifdef HAVE_TOR

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <bcmnvram.h>
#include <shutils.h>
#include "snmp.h"
#include <signal.h>
#include <utils.h>
#include <services.h>

void stop_tor(void)
{
	stop_process("tor", "Stopping TOR");
}

void start_tor(void)
{
	int ret;
	pid_t pid;
	char *tor_argv[] = { "tor", "--defaults-torrc", "/tmp/torrc",
		NULL
	};

	stop_tor();

	if (nvram_match("tor_enable", "0"))
		return;

	mkdir("/tmp/tor", 0700);
	FILE *fp = fopen("/tmp/torrc", "wb");
	fprintf(fp, "Log notice syslog\n");
	if (nvram_match("tor_relayonly", "1"))
		fprintf(fp, "SocksPort 0\n");
	else {
		fprintf(fp, "SocksPort 9050\n");
		fprintf(fp, "SocksPort %s:9050\n", nvram_safe_get("lan_ipaddr"));
	}
	fprintf(fp, "RunAsDaemon 1\n");
	fprintf(fp, "Address %s\n", nvram_invmatch("tor_address", "") ? nvram_safe_get("tor_address") : get_wan_ipaddr());
	if (nvram_invmatch("tor_id", ""))
		fprintf(fp, "Nickname %s\n", nvram_safe_get("tor_id"));
	if (nvram_invmatch("tor_bwrate", ""))
		fprintf(fp, "RelayBandwidthRate %d\n", atoi(nvram_safe_get("tor_bwrate")) * 1024);
	if (nvram_invmatch("tor_bwburst", ""))
		fprintf(fp, "RelayBandwidthBurst %d\n", atoi(nvram_safe_get("tor_bwburst")) * 1024);

//      fprintf(fp, "ControlPort 9051\n");
	if (nvram_match("tor_relay", "1")) {
		eval("iptables", "-I", "INPUT", "-p", "tcp", "-i", get_wan_face(), "--dport", "9001", "-j", "ACCEPT");
		fprintf(fp, "ORPort 9001\n");
	}
	if (nvram_match("tor_dir", "1")) {
		eval("iptables", "-I", "INPUT", "-p", "tcp", "-i", get_wan_face(), "--dport", "9030", "-j", "ACCEPT");
		fprintf(fp, "DirPort 9030\n");
	}
	if (nvram_match("tor_bridge", "1"))
		fprintf(fp, "BridgeRelay 1\n");
	if (nvram_match("tor_transparent", "1")) {
		fprintf(fp, "VirtualAddrNetwork 10.192.0.0/10\n");
		fprintf(fp, "AutomapHostsOnResolve 1\n");
		fprintf(fp, "TransPort 9040\n");
		fprintf(fp, "DNSPort 53\n");
		fprintf(fp, "TransListenAddress %s\n", nvram_safe_get("lan_ipaddr"));
		fprintf(fp, "DNSListenAddress %s\n", nvram_safe_get("lan_ipaddr"));
		sysprintf("iptables -t nat -A PREROUTING -i br0 -p udp --dport 53 -j DNAT --to %s:53", nvram_safe_get("lan_ipaddr"));
		sysprintf("iptables -t nat -A PREROUTING -i br0 -p tcp --syn -j DNAT --to %s:9040", nvram_safe_get("lan_ipaddr"));

	}
#ifdef HAVE_X86
	eval("mkdir", "-p", "/tmp/tor");
	fprintf(fp, "DataDirectory /tmp/tor\n");
#else
	if (nvram_match("enable_jffs2", "1")
	    && nvram_match("jffs_mounted", "1")
	    && nvram_match("sys_enable_jffs2", "1")) {
		eval("mkdir", "-p", "/jffs/tor");
		fprintf(fp, "DataDirectory /jffs/tor\n");
	} else {
		fprintf(fp, "DataDirectory /tmp/tor\n");
	}
#endif

	fclose(fp);
	ret = _evalpid(tor_argv, NULL, 0, &pid);
}

#endif				/* HAVE_WOL */

/*- 
 * Copyright (c) 2010 iXsystems, Inc.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 *
 * $Id: netif.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>

#include "txt-sysinstall.h"

#define MAXNETIFS	8
#define MODULE		"Network interface setup"

static int
netif_fire(dialogMenuItem *self)
{
	int dhcp, ipv6;
	char command[64];

	screen_clear(MODULE " >> DHCP");
	dhcp = dialog_yesno(MODULE,
	    "Do you want to configure the interface automatically? "
	    "(i.e. DHCP)", 6, 60);
	if (!dhcp) {
		int res;
restart_dhcp:
		screen_clear(MODULE " >> DHCP");
		dialog_busy(5, 60, "Performing DHCP requests. Please wait.");
		dialog_update();
		snprintf(command, sizeof(command),
		    SCRIPTS_PATH "netif-dhcp.sh %s", self->prompt);
		res = dialog_prgbox(MODULE, command,
		    10, 60, TRUE, 0);
		if (res) {
			if (!dialog_yesno(MODULE, "DHCP configuration failed. "
			    "Do you want to try again?", 6, 60))
				goto restart_dhcp;
		}
	}
	screen_clear(MODULE " >> IPv6");
	ipv6 = dialog_yesno("Network interface setup",
	    "Do you wish to configure IPv6 address via router advertisements? "
	    "(i.e. rtsol/rtadvd)", 6, 60);
	if (!ipv6) {
		screen_clear(MODULE " >> IPv6");
		dialog_busy(5, 60, "Performing router solicitation. "
		    "Please wait.");
		dialog_update();
		snprintf(command, sizeof(command),
		    SCRIPTS_PATH "netif-ipv6.sh %s", self->prompt);
		dialog_prgbox("Network interface setup", command,
		    10, 60, TRUE, 1);
	}

	return 0;
}

int
dialog_netif(void)
{
	char *token;
	char *buf;
	dialogMenuItem *menus;
	int status;
	int i = 0;

	buf = safe_malloc(BUFSZ+1);
	status = run_pcsysinstall(buf, BUFSZ, "detect-nics", NULL);
	menus = safe_malloc(sizeof(*menus)*MAXNETIFS);

	while ((token = strsep(&buf, "\n")) != NULL && safe_strlen(token) > 0 &&
	    i < MAXNETIFS) {
		char *net;

		menus[i].prompt = strsep(&token, ":");
		net = strsep(&token, ":");
		/* trim string and remove '<' and '>' */
		net += 2;
		net[safe_strlen(net)-1] = 0;
		menus[i].title = net;
		menus[i].fire = netif_fire;
		i++;
	}
	screen_clear(MODULE);
	dialog_menu("Network interface setup",
	    "Please select the network interface you want to configure:",
	    7+i, 70, i, -i, menus, NULL, NULL, NULL);
	free(buf);
	free(menus);

	return (0);
}

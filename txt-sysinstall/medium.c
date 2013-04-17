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
 * $Id: medium.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>
#include <ctype.h>

#include "txt-sysinstall.h"

#define MODULE		"Medium selection"


static int
medium_fire(dialogMenuItem *self)
{
	//char ftpSite[128] = "http://darkstar.ist.utl.pt/pcbsd/current/i386/netinstall/";
	installMedium(self->data);

	if (!strcmp(self->data, "ftp")) {
		dialog_ftp();
/*
		screen_clear(MODULE);
		dialog_inputbox("FTP/HTTP mirror", "Please type the "
		    "URL where to fetch the archives from:",
		    8, 70, ftpSite);
		ftpPath(ftpSite);
*/
	}

	return (0);
}


int
dialog_medium(void)
{
	dialogMenuItem menus[6];
	int i;

	i = 0;
	bzero(menus, sizeof(menus));

	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);
	DMENUFD(&menus[i++], "DVD", "Search the install archives on a DVD",
		medium_fire, "dvd");
	DMENUFD(&menus[i++], "USB", "Search the install archives on a USB drive",
	    medium_fire, "usb");
	DMENUFD(&menus[i++], "FTP/HTTP", "Fetch and install the archives from a server",
	    medium_fire, "ftp");
	DMENUFD(&menus[i++], "rsync", "Pull the system data from a ssh + rsync server",
	    medium_fire, "rsync");

	screen_clear(MODULE);
	dialog_menu(MODULE, "Please select the install medium:",
	    7+i-2, 70, i-2, -i+2, menus+2, "", NULL, NULL);

	return (0);
}

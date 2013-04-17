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
 * $Id: mainmenu.c 7782 2010-10-12 22:32:13Z johnh $
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <dialog.h>

#include "txt-sysinstall.h"

#define MODULE "Main Menu"
#define	MAXLABELS 8

enum {
	DO_DISKSEL = 1,
	DO_NETIF = 2
};


static int
next_fire(dialogMenuItem *self)
{
	*((int *)self->data) = DO_DISKSEL;
	return (0);
}

static int
exit_fire(dialogMenuItem *self)
{
	return (0);
}

static int
install_fire(dialogMenuItem *self)
{
	installMode("fresh");
	*((int *)self->data) = DO_DISKSEL;

	return (0);
}

static int
upgrade_fire(dialogMenuItem *self)
{
	installMode("upgrade");
	*((int *)self->data) = DO_DISKSEL;

	return (0);
}

static int
netif_fire(dialogMenuItem *self)
{
	*((int *)self->data) = DO_NETIF;
	return (0);
}

static int
shell_fire(dialogMenuItem *self)
{
	end_dialog();
	if (fork() == 0) {
		execl("/bin/tcsh", "tcsh", NULL);
		_exit(1);
	} else {
		wait(NULL);
	}

	return (0);
}

int
dialog_sysinstall(void)
{
	int i;
	int res;
	int dialog;
	dialogMenuItem *menus;

	i = 0;
	dialog = 0;
	menus = safe_malloc(sizeof(*menus)*MAXLABELS+2);
	DMENUFD(&menus[i++], "Next", NULL, next_fire, &dialog);
	DMENUF(&menus[i++], "Exit", NULL, exit_fire);
	DMENUFD(&menus[i++], "Install", "Install PC-BSD or FreeBSD",
		install_fire, &dialog);
	DMENUFD(&menus[i++], "Upgrade", "Upgrade PC-BSD or FreeBSD",
		upgrade_fire, &dialog);
	DMENUFD(&menus[i++], "Configure Network", "Setup network interfaces",
		netif_fire, &dialog);
	DMENUFD(&menus[i++], "Shell", "Run tcsh", shell_fire, &dialog);

	screen_clear(MODULE);
	res = dialog_menu("Welcome to "OSNAME" install",
	    "Welcome to the "OSNAME" installer.\nPlease select your "
	    "choice from the menu below.",
	    12, 60, 4, -4, menus+2, "", 
	    NULL, NULL);

	free(menus);

	switch(dialog) {
		case DO_DISKSEL:
			dialog_disksel();
			break;

		case DO_NETIF:
			dialog_netif();
			break;
	}

	return (res);
}

void
sysinstall(void)
{
	dialog_sysinstall();
	dialog_system();
	dialog_partsel();
	dialog_label();
	dialog_useradd();
	dialog_rootpass();
	dialog_tzone();
	dialog_medium();
	dialog_components();
	dialog_packages();

	save_config();
	do_install();
}

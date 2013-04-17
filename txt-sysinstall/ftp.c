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
 * $Id: ftp.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>
#include <ctype.h>

#include "txt-sysinstall.h"

#define MODULE		"FTP selection"
#define	MAXSERVERS	512

static int
ftp_fire(dialogMenuItem *self)
{
	char *buf;
	char *token;
	char *arch;
	char *branch;
	char ftpSite[1024];

	buf = safe_malloc(BUFSZ+1);
	run_pcsysinstall(buf, BUFSZ, "list-config", NULL);

	arch = branch = NULL;
	while ((token = strsep(&buf, "\n")) != NULL && safe_strlen(token) > 0) {
		char *key, *val;

		key = strsep(&token, "=");
		val = strsep(&token, "=");

		if (!strcmp(key, "arch"))
			arch = val;
		else if (!strcmp(key, "branch"))
			branch = val;
	}

	/* XXX This could probably be done better XXX */
	bzero(&ftpSite, sizeof(ftpSite));
	snprintf(ftpSite, sizeof(ftpSite),
		"ftp://%s/pub/FreeBSD/releases/%s/%s/",
		self->title, arch, branch);
	ftpPath(ftpSite);

	free(buf);
	return (0);
}

int
dialog_ftp(void)
{
	char *buf;
	char *token;
	char *desc;
	char *server;
	dialogMenuItem *menus;
	int status;
	int i;

	screen_clear(MODULE);
	dialog_busy(5, 60, "Getting FTP mirrors...");

	buf = safe_malloc(BUFSZ+1);
	status = run_pcsysinstall(buf, BUFSZ, "list-mirrors", NULL);
	menus = safe_malloc(sizeof(*menus)*MAXSERVERS+4);

	i = 0;
	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);

	while ((token = strsep(&buf, "\n")) != NULL && safe_strlen(token) > 0 &&
		i < MAXSERVERS-1) {
		desc = strsep(&token, ":");
		server = strsep(&token, ":");
		server += 1;

		DMENUF(&menus[i++], desc, server, ftp_fire);
	}

	screen_clear(MODULE);
	dialog_menu(MODULE, "Please pick a FTP mirror",
		17, 60, 10, -i+2, menus+2, "", NULL, NULL);

	free(menus);
	free(buf);
	return (0);
}

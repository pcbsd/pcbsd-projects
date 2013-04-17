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
 * $Id: tzone.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>

#include "txt-sysinstall.h"

#define MAXTZONES	600
#define MODULE		"Timezone selection"

static int
tzone_fire(dialogMenuItem *self)
{
	char tzone[64];
	char *place;
	
	/* remove the comment, if there is one */
	place = strsep(&self->title, ":");
	if (!place)
		place = self->title;

	snprintf(tzone, sizeof(tzone), "%s/%s", self->prompt, place);
	timeZone(tzone);

	return (0);
}

int
dialog_tzone(void)
{
	char *token;
	char *buf;
	dialogMenuItem *menus;
	int status;
	int i = 0;
	int ntp;

	screen_clear(MODULE);
	dialog_busy(5, 60, "Reading the list of timezones. Please wait.");

	buf = safe_malloc(BUFSZ+1);
	status = run_pcsysinstall(buf, BUFSZ, "list-tzones", NULL);
	menus = safe_malloc(sizeof(*menus)*MAXTZONES);

	while ((token = strsep(&buf, "\n")) != NULL && safe_strlen(token) > 0 &&
	    i < MAXTZONES) {

		DMENUF(&menus[i++], strsep(&token, "/"), token, tzone_fire);
	}

	screen_clear(MODULE);
	dialog_menu("Timezone selection",
	    "Please select the time zone you are in:",
	    20, 70, 13, -i, menus, NULL, NULL, NULL);

	free(buf);
	free(menus);

	ntp = dialog_yesno("Network Time Protocol",
	    "Do you want to enable NTP (automatic time synchronization) ?",
	    5, 70);
	if (ntp == FALSE)
		enableNTP("yes");

	return (0);
}

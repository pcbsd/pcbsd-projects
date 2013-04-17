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
 * $Id: disksel.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <unistd.h>
#include <search.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>

#include "txt-sysinstall.h"

#define MODULE "Disk Selection"


static int
disk_fire(dialogMenuItem *self)
{
	struct disk_info *di;
	char buf[32];
	int loop;
	int n;

	di = (struct disk_info *)self->data;
	di->used = 1;

	n = 0;
	loop = 1;	
	while (loop == 1) {
		ENTRY item, *find;

		bzero(&buf, sizeof(buf));
		snprintf(buf, sizeof(buf), "disk%d", n);

		item.key = safe_strdup(buf); 
		item.data = NULL;

		find = hsearch(item, FIND);
		if (find == NULL) {
			ENTRY new;

			new.key = safe_strdup(buf);
			new.data = di;
			hsearch(new, ENTER);

			loop = 0;
		}

		free(item.key);
		n++;
	}

	appendconfig(buf, self->prompt);
	set_current_disk(di);

	return (0);
}

int
dialog_disksel(void)
{
	dialogMenuItem *menus;
	struct disk_list *disks;
	struct disk_info *di;
	int i;

	i = 0;
	disks = get_disk_list();
	menus = safe_malloc(sizeof(*menus) * (disks->ndisks + 2));
	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);

	STAILQ_FOREACH(di, &disks->list, entries) {
		DMENUFD(&menus[i++], di->disk,
			di->desc == NULL ? "" : di->desc, disk_fire, di);
	}

	screen_clear(MODULE);
	dialog_menu(MODULE,
	    "Please select the disk on which you want to install "
	    "or upgrade\nPC-BSD or FreeBSD:",
	    7+i-2, 70, i-2, -i+2, menus+2, "", NULL, NULL);

	free(menus);
	return (0);
}

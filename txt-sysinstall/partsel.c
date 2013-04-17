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
 * $Id: partsel.c 7784 2010-10-13 01:13:55Z johnh $
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>
#include <libutil.h>

#include "txt-sysinstall.h"

#define MODULE		"Partition Manager"
#define MAXPARTS	64

enum {
	USEALL  = 1,
	USENEW  = 2
};

struct mirror_disk {
	char disk[128];
	char balance[128];
};

struct partscheme {
	char scheme[4];
};

static int
part_fire(dialogMenuItem *self)
{
	switch (self->aux) {
	case USEALL:
		partition("all");
		break;

	case USENEW:
		partition("free");
		break;

	default:
		partition(self->prompt);
	}

	return (0);
}

static int
gmirror_disk_fire(dialogMenuItem *self)
{
	struct mirror_disk *md = self->data;

	strcpy(md->disk, self->prompt);
	return (DITEM_SUCCESS);
}

static int
gmirror_disk_checked(dialogMenuItem *self)
{
	struct mirror_disk *md = self->data;

	if (strcmp(md->disk, self->prompt) == 0)
		return (TRUE);

	return (FALSE);
}

static int
gmirror_balance_fire(dialogMenuItem *self)
{
	struct mirror_disk *md = self->data;

	strcpy(md->balance, self->prompt);
	return (DITEM_SUCCESS);
}

static int
gmirror_balance_checked(dialogMenuItem *self)
{
	struct mirror_disk *md = self->data;

	if (strcmp(md->balance, self->prompt) == 0)
		return (TRUE);

	return (FALSE);
}

static struct mirror_disk *
configure_gmirror(void)
{
	int i;
	dialogMenuItem *menus;
	struct disk_list *disks;
	struct disk_info *temp;
	struct mirror_disk *md;

	screen_clear("MIRROR");
	disks = get_disk_list();
	menus = safe_malloc(sizeof(*menus) * (disks->ndisks + 2));

	md = safe_malloc(sizeof(*md));

	i = 0;
	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);
	STAILQ_FOREACH(temp, &disks->list, entries) {
		if (temp->used == 0) {
			RDMENUFCD(&menus[i++], temp->disk,  "", &gmirror_disk_fire,
				&gmirror_disk_checked, md);
		}
	}

	screen_clear("MIRROR");
	dialog_radiolist("MIRROR", "Disk Mirror",
		7+i-2, 60, i-2, -i+2, menus+2, NULL);

	if (notnull(md->disk)) {
		free(menus);

		i = 0;
		menus = safe_malloc(sizeof(*menus) * (disks->ndisks + 2));

		strcpy(md->balance, "round-robin");
		DMENUF(&menus[i++], "Next", NULL, NULL);
		DMENUF(&menus[i++], "Back", NULL, NULL);
		RDMENUFCD(&menus[i++], "load",  "", &gmirror_balance_fire,
			&gmirror_balance_checked, md);
		RDMENUFCD(&menus[i++], "prefer",  "", &gmirror_balance_fire,
			&gmirror_balance_checked, md);
		RDMENUFCD(&menus[i++], "round-robin",  "", &gmirror_balance_fire,
			&gmirror_balance_checked, md);
		RDMENUFCD(&menus[i++], "split",  "", &gmirror_balance_fire,
			&gmirror_balance_checked, md);

		screen_clear("MIRROR");
		dialog_radiolist("MIRROR", "Mirror Balance",
			7+i-2, 60, i-2, -i+2, menus+2, NULL);

	} else {
		free(md);
		md = NULL;
	}

	free(menus);
	return (md);
}

void
dialog_bootloader(void)
{
	int bootloader;

	screen_clear("Bootloader");
	bootloader = dialog_yesno("Bootloader setup",
	    "Do you want to install a boot loader ?", 5, 60);
	if (bootloader == FALSE)
		bootManager("bsd");
	else
		bootManager("none");
}

void
dialog_mirror(void)
{
	screen_clear("MIRROR");
	if (dialog_noyes("MIRROR", "Configure Mirror?", 5, 10) == 0) {
		struct mirror_disk *md = configure_gmirror();
		if (md != NULL) {
			struct disk_info *gdi;

			mirror(md->disk);
			mirrorbal(md->balance);

			gdi = get_disk_info_by_disk(md->disk);
			gdi->used = 1;
			free(md);
		}
	}
}

static int
partscheme_fire(dialogMenuItem *self)
{
	struct partscheme *ps = self->data;

	strcpy(ps->scheme, self->prompt);
	return (DITEM_SUCCESS);
}

static int
partscheme_checked(dialogMenuItem *self)
{
	struct partscheme *ps = self->data;

	if (strcmp(ps->scheme, self->prompt) == 0)
		return (TRUE);

	return (FALSE);
}

void
dialog_partscheme(void)
{
	dialogMenuItem *menus;
	struct partscheme *ps;
	int i;

	ps = safe_malloc(sizeof(*ps));
	menus = safe_malloc(sizeof(*menus) * 4);

	i = 0;
	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);
	RDMENUFCD(&menus[i++], "GPT", "", &partscheme_fire,
		&partscheme_checked, ps);
	RDMENUFCD(&menus[i++], "MBR", "", &partscheme_fire,
		&partscheme_checked, ps);

	screen_clear("Partscheme");
	dialog_radiolist("Partscheme", "Partscheme",
		7+i-2, 60, i-2, -i+2, menus+2, NULL);

	partscheme(ps->scheme);

	free(menus);
	free(ps);
}

int
dialog_partsel(void)
{
	int i;
	int status;
	char *buf;
	char *token;
	char *format;
	char prompt[1024];
	char strfreemb[6];
	dialogMenuItem *menus;
	struct disk_info *di;

	screen_clear(MODULE);
	dialog_busy(5, 60, "Reading partition list...");

	format = NULL;
	di = get_current_disk();

	buf = safe_malloc(BUFSZ+1);
	status = run_pcsysinstall(buf, BUFSZ, "disk-part", di->disk);
	if (!strncmp(buf, "Error", 5)) {
		free(buf);
		return (-1);
	}

	menus = safe_malloc(sizeof(*menus)*MAXPARTS+4);

	menus[0].prompt = "Next";
	menus[1].prompt = "Back";
	i = 2;
	while ((token = strsep(&buf, "\n")) != NULL && safe_strlen(token) > 0 &&
	    i < MAXPARTS-1) {
		char *part, *type, *value;

		type = strsep(&token, ":");
		part = strsep(&type, "-");
		type = strsep(&type, "-");
		value = strsep(&token, ":");
		value++;
		if (!strcmp(type, "format")) {
			format = value;
		} else if (!strcmp(type, "freemb")) {
			humanize_number(strfreemb, sizeof(strfreemb)-1,
			    atol(value) * 1024 * 1024,
			    "", HN_AUTOSCALE, HN_B | HN_NOSPACE | HN_DECIMAL);
		} else if (!strcmp(type, "label")) {
			menus[i].prompt = part;
			menus[i].title = value;
			menus[i].fire  = part_fire;
			i++;
		}
	}
	
	DMENU(&menus[i++], __DECONST(char *, di->disk), "Use entire disk",
		NULL, NULL, part_fire, NULL, USEALL);
	DMENU(&menus[i++], __DECONST(char *, di->disk),
		"Create a new partition using free space",
		NULL, NULL, part_fire, NULL, USENEW);

	screen_clear(MODULE);
	snprintf(prompt, sizeof(prompt), "The disk %s <%s>\n"
	    "is formatted using %s and has %s free\n\n"
	    "Please select the partition you want to use for the installation:",
	    di->disk, di->desc, format, strfreemb);
	dialog_menu(MODULE, prompt,
	    10+i-2, 70, i-2, -i+2, menus+2, "", NULL, NULL);
	free(menus);
	free(buf);

	dialog_bootloader();
	dialog_mirror();
	dialog_partscheme();

	commitDiskPart();
	return (0);
}

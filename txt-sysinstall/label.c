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
 * $Id: label.c 7783 2010-10-13 01:12:11Z johnh $
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>
#include <libutil.h>
#include <limits.h>

#include "txt-sysinstall.h"

#define MODULE "Disk label setup"
#define MAXLABELS 64
#define	MAXTYPES 10	
#define	MAXZPOOLS 14

#define	FS_UFS					0x01	
#define	FS_ZFS					0x02
#define	FS_SWAP					0x03

#define	FS_FLAGS_GJOURNAL		0x000001
#define	FS_FLAGS_SOFTUPDATES	0x000002
#define	FS_FLAGS_ENCRYPT		0x000004

#define	ZFS_POOL_BASIC			0x000010
#define	ZFS_POOL_MIRROR			0x000020
#define	ZFS_POOL_RAIDZ			0x000040
#define	ZFS_POOL_RAIDZ1			ZFS_POOL_RAIDZ	
#define	ZFS_POOL_RAIDZ2			0x000080
#define	ZFS_POOL_DISK			0x000100
#define	ZFS_POOL_FILE			0x000200
#define	ZFS_POOL_SPARE			0x000400
#define	ZFS_POOL_LOG			0x000800
#define	ZFS_POOL_CACHE			0x001000

#define	FS_ZERO(x)				(x = 0)
#define	GET_FS_TYPE(type) 		(type & 0x000000ff)
#define	GET_FS_FLAGS(type)		((type & 0xffffff00) >> 24)
#define	SET_FS_TYPE(x, type)	(x = (x | type))	
#define CLEAR_FS_TYPE(x, type)	(x = (x & ~type))
#define	SET_FS_FLAGS(x, flags)	(x = (x | (flags << 24)))
#define CLEAR_FS_F(x, flags)	(x = (x & ~(flags << 24)))
#define	FS_FLAG_SET(x, flags)	(x & (flags << 24))
#define	FS_TYPE_SET(x, type)	(x & type)

struct label_info {
	char *label;
	char *desc;
	char *password;
	size_t size;
	long flags;
	int selected;
	STAILQ_ENTRY(label_info) entries;
};
STAILQ_HEAD(label_info_head, label_info) label_info_list =
	STAILQ_HEAD_INITIALIZER(label_info_list);

struct zpool_disk {
	struct disk_info *di;
	int selected;
	STAILQ_ENTRY(zpool_disk) entries;
};
STAILQ_HEAD(zpool_disk_head, zpool_disk) zpool_disk_list = 
	STAILQ_HEAD_INITIALIZER(zpool_disk_list);

struct zfs_mount {
	char *path;
	STAILQ_ENTRY(zfs_mount) entries;
};
STAILQ_HEAD(zfs_mount_head, zfs_mount) zfs_mount_list = 
	STAILQ_HEAD_INITIALIZER(zfs_mount_list);

static void
add_label(const char *label, const char *desc, char *password, size_t size, long flags)
{
	struct label_info *li;

	li = safe_malloc(sizeof(*li));
	li->label = safe_strdup(label);
	li->desc = safe_strdup(desc);
	li->password = safe_strdup(password);
	li->size = size;
	li->flags = flags;
	li->selected = 0;

	STAILQ_INSERT_TAIL(&label_info_list, li, entries);
}

static int
label_exists(const char *label)
{
	int exists = 0;
	struct label_info *li;

	STAILQ_FOREACH(li, &label_info_list, entries) {
		if (strcmp(label, li->label) == 0) {
			exists = 1;
			break;
		}
	}

	return (exists);
}

static const char *
zpool_type_to_string(unsigned long type)
{
	const char *str = NULL;

	switch (type) {
		case ZFS_POOL_BASIC:
			str = "";
			break;

		case ZFS_POOL_MIRROR:
			str = "mirror";
			break;

		case ZFS_POOL_RAIDZ1:
			str = "raidz1";
			break;

		case ZFS_POOL_RAIDZ2:
			str = "raidz2";
			break;

		case ZFS_POOL_DISK:
			str = "disk";
			break;

		case ZFS_POOL_FILE:
			str = "file";
			break;

		case ZFS_POOL_SPARE:
			str = "spare";
			break;

		case ZFS_POOL_LOG:
			str = "log";
			break;

		case ZFS_POOL_CACHE:
			str = "cache";
			break;
	}

	return (str);
}

static char *
zpool_disk_list_to_string(unsigned long type)
{
	char *diskstr;
	char buf[2048];
	struct zpool_disk *zd;

	bzero(&buf, sizeof(buf));
	STAILQ_FOREACH(zd, &zpool_disk_list, entries) {
		struct disk_info *di = zd->di;
		strlcat(buf, di->disk, sizeof(buf));
		strlcat(buf, " ", sizeof(buf));
	}

	diskstr = NULL;
	if (!empty(buf)) {
		const char *typestr = zpool_type_to_string(type);
		char str[2048];

		buf[safe_strlen(buf) - 1] = 0;

		bzero(&str, sizeof(str));
		if (notnull(typestr))
			snprintf(str, sizeof(str), "(%s: %s)", typestr, buf);
		else
			snprintf(str, sizeof(str), "(%s)", buf);

		diskstr = safe_strdup(str);
	}

	return (diskstr);
}


static void
get_password(char *module, char *password)
{
	int loop;
	unsigned char password1[128];
	unsigned char password2[128];

	DialogInputAttrs |= DITEM_NO_ECHO;

	loop = 1;
	while (loop) {
		bzero(&password1, sizeof(password1));
		bzero(&password2, sizeof(password2));

		screen_clear("PASSWORD");
		dialog_inputbox(module, "Password", 8, 60, password1);
		dialog_inputbox(module, "Confirm", 8, 60, password2);

		if (strncmp(password1, password2, sizeof(password2)) == 0) {
			strlcpy(password, password1, sizeof(password1));	
			loop = 0;
		}
	}

	DialogInputAttrs &= ~DITEM_NO_ECHO;
}

static int
ufs_fire(dialogMenuItem *self)
{
	int loop;
	int enc;
	long flags;
	int64_t size;
	int64_t mb;
	char buf[64];
	char mnt[1024];
	char password[64];

	bzero(&buf, sizeof(buf));
	bzero(&mnt, sizeof(mnt));
	screen_clear("UFS");

	FS_ZERO(flags);
	SET_FS_TYPE(flags, FS_UFS);

	dialog_inputbox("UFS", "Size", 8, 70, buf);
	expand_number(buf, &size);
	b2mb(&size, &mb);

	loop = 1;
	while (loop == 1) {
		bzero(&mnt, sizeof(mnt));
		dialog_inputbox("UFS", "Mount Point", 8, 70, mnt);
		if (label_exists(mnt)) {
			snprintf(buf, sizeof(buf), "%s already specified", mnt);
			dialog_notify(buf);
			continue;
		}

		loop = 0;
	}

	enc = dialog_noyes("UFS", "Encrypt?", 8, 10);
	if (enc == 0) {
		SET_FS_FLAGS(flags, FS_FLAGS_ENCRYPT);
		bzero(&password, sizeof(password));
		get_password("UFS", password);
	}

	if (self->aux != 0)
		SET_FS_FLAGS(flags, self->aux);

	add_label(mnt, NULL, password, mb, flags);
	return (0);
}

static int
zpool_fire(dialogMenuItem *self)
{
	*((unsigned long *)self->data) = self->aux;
	return (0);
}

static int
zpool_disk_fire(dialogMenuItem *self)
{
	if (self->data) {
		struct zpool_disk *zd = self->data;
		zd->selected = !zd->selected;
		return (DITEM_SUCCESS);
	}

	return (DITEM_FAILURE);
}

static int
zpool_disk_checked(dialogMenuItem *self)
{
	struct zpool_disk *zd = self->data;
	if (zd->selected)
		return (TRUE);

	return (FALSE);
}

static void
get_zfs_zpool_type(unsigned long *type)
{
	int i;
	dialogMenuItem *menus;

	i = 0;
	menus = safe_malloc(sizeof(*menus)*(MAXZPOOLS+2));
	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);
	DMENUFDA(&menus[i++], "basic", "", &zpool_fire,
		type, ZFS_POOL_BASIC);
	DMENUFDA(&menus[i++], "disk", "", &zpool_fire,
		type, ZFS_POOL_DISK);
	DMENUFDA(&menus[i++], "file", "", &zpool_fire,
		type, ZFS_POOL_FILE);
	DMENUFDA(&menus[i++], "mirror", "", &zpool_fire,
		type, ZFS_POOL_MIRROR);
	DMENUFDA(&menus[i++], "raidz", "", &zpool_fire,
		type, ZFS_POOL_RAIDZ);
	DMENUFDA(&menus[i++], "raidz1", "", &zpool_fire,
		type, ZFS_POOL_RAIDZ1);
	DMENUFDA(&menus[i++], "raidz2", "", &zpool_fire,
		type, ZFS_POOL_RAIDZ2);
	DMENUFDA(&menus[i++], "spare", "", &zpool_fire,
		type, ZFS_POOL_SPARE);
	DMENUFDA(&menus[i++], "log", "", &zpool_fire,
		type, ZFS_POOL_LOG);
	DMENUFDA(&menus[i++], "cache", "", &zpool_fire,
		type, ZFS_POOL_CACHE);

	screen_clear("ZFS");
	dialog_menu("ZFS", "ZFS Pool Type",
		7+i-2, 40, i-2, -i+2,
		menus+2, "", NULL, NULL);
	
	free(menus);
}

static void
get_zfs_zpool_disks(void)
{
	int i;
	dialogMenuItem *menus;
	struct disk_info *di;
	struct disk_list *disks;
	struct zpool_disk *zpool_disks;

	i = 0;
	disks = get_disk_list();
	zpool_disks = safe_malloc(sizeof(*zpool_disks) * (disks->ndisks + 2));

	menus = safe_malloc(sizeof(*menus) * (disks->ndisks + 2));
	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);
	STAILQ_FOREACH(di, &disks->list, entries) {
		if (di->used)
			continue;

		zpool_disks[i].di = di;
		zpool_disks[i].selected = 0;
		DMENUFCD(&menus[i], di->disk,  "", &zpool_disk_fire,
			&zpool_disk_checked, &zpool_disks[i]);
		i++;
	}

	screen_clear("ZFS");
	dialog_checklist("ZFS", "ZFS Pool Devices",
		7+i-2, 60, i-2, -i+2, menus+2, "");

	for (i = 0;i < (int)(disks->ndisks + 2);i++) {
		if (zpool_disks[i].selected) {
			struct zpool_disk *disk;

			disk = safe_malloc(sizeof(*disk));
			disk->di = zpool_disks[i].di;
			disk->selected = zpool_disks[i].selected;
			STAILQ_INSERT_TAIL(&zpool_disk_list, disk, entries);
		}

		zpool_disks[i].di = NULL;
	}

	free(zpool_disks);
}

static void
get_zfs_mount_points(char **pptr)
{
	int res;
	size_t bytes;
	char mnt[PATH_MAX];
	struct zfs_mount *temp, *z;

	res = 0;
	bytes = 0;
	while (res == 0) {
		bzero(&mnt, sizeof(mnt));
		screen_clear("ZFS");

		res = dialog_inputbox("ZFS", "Mount Point", 8, 70, mnt);
		if (empty(mnt))
			continue;

		if (label_exists(mnt)) {
			char buf[1024];
			snprintf(buf, sizeof(buf), "%s already specified", mnt);
			dialog_notify(buf);
			continue;
		}

		if (res == 0) {
			struct zfs_mount *zm;

			zm = safe_malloc(sizeof(*zm));
			zm->path = safe_strdup(mnt);
			STAILQ_INSERT_TAIL(&zfs_mount_list, zm, entries);

			bytes += safe_strlen(mnt) + 2;
		}
	}

	*pptr = safe_malloc(bytes + 1);
	STAILQ_FOREACH_SAFE(z, &zfs_mount_list, entries, temp) {
		strlcat(*pptr, z->path, bytes);
		strlcat(*pptr, ",", bytes);

		STAILQ_REMOVE(&zfs_mount_list, z, zfs_mount, entries);
		free(z->path);
		free(z);
	}

	(*pptr)[strlen(*pptr) - 1] = 0;
}

static void
free_zpool_disk_list(void)
{
	struct zpool_disk *zd, *temp;

	STAILQ_FOREACH_SAFE(zd, &zpool_disk_list, entries, temp) {
		STAILQ_REMOVE(&zpool_disk_list, zd, zpool_disk, entries);
		free(zd);
	}
}

static int
zfs_fire(dialogMenuItem *self)
{
	int enc;
	long flags;
	int64_t size;
	int64_t mb;
	unsigned long type;
	char buf[1024];
	char password[64];
	char *mount_points;

	bzero(&buf, sizeof(buf));
	screen_clear("ZFS");

	FS_ZERO(flags);
	SET_FS_TYPE(flags, FS_ZFS);

	dialog_inputbox("ZFS", "Size", 8, 70, buf);
	expand_number(buf, &size);
	b2mb(&size, &mb);

	type = 0;
	mount_points = NULL;

	get_zfs_zpool_type(&type);
	SET_FS_FLAGS(flags, type);

	get_zfs_zpool_disks();
	get_zfs_mount_points(&mount_points);

	enc = dialog_noyes("ZFS", "Encrypt?", 8, 10);
	if (enc == 0) {
		SET_FS_FLAGS(flags, FS_FLAGS_ENCRYPT);
		bzero(&password, sizeof(password));
		get_password("ZFS", password);
	}

	if (self->aux != 0)
		SET_FS_FLAGS(flags, self->aux);

	add_label(mount_points, NULL, password, mb, flags);
	free(mount_points);

	return (0);
}

static int
swap_fire(dialogMenuItem *self)
{
	int enc;
	long flags;
	int64_t size;
	int64_t mb;
	char buf[64];
	char password[64];

	screen_clear("SWAP");

	FS_ZERO(flags);
	SET_FS_TYPE(flags, FS_SWAP);

	bzero(&buf, sizeof(buf));
	dialog_inputbox("SWAP", "Size", 8, 70, buf);

	expand_number(buf, &size);
	b2mb(&size, &mb);

	enc = dialog_noyes("SWAP", "Encrypt?", 8, 10);
	if (enc == 0) {
		SET_FS_FLAGS(flags, FS_FLAGS_ENCRYPT);
		bzero(&password, sizeof(password));
		get_password("SWAP", password);
	}

	if (self->aux != 0)
		SET_FS_FLAGS(flags, self->aux);

	add_label("none", NULL, password, mb, flags);
	return (0);
}


static int
dialog_partition(void)
{
	int i;
	int res;
	unsigned long data;
	dialogMenuItem *menus;

	i = 0;
	menus = safe_malloc(sizeof(*menus)*MAXTYPES+2);
	DMENUF(&menus[i++], "OK", NULL, NULL);
	DMENUF(&menus[i++], "Cancel", NULL, NULL);

	data = 0;
	DMENUFD(&menus[i++], "UFS", "UFS filesystem",
		ufs_fire, &data);

	DMENUFDA(&menus[i++], "UFS+S", "UFS + soft updates",
		ufs_fire, &data, FS_FLAGS_SOFTUPDATES);

	DMENUFDA(&menus[i++], "UFS+J", "UFS + gjournal",
		ufs_fire, &data, FS_FLAGS_GJOURNAL);

	DMENUFD(&menus[i++], "ZFS", "zeta filesystem",
		zfs_fire, &data);

	DMENUFD(&menus[i++], "SWAP", "swap partition" ,
		swap_fire, &data);

	res = 0;
	while (res == 0) {
		screen_clear(MODULE);
		res = dialog_menu(MODULE, "Please select a filesystem:",
			-1, 60, 7, -i+2, menus+2, "", NULL, NULL);
	}
	
	free(menus);
	return (res);
}


static void
write_disk_labels(void)
{
	struct label_info *li;

	STAILQ_FOREACH(li, &label_info_list, entries) {
		char line[8192];
		char num[32];

		bzero(&num, sizeof(num));
		snprintf(num, sizeof(num), "%ld", li->size);

		bzero(&line, sizeof(line));
		switch (GET_FS_TYPE(li->flags)) {
			case FS_UFS: {
				strlcpy(line, "UFS", sizeof(line));
				if (FS_FLAG_SET(li->flags, FS_FLAGS_GJOURNAL))
					strlcat(line, "+J", sizeof(line));
				if (FS_FLAG_SET(li->flags, FS_FLAGS_SOFTUPDATES))
					strlcat(line, "+S", sizeof(line));
				if (FS_FLAG_SET(li->flags, FS_FLAGS_ENCRYPT))
					strlcat(line, ".eli", sizeof(line));

				strlcat(line, " ", sizeof(line));
				strlcat(line, num, sizeof(line));
				strlcat(line, " ", sizeof(line));
				strlcat(line, li->label, sizeof(line));

				disk(0, line);

				if (FS_FLAG_SET(li->flags, FS_FLAGS_ENCRYPT))
					encpass(li->password);

				break;
			}

			case FS_ZFS: {
				char *diskstr;
				unsigned long zpool;

				strlcpy(line, "ZFS", sizeof(line));
				if (FS_FLAG_SET(li->flags, FS_FLAGS_ENCRYPT))
					strlcat(line, ".eli", sizeof(line));
				
				strlcat(line, " ", sizeof(line));
				strlcat(line, num, sizeof(line));
				strlcat(line, " ", sizeof(line));
				strlcat(line, li->label, sizeof(line));

				zpool = 0;
				if (FS_FLAG_SET(li->flags, ZFS_POOL_BASIC))
					zpool = ZFS_POOL_BASIC;

				if (FS_FLAG_SET(li->flags, ZFS_POOL_MIRROR))
					zpool = ZFS_POOL_MIRROR;
		
				if (FS_FLAG_SET(li->flags, ZFS_POOL_RAIDZ1))
					zpool = ZFS_POOL_RAIDZ1;

				if (FS_FLAG_SET(li->flags, ZFS_POOL_RAIDZ2))
					zpool = ZFS_POOL_RAIDZ2;
	
				if (FS_FLAG_SET(li->flags, ZFS_POOL_DISK))
					zpool = ZFS_POOL_DISK;

				if (FS_FLAG_SET(li->flags, ZFS_POOL_FILE))
					zpool = ZFS_POOL_FILE;

				if (FS_FLAG_SET(li->flags, ZFS_POOL_SPARE))
					zpool = ZFS_POOL_SPARE;

				if (FS_FLAG_SET(li->flags, ZFS_POOL_LOG))
					zpool = ZFS_POOL_LOG;

				if (FS_FLAG_SET(li->flags, ZFS_POOL_CACHE))
					zpool = ZFS_POOL_CACHE;

				diskstr = zpool_disk_list_to_string(zpool);
				if (notnull(diskstr)) {
					strlcat(line, " ", sizeof(line));
					strlcat(line, diskstr, sizeof(line));
					free(diskstr);
				}

				disk(0, line);

				if (FS_FLAG_SET(li->flags, FS_FLAGS_ENCRYPT))
					encpass(li->password);

				break;
			}

			case FS_SWAP: {
				strlcpy(line, "SWAP", sizeof(line));
				if (FS_FLAG_SET(li->flags, FS_FLAGS_ENCRYPT))
					strlcat(line, ".eli", sizeof(line));

				strlcat(line, " ", sizeof(line));
				strlcat(line, num, sizeof(line));
				strlcat(line, " ", sizeof(line));
				strlcat(line, li->label, sizeof(line));

				disk(0, line);

				if (FS_FLAG_SET(li->flags, FS_FLAGS_ENCRYPT))
					encpass(li->password);
				break;
			}
		}
	}

	commitDiskLabel();
	free_zpool_disk_list();
}

static int
label_fire(dialogMenuItem *self)
{
	const char *disk;
	struct disk_info *di;

	disk = getconfigval("disk0");
	di = get_disk_info(disk, NULL);

	dialog_partition();

	free_disk_info((void *)di);
	return (0);
}


int
dialog_label(void)
{
	int i;
	char *buf;
	int swap;
	int status;
	dialogMenuItem *menus;


	buf = safe_malloc(BUFSZ+1);
	status = run_pcsysinstall(buf, BUFSZ, "sys-mem", NULL);
	swap = atoi(buf) * 2 + 32;
	free(buf);

	screen_clear(MODULE);
	if (dialog_yesno(MODULE,
	    "Do you wish to configure your BSD label?\n\n"
	    "<give a brief explanation of what are BSD labels>", 8, 60)) {
		char value[64];

		/*
		 * This is the default label configuration.
		 */
		disk(0, "UFS+S 1024 /");
		snprintf(value, sizeof(value), "SWAP %d none", swap);
		disk(0, value);
		disk(0, "UFS+S 0 /usr");
		commitDiskLabel();

		return (0);
	}

	i = 0;
	menus = safe_malloc(sizeof(*menus)*MAXLABELS+2);
	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);
	DMENUF(&menus[i++], "Add Label", "", label_fire);

	DialogX = (COLS - 50) / 2;
	DialogY = 2;

	screen_clear(MODULE);
	dialog_menu(MODULE, "BSD partitions:", 12, 50, 3, -i+2,
    	menus+2, "", NULL, NULL);
	free(menus);

	write_disk_labels();
	return (0);
}

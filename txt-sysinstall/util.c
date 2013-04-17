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
 * $Id: util.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <sys/types.h>
#include <sys/queue.h>
#include <sys/wait.h>
#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dialog.h>
#include <ctype.h>
#include <curses.h>

#include "txt-sysinstall.h"

#define	MB	(1024 * 1024)

struct config {
	char *key;
	char *value;
	STAILQ_ENTRY(config) entries;
};
STAILQ_HEAD(confighead, config) configlist;

static struct disk_list DISK_LIST;
static struct disk_info *CURRENT_DISK;

struct disk_info *
set_current_disk(struct disk_info *di)
{
	struct disk_info *temp;

	temp = CURRENT_DISK;
	CURRENT_DISK = di;

	return (temp);
}

struct disk_info *
get_current_disk(void)
{
	return (CURRENT_DISK);
}

void
screen_clear(const char *module)
{
	dialog_clear();
	mvprintw(0, 0, "%s installer >> %s", OSNAME,  module);
}

int
empty(const char *str)
{
	int res = 1;

	if (str == NULL || str[0] == 0)
		return (res);

	while (*str != 0) {
		if (!isspace(*str) && !iscntrl(*str))
			return (0);
		str++;
	}

	return (res);
}

void *
safe_malloc(size_t size)
{
	void *ptr;

	if ((ptr = malloc(size)) == NULL)
		err(1, "malloc failed!");

	bzero(ptr, size);
	return (ptr);
}

size_t
safe_strlen(const char *str)
{
	size_t len = 0;

	if (notnull(str))
		len = strlen(str);

	return (len);
}

char *
safe_strdup(const char *str)
{
	size_t len;
	char *copy;

	copy = NULL;
	if (notnull(str)) {
		len = safe_strlen(str) + 1;
		copy = safe_malloc(len);
		memcpy(copy, str, len);
	}

	return (copy);
}

/*
 * Run pc-sysinstall with the specified cmd. If the function
 * returns non-NULL, it's up to the caller to free the buf.
 */
int
run_pcsysinstall(char *buf, size_t size, const char *cmd1, const char *cmd2)
{
	int fd[2];
	ssize_t len;
	int status;

	status = 0;
	if (pipe(fd) < 0)
		return (-1);

	switch (fork()) {
	case 0:
		close(fd[1]);
		dup2(fd[0], STDOUT_FILENO);
		close(fd[0]);
		execl(PCSYSINSTALL, "pc-sysinstall", cmd1, cmd2, NULL);
		_exit(1);

	case -1:
		close(fd[0]);
		close(fd[1]);
		return (-1);

	default:
		wait(&status);
		if (status != 0) {
			end_dialog();
			close(fd[0]);
			close(fd[1]);
			return (status);
		}
	}

	if ((len = read(fd[1], buf, size)) < 0) {
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}

	close(fd[0]);
	close(fd[1]);
	return (status);
}

struct disk_info *
get_disk_info(const char *disk, const char *desc)
{
	char *buf;
	char *token;
	struct disk_info *di;

	if (disk == NULL)
		return (NULL);

	buf = safe_malloc(BUFSZ+1);
	run_pcsysinstall(buf, BUFSZ, "disk-info", disk);

	di = safe_malloc(sizeof(*di));
	di->disk = safe_strdup(disk);
	if (notnull(desc))
		di->desc = safe_strdup(desc);

	while ((token = strsep(&buf, "\n")) != NULL &&
		safe_strlen(token) > 0) {
		char *key, *val;

		key =  strsep(&token, "=");
		val = strsep(&token, ":");
		if (strcmp(key, "cylinders") == 0)
			di->ncyl = strtol(val, 0, 10);
		else if (strcmp(key, "heads") == 0)
			di->nhead = strtol(val, 0, 10);
		else if (strcmp(key, "sectors") == 0)
			di->nsect = strtol(val, 0, 10);
		else if (strcmp(key, "size") == 0)
			di->size = strtol(val, 0, 10);
		else if (strcmp(key, "type") == 0 && notnull(val))
			di->type = safe_strdup(val);
		di->used = 0;
	}

	free(buf);
	return (di);
}

struct disk_info *
get_disk_info_by_disk(const char *disk)
{
	struct disk_info *di;

	di = NULL;
	STAILQ_FOREACH(di, &DISK_LIST.list, entries) {
		if (strcmp(di->disk, disk) == 0)
			break;
	}

	return (di);
}

void
free_disk_info(void *args)
{
	struct disk_info *di = (struct disk_info *)args;

	if (di != NULL) {
		free(di->disk);
		free(di->desc);
		free(di->type);
		free(di);
	}
}

struct disk_list *
get_disk_list(void)
{
	struct disk_list *ptr;

	ptr = &DISK_LIST;
	if (ptr->ndisks == 0) {
		char *buf, *token;

		buf = safe_malloc(BUFSZ+1);
		run_pcsysinstall(buf, BUFSZ, "disk-list", DISK_ARGS);

		ptr->ndisks = 0;
		STAILQ_INIT(&ptr->list);

		while ((token = strsep(&buf, "\n")) != NULL
			&& safe_strlen(token) > 0) {
			struct disk_info *di;
			char *disk, *desc;
	
			disk = strsep(&token, ":");
			desc = strsep(&token, ":");
			desc += 2;
			desc[safe_strlen(desc) - 1] = 0;
	
			di = get_disk_info(disk, desc);
			STAILQ_INSERT_TAIL(&ptr->list, di, entries);
			ptr->ndisks++;
		}
	}
	
	return (ptr);
}

void
free_disk_list(void)
{
	struct disk_list *ptr;
	struct disk_info *di, *temp;

	ptr = &DISK_LIST;
	STAILQ_FOREACH_SAFE(di, &ptr->list, entries, temp) {
		STAILQ_REMOVE(&ptr->list, di, disk_info, entries);
		free_disk_info(di);
	}
}

void
dialog_busy(int height, int width, const char *str)
{
	draw_box(stdscr, (LINES - height) / 2, (COLS - width) / 2,
		height, width, dialog_attr, border_attr);
	draw_shadow(stdscr, (LINES - height) / 2, (COLS - width) / 2,
	    height, width);
	attron(dialog_attr);
	mvprintw(LINES / 2 - 1, COLS / 2 - safe_strlen(str) / 2, str);
	dialog_update();
}

void
sysinstall_init(void)
{
	STAILQ_INIT(&configlist);
	get_disk_list();
}

void
sysinstall_fini(void)
{
	struct config *c, *c_temp;

	free_disk_list();

	STAILQ_FOREACH_SAFE(c, &configlist, entries, c_temp) {
		STAILQ_REMOVE(&configlist, c, config, entries);
		free(c->key);
		free(c->value);
		free(c);
	}
}

void
appendconfig(const char *key, const char *value)
{
	struct config *c;

	c = safe_malloc(sizeof(*c));
	c->key = safe_strdup(key);
	c->value = NULL;
	if (value) {
		c->value = safe_strdup(value);
	}
	STAILQ_INSERT_TAIL(&configlist, c, entries);
}

const char *
getconfigval(const char *key)
{
	const char *value;
	struct config *c;

	if (key == NULL)
		return (NULL);

	value = NULL;
	STAILQ_FOREACH(c, &configlist, entries) {
		if (strcmp(key, c->key) == 0) {
			value = c->value;
			break;
		}
	}

	return (value);
}

void
save_config(void)
{
	FILE *fp;
	struct config *c;

	if ((fp = fopen("result.cfg", "w")) == NULL) {
		end_dialog();
		err(1, "couldn't open config file\n");
		return;
	}

	STAILQ_FOREACH(c, &configlist, entries) {
		fprintf(fp, "%s", c->key);
		if (c->value)
			fprintf(fp, "=%s", c->value);
		fprintf(fp, "\n");
	}

	fclose(fp);
}

int
b2mb(int64_t *b, int64_t *mb)
{
	if (b == NULL || mb == NULL)
		return (-1);

	if (*b >= MB)
		*mb = *b / MB;
	else if (*b != 0)
		*mb = 1;
	else
		*mb = 0;
	
	return (0);
}

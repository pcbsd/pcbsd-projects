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
 * $Id: packages.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <sys/types.h>
#include <sys/queue.h>
#include <libgen.h>
#include <search.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dialog.h>
#include <ftpio.h>

#include "txt-sysinstall.h"

#define MODULE		"Package selection"
#define	PKGSTRSIZE	16384	

struct package_info {
	char *package;
	char *description;
	int selected;
	STAILQ_ENTRY(package_info) entries;
};

struct package_category_head {
	char *category;
	int size;
	STAILQ_HEAD(packagehead, package_info) packagelist;
};

struct package_category {
	char *category;
	STAILQ_ENTRY(package_category) entries;
};

static int categorylist_size = 0;
STAILQ_HEAD(categoryhead, package_category) categorylist =
	STAILQ_HEAD_INITIALIZER(categorylist);


static struct package_category_head *
add_category(const char *category, const char *package)
{
	struct package_category_head *pch;
	struct package_category *pc;
	ENTRY item;

	pch = safe_malloc(sizeof(*pch));
	pch->category = safe_strdup(category);
	pch->size = 1;
	STAILQ_INIT(&pch->packagelist);

	pc = safe_malloc(sizeof(*pc));
	pc->category = safe_strdup(category);
	STAILQ_INSERT_TAIL(&categorylist, pc, entries);
	categorylist_size++;

	item.key = safe_strdup(category);
	item.data = pch;
	hsearch(item, ENTER);

	return (pch);
}

static void
add_category_package(struct package_category_head *pch,
	const char *package, const char *description)
{
	struct package_info *pi;

	pi = safe_malloc(sizeof(*pi));
	pi->package = safe_strdup(package);
	pi->description = safe_strdup(description);
	pi->selected = 0;
	STAILQ_INSERT_TAIL(&pch->packagelist, pi, entries);

	pch->size++;
}

static void
parse_package_index(const char *path)
{
	FILE *fp;
	char *buf;
	char *ptr;
	char *token;
	int i;

	fp = fopen(path, "r");
	if (fp == NULL)
		return;

	i = 0;
	buf = safe_malloc(BUFSZ+1);
	while (fgets(buf, BUFSZ, fp) != NULL) {
		char *package, *description, *category;
		struct package_category_head *pch;
		ENTRY item, *find;

		ptr = buf;
		package = description = category = NULL;
		while ((token = strsep(&ptr, "|")) != NULL) {
			if (i == 1)
				package = basename(token);
			else if (i == 3)
				description = token;
			else if (i == 6)
				category = strsep(&token, " ");
			i++;
		}

		item.key = safe_strdup(category);
		item.data = NULL;

		find = hsearch(item, FIND);
		if (find == NULL)
			pch = add_category(category, package);
		else
			pch = find->data;

		add_category_package(pch, package, description);

		free(item.key);
		i = 0;
	}

	fclose(fp);
	free(buf);
}

static int
package_fire(dialogMenuItem *self)
{
	if (self->data) {
		struct package_info *pi = self->data;
		pi->selected = !pi->selected;
		return (DITEM_SUCCESS);
	}

	return (DITEM_FAILURE);
}

static int
package_checked(dialogMenuItem *self)
{
	struct package_info *pi = self->data;
	if (pi->selected)
		return (TRUE);

	return (FALSE);
}

static void
package_selected(dialogMenuItem *self, int selected)
{
	struct package_info *pi = self->data;

	if (selected) {
		char buf[1024];

		bzero(&buf, sizeof(buf));
		attron(dialog_attr);

		memset(buf, ' ', 100);
		mvprintw(LINES / 2 + 5, COLS / 2 - safe_strlen(buf) / 2, buf);
		dialog_update();

		mvprintw(LINES / 2 + 5, COLS / 2 - safe_strlen(pi->description) / 2, pi->description);
		dialog_update();
	}
}

static int
category_fire(dialogMenuItem *self)
{
	ENTRY item, *find;
	int res;

	item.key = self->prompt;
	item.data = NULL;

	res = DITEM_FAILURE;
	find = hsearch(item, FIND);
	if (find != NULL) {
		dialogMenuItem *menus;
		struct package_category_head *pch;
		struct package_info *pi;
		int i;

		pch = find->data;
		menus = safe_malloc(sizeof(*menus)*pch->size+2);

		i = 0;
		DMENUF(&menus[i++], "OK", NULL, NULL);
		DMENUF(&menus[i++], "Cancel", NULL, NULL);

		STAILQ_FOREACH(pi, &pch->packagelist, entries) {
			DMENU(&menus[i], pi->package, "", package_checked,
				package_selected, package_fire, pi, 0);
			i++;
		}

		screen_clear(MODULE);
		dialog_checklist(MODULE,
			"Please select packages:",
			20, 100, 10, -i+2, menus+2, "");

		free(menus);
		res = DITEM_SUCCESS;
	} 

	return (res);
}

static void
dialog_package_category(void)
{
	dialogMenuItem *menus;
	struct package_category *temp;
	int i;

	menus = safe_malloc(sizeof(*menus)*categorylist_size+2);

	i = 0;
	DMENUF(&menus[i++], "OK", NULL, NULL);
	DMENUF(&menus[i++], "Cancel", NULL, NULL);

	STAILQ_FOREACH(temp, &categorylist, entries) {
		DMENUF(&menus[i++], temp->category, "", category_fire);
	}

	screen_clear(MODULE);
	dialog_menu(MODULE,
		"Please select a category:",
		20, 40, 10, -i+2, menus+2, "", NULL, NULL);

	free(menus);
}

static void
free_package_lists(void)
{
	struct package_category *pc, *temp;

	STAILQ_FOREACH_SAFE(pc, &categorylist, entries, temp) {
		struct package_category_head *pch;
		ENTRY item, *find;

		item.key = pc->category;
		item.data = NULL;

		find = hsearch(item, FIND);
		pch = find->data;

		if (pch != NULL) {
			struct package_info *pi1, *pi2;

			STAILQ_FOREACH_SAFE(pi1, &pch->packagelist, entries, pi2) {
				STAILQ_REMOVE(&pch->packagelist, pi1,
					package_info, entries);
				free(pi1->package);
				free(pi1->description);
				free(pi1);
			}

			STAILQ_REMOVE(&categorylist, pc, package_category, entries);
			free(pc->category);
			free(pc);
		}
	}
}

static void
set_marked_packages(void)
{
	struct package_category *pc;
	char *pkgstr, *ptr;

	pkgstr = safe_malloc(PKGSTRSIZE);
	ptr = &pkgstr[0];

	STAILQ_FOREACH(pc, &categorylist, entries) {
		struct package_category_head *pch;
		ENTRY item, *find;

		item.key = pc->category;
		item.data = NULL;

		find = hsearch(item, FIND);
		pch = find->data;

		if (pch != NULL) {
			struct package_info *pi;

			STAILQ_FOREACH(pi, &pch->packagelist, entries) {
				if (pi->selected) {
					if (pkgstr[0] == 0) {
						strlcat(ptr, pi->package, PKGSTRSIZE);

					} else {
						strlcat(ptr, ",", PKGSTRSIZE);
						strlcat(ptr, pi->package, PKGSTRSIZE);
					}
				}
			}
		}
	}

	if (notnull(ptr))
		installPackages(pkgstr);

	free(pkgstr);
}

int
dialog_packages(void)
{
	char *buf;

	screen_clear(MODULE);
	dialog_busy(5, 60, "Getting packages...");

	buf = safe_malloc(BUFSZ+1);
	run_pcsysinstall(buf, BUFSZ, "get-packages", NULL);
	run_pcsysinstall(buf, BUFSZ, "list-packages", "@INDEX@");
	buf[safe_strlen(buf) - 1] = 0;

	parse_package_index(buf);
	dialog_package_category();

	set_marked_packages();
	free_package_lists();

	free(buf);
	return (0);
}

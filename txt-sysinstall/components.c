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
 * $Id: components.c 7782 2010-10-12 22:32:13Z johnh $
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>

#include "txt-sysinstall.h"

#define MAXCOMP		50	
#define	COMPSTRSIZE	16384	
#define MODULE		"Components selection"

struct component {
	char *name;
	int selected;
};

static int
component_checked(dialogMenuItem *self)
{
	struct component *c = self->data;
	if (c->selected)
		return (TRUE);

	return (FALSE);
}

static int
component_fire(dialogMenuItem *self)
{
	if (self->data) {
		struct component *c = self->data;
		c->selected = !c->selected;
		return (DITEM_SUCCESS);
	}

	return (DITEM_FAILURE);
}

int
dialog_components(void)
{
	char *token;
	char *buf;
	char *ptr;
	char *compstr;
	dialogMenuItem *menus;
	struct component *components;
	int status;
	int i;


	buf = safe_malloc(BUFSZ+1);
	status = run_pcsysinstall(buf, BUFSZ, "list-components", NULL);
	menus = safe_malloc(sizeof(*menus)*MAXCOMP);
	compstr = safe_malloc(COMPSTRSIZE);

	components = safe_malloc(sizeof(*components)*MAXCOMP);

	i = 0;
	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);

	while ((token = strsep(&buf, "\n")) != NULL && safe_strlen(token) > 0 &&
	    i < MAXCOMP) {
		char *key, *value;


		key = strsep(&token, ":");
		value = strsep(&token, ":");
		value++;

		if (!strcmp(key, "name")) {
			menus[i].prompt = value;
			components[i].name = value;
		}

		if (!strcmp(key, "desc")) {
			menus[i].title = value;
			menus[i].fire = component_fire;
			menus[i].checked = component_checked;
			menus[i].data = &components[i];
			i++;
		}
	}

	if (i > 2) {
		screen_clear(MODULE);
		dialog_checklist(MODULE,
	    	"Please select the components you want to install:",
	    	17, 60, 10, -i+2, menus+2, "");

		ptr = &compstr[0];
		for (i = 0;i < MAXCOMP;i++) {
			if (components[i].selected) {
				if (compstr[0] == 0) {
					strlcat(ptr, components[i].name, BUFSZ);

				} else {
					strlcat(ptr, ",", BUFSZ);
					strlcat(ptr, components[i].name, BUFSZ);
				}
			}
		}

		if (notnull(ptr))
			installComponents(compstr);
	}

	free(buf);
	free(compstr);
	free(menus);

	return (0);
}

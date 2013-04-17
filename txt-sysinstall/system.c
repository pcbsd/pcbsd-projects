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
 * $Id: components.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>

#include "txt-sysinstall.h"

#define MODULE		"System selection"

static int
system_fire(dialogMenuItem *self)
{
	installType(self->prompt);
	if (!strcmp(self->prompt, "FreeBSD"))
		packageType("split");
	else
		packageType("uzip");

	return (0);
}

int
dialog_system(void)
{
	dialogMenuItem *menus;
	int i;

	i = 0;
	menus = safe_malloc(sizeof(*menus)*6);
	DMENUF(&menus[i++], "Next", NULL, NULL);
	DMENUF(&menus[i++], "Back", NULL, NULL);
	DMENUF(&menus[i++], "PC-BSD", "", system_fire);
	DMENUF(&menus[i++], "FreeBSD", "", system_fire);

	screen_clear(MODULE);
	dialog_menu(MODULE, "Please select the system to install:",
	    9, 40, 2, -2, menus+2, "", NULL, NULL);


	free(menus);
	return (0);
}

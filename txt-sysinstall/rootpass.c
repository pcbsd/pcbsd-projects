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
 * $Id: rootpass.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dialog.h>
#include <string.h>

#include "txt-sysinstall.h"

#define MODULE	"Root password"


int
dialog_rootpass(void)
{
	int loop;
	char rootPass1[64];
	char rootPass2[64];

	screen_clear(MODULE);
	dialog_notify("You will be prompted to setup your root account "
	    "password.\n\nThe root account is a special account that has "
	    "access to\nall system resources.");

	loop = 1;
	while (loop) {
		bzero(rootPass1, sizeof(rootPass1));
		bzero(rootPass2, sizeof(rootPass2));
		screen_clear(MODULE);

		DialogInputAttrs |= DITEM_NO_ECHO;
		dialog_inputbox(MODULE, "Please type the root password:",
			8, 60, rootPass1);
		dialog_inputbox(MODULE, "Please confirm the root password:",
			8, 60, rootPass2);
		DialogInputAttrs &= ~DITEM_NO_ECHO;

		if (strncmp(rootPass1, rootPass2, sizeof(rootPass1)) == 0)
			loop = 0;
	}

	rootPass(rootPass2);
	return (0);
}

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
 * $Id: useradd.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <dialog.h>
#include <stdlib.h>
#include <string.h>

#include "txt-sysinstall.h"

#define MODULE "User setup"


char shellPath[64];

static int
shell_fire(dialogMenuItem *self)
{
	snprintf(shellPath, sizeof(shellPath), "/bin/%s", self->prompt);

	return (0);
}

int
dialog_useradd(void)
{
	unsigned char username[64], realname[64];
	unsigned char password1[64];
	unsigned char password2[64];
	unsigned char prompt[1024];
	int loop;
	int add;

	dialogMenuItem shells[] = {
		{ .prompt = "bash", .title = "Bourne Again Shell",
		  .fire = shell_fire },
		{ .prompt = "csh", .title = "C Shell",
		  .fire = shell_fire },
		{ .prompt = "tcsh", .title = "TENEX C Shell",
		  .fire = shell_fire },
		{ .prompt = "zsh", .title = "Z Shell",
		  .fire = shell_fire },
	};

	bzero(username, sizeof(username));
	bzero(realname, sizeof(realname));
	bzero(password1, sizeof(password1));
	bzero(password2, sizeof(password2));

useradd_restart:
	screen_clear(MODULE);
	if (dialog_inputbox("Add user", "Please insert the desired username:",
	    8, 60, username) == 0 && !empty(username)) {
		dialog_inputbox("Add user", "Please insert the real name:",
	   	 8, 60, realname);

		loop = 1;
		while (loop) {
			DialogInputAttrs |= DITEM_NO_ECHO;
			dialog_inputbox("Add user", "Please type the password for this user:",
		   	 8, 60, password1);
			dialog_inputbox("Add user", "Please confirm the password for this user:",
		   	 8, 60, password2);
			DialogInputAttrs &= ~DITEM_NO_ECHO;
			if (strncmp(password1, password2, sizeof(password2)) == 0)
				loop = 0;
		}

		dialog_menu("Add user", "Please select the shell for this user:",
		    11, 60, 4, -4, shells, NULL, NULL, NULL);

		snprintf(prompt, sizeof(prompt), "Is the following information "
		    "correct?\n\n"
		    "Username:\t%s\nReal name:\t%s\nShell:\t%s\n",
		    username, realname, shellPath);
		screen_clear(MODULE);
		add = dialog_yesno("Add user", prompt, 10, 60);
		if (add == FALSE) {
			userName(username);
			userComment(realname);
			userPass(password2);
			userGroups("wheel,operator");
			autoLoginUser(username);
			userShell(shellPath);
			commitUser();
		} else {
			goto useradd_restart;
		}
	}

	return (0);
}

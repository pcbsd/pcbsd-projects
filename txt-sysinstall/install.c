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
 * $Id: install.c 7781 2010-10-12 21:04:39Z johnh $
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dialog.h>
#include <signal.h>
#include <poll.h>
#include <libgen.h>

#include "txt-sysinstall.h"

#define MODULE		"Installation"

static void
show_progress(char *line)
{
	dialog_busy(5, 60, line);

#if 0
	static int nfiles = 0, cfile = 0;
	static char value[64];
	char prompt[64];

	if (!strncmp(line, "Running gpart on", 16)) {
		dialog_busy(5, 60, "Partitioning and labeling disk...");
	} else if (!strcmp(line, "pc-sysinstall: Extraction Finished")) {
		screen_clear(MODULE);
		dialog_busy(5, 60, "Finishing installation...");
	} else if (!strncmp(line, "Cleaning up", 11)) { 	
		dialog_busy(5, 60, "Cleaning up disk...");
	} else if (!strncmp(line, "NEWFS:", 6)) {
		strlcpy(value, line+7, sizeof(value));
		snprintf(prompt, sizeof(prompt), "Creating filesystem %s",
		    value);
		dialog_busy(5, 60, prompt);
	} else if (!strncmp(line, "FETCH:", 6)) {
		screen_clear(MODULE);
		dialog_busy(5, 60, "Preparing to fetch...");
		strlcpy(value, basename(line+6), sizeof(value));
	} else if (!strncmp(line, "SIZE:", 5)) {
		int size, down;

		sscanf(line, "SIZE: %u DOWNLOADED: %u", &size, &down);
		if (down == 0)
			down = 1;
		if (size == 0)
			size = 1;
		if (down > size)
			down = size;
		snprintf(prompt, sizeof(prompt), "Downloading file %s",
		    value);
		dialog_gauge("File download", prompt,
		    (LINES - 7) / 2, (COLS - 60) / 2, 7, 60,
		    (down * 100) / size);
	} else if (!strncmp(line, "INSTALLCOUNT:", 13)) {
		nfiles = atoi(line+14);
		cfile = 0;
	/* XXX we should be using a FSM */
	} else if (!strncmp(line, "x ", 2)) {
		cfile++;

		snprintf(prompt, sizeof(prompt), "%s", line + 3);
		dialog_gauge("Installation progress", prompt,
		    (LINES - 7) / 2, (COLS - 60) / 2, 7, 60,
		    (cfile * 100) / nfiles);

	}
#endif
}

static int signaled = 0;

static void
sig_chld(int sig)
{
	signaled++;
}

void
do_install(void)
{
	int fd[2];
	int status;
	struct pollfd pfd;

	screen_clear(MODULE);

	if (dialog_yesno(MODULE, "Do you want to start the installation?",
	    5, 60))
		return;

	dialog_busy(5, 60, "Initializing installation...");

	if (pipe(fd) < 0) {
		end_dialog();
		err(1, "cannot create a pipe");
	}

	signal(SIGCHLD, sig_chld);

	switch (fork()) {
	case 0:
		close(fd[1]);
		dup2(fd[0], STDERR_FILENO);
		dup2(fd[0], STDOUT_FILENO);
		close(fd[0]);
		execl(PCSYSINSTALL, "pc-sysinstall", "-c", "result.cfg", NULL);
		_exit(1);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		break;
	case -1:
		close(fd[0]);
		close(fd[1]);
		break;
	default:
	{
		int i, ret;
		char buf[1];
		char line[1024];


		pfd.fd = fd[1];
		pfd.events = POLLIN;
		i = 0;
		while (!signaled) {
			ret = poll(&pfd, 1, 1000);
			if (ret == 0)
				continue;
			if (ret < 0)
				break;
			/* XXX really optimize this */
			if (read(fd[1], buf, 1) < 0)
				break;
			if (*buf == '\n') {
				line[i] = 0;
				show_progress(line);
				i = 0;
			} else {
				line[i] = *buf;
				i++;
			}
		}
		wait(&status);

	}
	}
	close(fd[0]);
	close(fd[1]);

	if (status != 0)
		dialog_textbox("Error occured",
		    "/tmp/.pc-sysinstall/pc-sysinstall.log", 20, 70);
	else {
		screen_clear(MODULE);
		dialog_mesgbox("Installation finished",
		    "Congratulations, you now have PC-BSD or FreeBSD "
		    "installed\nin your system.\n\n", 7, 70);
	}

	system("/bin/sh");
	exit(0);
}

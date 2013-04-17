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
 * $Id: main.c 7781 2010-10-12 21:04:39Z johnh $
 */

#include <dialog.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define	HASHSZ	1024

#include "txt-sysinstall.h"

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-s style]\n", getprogname());
}

int
main(int argc, char *argv[])
{
	char style[64] = "style.rc";
	int ch;

	while ((ch = getopt(argc, argv, "as:")) != -1) {
		switch (ch) {
		case 's':
			strlcpy(style, optarg, sizeof(style));
			break;
		case '?':
		default:
			usage();
		}
	}

	setenv("DIALOGRC", style, 1);
	sysinstall_init();
	init_dialog();
	hcreate(HASHSZ);

	sysinstall();

	hdestroy();
	end_dialog();
	sysinstall_fini();
}

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
 */

#include <sys/types.h>
#include <sys/queue.h>

#ifdef	__PCBSD__
#define	OSNAME	"PC-BSD"
#define	PKGFUNC	dialog_components
#else
#define	OSNAME	"FreeBSD"
#define	PKGFUNC	dialog_packages
#endif

#define SCRIPTS_PATH	"/usr/sbin/"
#define PCSYSINSTALL	"/usr/sbin/pc-sysinstall"

#define	BUFSZ	1024*1024

#define DMENU(m, p, t, c, s, f, d, a) do { \
	dialogMenuItem *mptr = m;	\
	mptr->prompt = p;	\
	mptr->title  = t;	\
	mptr->checked = c;	\
	mptr->selected = s;	\
	mptr->fire   = f;	\
	mptr->data   = d;	\
	mptr->lbra = '[';	\
	mptr->mark = 'X';	\
	mptr->rbra = ']';	\
	mptr->aux = a;		\
} while (0) 

#define RDMENU(m, p, t, c, s, f, d, a) do { \
	dialogMenuItem *mptr = m;	\
	mptr->prompt = p;	\
	mptr->title  = t;	\
	mptr->checked = c;	\
	mptr->selected = s;	\
	mptr->fire   = f;	\
	mptr->data   = d;	\
	mptr->lbra = '(';	\
	mptr->mark = '*';	\
	mptr->rbra = ')';	\
	mptr->aux = a;		\
} while (0) 

#define	DMENUF(m, p, t, f) \
	DMENU(m, p, t, NULL, NULL, f, NULL, 0)
#define	RDMENUF(m, p, t, f) \
	RDMENU(m, p, t, NULL, NULL, f, NULL, 0)

#define	DMENUFA(m, p, t, f, a) \
	DMENU(m, p, t, NULL, NULL, f, NULL, a)
#define	RDMENUFA(m, p, t, f, a) \
	RDMENU(m, p, t, NULL, NULL, f, NULL, a)

#define	DMENUFDA(m, p, t, f, d, a) \
	DMENU(m, p, t, NULL, NULL, f, d, a)
#define	RDMENUFDA(m, p, t, f, d, a) \
	RDMENU(m, p, t, NULL, NULL, f, d, a)

#define	DMENUFC(m, p, t, f, c) \
	DMENU(m, p, t, c, NULL, f, NULL, 0)
#define	RDMENUFC(m, p, t, f, c) \
	RDMENU(m, p, t, c, NULL, f, NULL, 0)

#define	DMENUFCD(m, p, t, f, c, d) \
	DMENU(m, p, t, c, NULL, f, d, 0)
#define	RDMENUFCD(m, p, t, f, c, d) \
	RDMENU(m, p, t, c, NULL, f, d, 0)

#define	DMENUFD(m, p, t, f, d) \
	DMENU(m, p, t, NULL, NULL, f, d, 0)
#define	RDMENUFD(m, p, t, f, d) \
	RDMENU(m, p, t, NULL, NULL, f, d, 0)

#define	DMENUS(m, p, t, s) \
	DMENU(m, p, t, NULL, s, NULL, NULL, 0)
#define	RDMENUS(m, p, t, s) \
	RDMENU(m, p, t, NULL, s, NULL, NULL, 0)

#define	notnull(x)	(x != NULL && x[0] != 0)

struct disk_info {
	char *disk;
	char *desc;
	size_t ncyl;
	size_t nhead;
	size_t nsect;
	size_t size;
	char *type;
	int used;
	STAILQ_ENTRY(disk_info) entries;
};

struct disk_list {
	size_t ndisks;
	STAILQ_HEAD(diskhead, disk_info) list;
};

#ifdef	DEBUG
#define	DISK_ARGS	"-m"
#else
#define	DISK_ARGS	NULL
#endif

/* dialogs */
void		sysinstall(void);
int			dialog_sysinstall(void);
int			dialog_disksel(void);
int			dialog_useradd(void);
int			dialog_netif(void);
int			dialog_rootpass(void);
int			dialog_partsel(void);
int			dialog_tzone(void);
int			dialog_components(void);
int			dialog_medium(void);
int			dialog_label(void);
int			dialog_ftp(void);
int			dialog_packages(void);
int			dialog_system(void);
void		dialog_busy(int, int, const char *);
void		do_install(void);

void		dialog_bootloader(void);
void		dialog_mirror(void);
void		dialog_partscheme(void);

/* utility functions */
void		sysinstall_init(void);
void		sysinstall_fini(void);
void		screen_clear(const char *);
int			empty(const char *);		
void *		safe_malloc(size_t);
size_t		safe_strlen(const char *);
char *		safe_strdup(const char *);
int			run_pcsysinstall(char *, size_t, const char *, const char *);
void		appendconfig(const char *, const char *);
const char 	*getconfigval(const char *);
void		save_config(void);
int			b2mb(int64_t *, int64_t *);

struct disk_info *get_disk_info(const char *, const char *);
struct disk_info *get_disk_info_by_disk(const char *);
void		free_disk_info(void *);

struct disk_info *set_current_disk(struct disk_info *);
struct disk_info *get_current_disk(void);

struct disk_list *get_disk_list(void);
void	free_disk_list(void);

#define installType(x)          appendconfig("installType", x)
#define installMode(x)          appendconfig("installMode", x)
#define installMedium(x)        appendconfig("installMedium", x)

#define installComponents(x)    appendconfig("installComponents", x)
#define installPackages(x)      appendconfig("installPackages", x)

#define packageType(x)          appendconfig("packageType", x)

#define rootPass(x)             appendconfig("rootPass", x)
#define encpass(x)				appendconfig("encpass", x)

#define userName(x)             appendconfig("userName", x)
#define userComment(x)          appendconfig("userComment", x)
#define userPass(x)             appendconfig("userPass", x)
#define userGroups(x)           appendconfig("userGroups", x)
#define userShell(x)            appendconfig("userShell", x)
#define autoLoginUser(x)        appendconfig("autoLoginUser", x)
#define commitUser()            appendconfig("commitUser", NULL)

#define partition(x)            appendconfig("partition", x)
#define bootManager(x)          appendconfig("bootManager", x)
#define partscheme(x)           appendconfig("partscheme", x)

#define mirror(x)               appendconfig("mirror", x)
#define mirrorbal(x)            appendconfig("mirrorbal", x)

#define ftpPath(x)              appendconfig("ftpPath", x)

#define timeZone(x)             appendconfig("timeZone", x) 

#define enableNTP(x)            appendconfig("enableNTP", x)

#define disk(n,x) \
do { \
    char dp[16]; \
    snprintf(dp, sizeof(dp), "disk%d-part", n); \
    appendconfig(dp, x); \
} while (0)

#define commitDiskPart()        appendconfig("commitDiskPart", NULL)
#define commitDiskLabel()       appendconfig("commitDiskLabel", NULL)

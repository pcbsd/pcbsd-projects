# $Id: Makefile 7782 2010-10-12 22:32:13Z johnh $

PROG=	txt-sysinstall
SRCS=	main.c mainmenu.c disksel.c partsel.c useradd.c netif.c rootpass.c
SRCS+=	tzone.c components.c util.c install.c medium.c label.c ftp.c
SRCS+=	packages.c system.c

CFLAGS+=	-ggdb -DDEBUG 

WARNS=	3

DPADD=	${LIBDIALOG} ${LIBUTIL}
LDADD=	-ldialog -lutil
NO_MAN=

.include <bsd.prog.mk>

#!/bin/sh

/sbin/ifconfig $1 inet6 -ifdisabled 2>&1 &&
/sbin/ifconfig $1 inet6 accept_rtadv 2>&1 &&
/sbin/rtsol $1 2>&1
if [ $? -ne 0 ]; then
	return
fi
l=`/sbin/ifconfig $1 | grep inet6 | grep -v 'inet6 fe80'`
if [ $? -eq 0 ]; then
	echo $l
else
	echo No IPv6 router found.
fi

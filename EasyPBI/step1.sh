#!/bin/sh

#Quick script to automate the conversion of the EasyPBI sources into a 
#  distibution file so that the port can be updated for each version

LOCALSRCDIR=~/Projects/pcbsd/current/src-qt4/EasyPBI

echo "Starting the packaging process for the EasyPBI sources"
echo "  - Current local source directory: ${LOCALSRCDIR}"
#copy the sources over to this directory
mkdir EasyPBI-source
cp -R ${LOCALSRCDIR}/* EasyPBI-source/.
#Clean up any compiled files from the dir
cd EasyPBI-source
make clean
rm Makefile
rm EasyPBI
rm i18n/*.qm

echo "Please look over the EasyPBI-source directory to make sure that no compiled files still remain"
echo "Then run step2.sh to finish up the process"

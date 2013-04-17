#!/bin/sh
#        Author: Kris Moore
#   Description: Creates a vbox / vmware disk image
#     Copyright: 2011 PC-BSD Software / iXsystems
#       License: BSD
############################################################################
# Please read through the User options section, setting them for your images
#
# This script will normally only run on amd64 systems, to create a i386
# image, simply point the script to a i386 PC-BSD 'netinstall' location
# and the resulting image will be installed with a 32bit system
############################################################################
#
# User options

# Set to directory where you want temp files to be created, needs lots of space
WRKDIR="/usr/local/tmp"

# Set to directory where you want to save finished images
OUTDIR="/usr/local/tmp"

# Set the size in MB of the images
IMAGESIZE="10000"

# VFS FILES
MFSFILE="${WRKDIR}/PCBSD.ufs"
VDIFILE="${OUTDIR}/PCBSD-VBOX.vdi"
VMDKFILE="${OUTDIR}/PCBSD-VMWARE.vmdk"

# Dist files location, set to LOCAL / NETWORK
# If set to LOCAL will use pc-sysinstall.cfg.local, else pc-sysinstall.cfg.net
DLOC="NETWORK"

# If DLOC = LOCAL, set this to the location of the netinstall/ folder from a PC-BSD distribution
DDIR="/usr/local/tmp/netinstall"

# Set this to the mirror location you want to install from if DLOC = NETWORK
# Refer to the PC-BSD website for a list of public mirrors
DNET="ftp://mirrors.isc.org/pub/pcbsd/9.0-RC2/amd64/netinstall"

# Set this to the meta-pkgs you want to install
# For a list of what is available for this distribution, extract the xtra-data.tbz file in netinstall/
# and look at the available directories in base-ports/
MPKGS="base-system,KDE,KDE-Accessibility,KDE-Artwork,KDE-Education,KDE-Games,KDE-Graphics,KDE-Multimedia,KDE-Network,KDE-PIM,LXDE,XFCE,GNOME,VirtualBoxGuest"

#######################################################################################
# Lets start the creation now

if [ ! -d ${WRKDIR} ] ; then
	mkdir -p ${WRKDIR}
fi
if [ ! -d ${OUTDIR} ] ; then
	mkdir -p ${OUTDIR}
fi

# Create the filesystem backend file
echo "Creating memory file system... This may take a while..."
dd if=/dev/zero of=${MFSFILE} bs=1m count=${IMAGESIZE}
MD="`mdconfig -a -t vnode -f ${MFSFILE}`"
export MD

# Copy the pc-sysinstall config
if [ "$DLOC" = "NETWORK" ] ; then
  cp pc-sysinstall.cfg.net pc-sysinstall.cfg.$$
  sed -i '' "s|%%FTPPATH%%|${DNET}|g" pc-sysinstall.cfg.$$
  sed -i '' "s|%%PKGLOC%%|${DNET}/packages/|g" pc-sysinstall.cfg.$$
else
  cp pc-sysinstall.cfg.local pc-sysinstall.cfg.$$
  sed -i '' "s|%%LOCALPATH%%|${DDIR}|g" pc-sysinstall.cfg.$$
  sed -i '' "s|%%PKGLOC%%|${DDIR}/packages|g" pc-sysinstall.cfg.$$
fi

# Set the MD device name we are working on
sed -i '' "s|%%DISKDEV%%|${MD}|g" pc-sysinstall.cfg.$$

# Set the meta-packages we will be installing
sed -i '' "s|%%MPKGS%%|${MPKGS}|g" pc-sysinstall.cfg.$$

# Set the path of anything which needs it
PWD=`pwd`
sed -i '' "s|%%PWD%%|${PWD}|g" pc-sysinstall.cfg.$$

# Run pc-sysinstall
pc-sysinstall -c pc-sysinstall.cfg.$$

# Cleanup the config file
rm pc-sysinstall.cfg.$$

# Unmount the MD device
sleep 30
sync
mdconfig -d -u ${MD}
sleep 30
sync

# Create the disk images from the raw file now
rm ${VDIFILE} 2>/dev/null
VBoxManage convertfromraw --format VDI ${MFSFILE} ${VDIFILE}
rm ${VDIFILE}.bz2 2>/dev/null
bzip2 ${VDIFILE}
chmod 644 ${VDIFILE}.bz2

# Do VMWARE now
rm ${VMDKFILE} 2>/dev/null
VBoxManage convertfromraw --format VMDK ${MFSFILE} ${VMDKFILE}
rm ${VMDKFILE}.bz2 2>/dev/null
bzip2 ${VMDKFILE}
chmod 644 ${VMDKFILE}.bz2

# Run MD5 command
md5 -q ${VDIFILE}.bz2 >${VDIFILE}.md5
sha256 -q ${VDIFILE}.bz2 >${VDIFILE}.sha256
md5 -q ${VMDKFILE}.bz2 >${VMDKFILE}.md5
sha256 -q ${VMDKFILE}.bz2 >${VMDKFILE}.sha256

rm $MFSFILE

#!/bin/sh

#Script for approving a PBI on a repo server
# This will eventually become a single function in a daemon that keeps track of a build repo


#These Variables will be set a different way later (either input options, or global variables)
PBIRT_BUILDTREE=/path/to/PBI/autobuild/tree #Directory tree for created PBI's
PBIRT_DISTTREE=/path/to/PBI/distribution/tree #Directory tree for approved PBI's
PBIRT_DISTURLBASE=/base/distribution/URL #URL base for the distribution tree
PBIRT_INDEXFILE=/path/to/PBI/indexfile #PBI index file location
PBIRT_KEEPOLD=1; #Total number of older PBI version's to keep for a particular program


#Loop over all inputs
for PBI in $@
do

  #Get the full file path on the build tree
  BPATH=
  
  #Get the directory to place the file in the distribution tree
  DPATH=
  
  #Copy the file over
  cp ${PBIRT_BUILDTREE}/${BPATH}/${PBI} ${PBIRT_DISTTREE}/${DPATH}/${PBI}
  
  #clean up the old file
  rm ${PBIRT_DISTTREE}/*.pbp #remove all old patches
  NUM=0
  OLDVERS="" #keep track of the version numbers for the PBI's we are keeping
  for i in `ls -t ${PBIRT_DISTTREE}/${DPATH}/*.pbi`; do
    if [ ${NUM} .ge. ${PBIRT_PBIKEEP} ]; then
      rm ${i}
    else
      #Get the version that we are keeping
      VER=`pbi_add -i ${i} | grep Version | cut -d : -f 2 | tr -d " "`
      OLDVERS=${OLDVERS},${VER}
        #copy over any .pbp files that match
      for j in `ls ${PBIRT_BUILDTREE}/${BPATH}/*${VER}*.pbp`; do
        cp ${j} ${PBIRT_DISTTREE}/${DPATH}/.
      done
    fi
    NUM=${NUM}+1
  done

  
  #Now add the PBI to the index
  pbi_indextool add -b ${OLDVERS} -f ${PBI} -k ${PBIRT_KEEPOLD} -u ${PBIRT_DISTURLBASE}/${DPATH}/${PBI} ${PBIRT_INDEXFILE}

done

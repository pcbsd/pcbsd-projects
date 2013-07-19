#!/bin/sh

#This is the daemon portion of the pbi_repotools suite
#Make sure to adjust the parameters here at the top of the script to fit it to your repository structure

setparams() {
  #Local Repo Paths
  PBIRT_DISTBASE=/path/to/distibution/tree/base
  PBIRT_DISTURLBASE=/base/distribution/tree/url #for webserver distribution via the pbi index
  PBIRT_BUILDBASE=/path/to/build/tree/base
  PBIRT_INDEXFILE=/path/to/repo/index.file
  PBIRT_METAFILE=/path/to/repo/meta.file
  
  # == VERSION CONTROL SETTINGS FOR INDEX/META FILES ==
  PBIRT_USEGIT="NO" # [YES/NO] - auto-commit index/meta files to GIT repository
    #Setting "Yes" to GIT assumes that you already have your global git settings configured (username,password, remote repo)
  PBIRT_USESVN="NO"  # [YES/NO] - auto-commit index/meta files to SVN repository
  if [ ${PBIRT_USESVN} == "YES" ]; then
    SVNUSER="myusername"
    SVNPASS="mypassword"
  fi
  PBIRT_VCSDIR=/path/to/version/controlled/dir #directory that the index/meta files is in works best
  PBIRT_PBIVERSION="9"

  # == DAEMON REFRESH TIME ==
  PBIRT_CHECKINTERVAL="30" #Time in minutes to check for new jobs
}

# ===== VCS UPDATE INDEX/META FILES =====
updateIndexMetaFiles() {
  #If using GIT or SVN, update the index
  if [ ${PBIRT_USEGIT} == "YES" ]; then
    (cd ${PBIRT_VCSDIR}; git pull --rebase)  #need to verify this cmd
  fi
  if [ ${PBIRT_USESVN} == "YES" ]; then
    svn update ${PBIRT_VCSDIR}
  fi
}

# ===== VCS COMMIT INDEX FILE =====
commitIndex() {
  #If using GIT or SVN, commit the index file
  #Input: #{COMMITMSG}
  if [ ${PBIRT_USEGIT} == "YES" ]; then
    ( cd ${PBIRT_VCSDIR}; git add ${PBIRT_INDEXFILE}; git commit -m ${COMMITMSG}; git push origin master )  #need to verify this cmd
  fi
  if [ ${PBIRT_USESVN} == "YES" ]; then
    svn commit -m ${COMMITMSG} --username ${SVNUSER} --password ${SVNPASS} ${PBIRT_INDEXFILE}
  fi
  unset COMMITMSG
}

# ===== VCS COMMIT META FIle =====
commitMeta() {
  #If using GIT or SVN, commit the meta file
  #Input: #{COMMITMSG}
  if [ ${PBIRT_USEGIT} == "YES" ]; then
    (cd ${PBIRT_VCSDIR}; git add ${PBIRT_METAFILE}; git commit -m ${COMMITMSG}; git push origin master)  #need to verify this cmd
  fi
  if [ ${PBIRT_USESVN} == "YES" ]; then
    svn commit -m ${COMMITMSG} --username ${SVNUSER} --password ${SVNPASS} ${PBIRT_METAFILE}
  fi
  unset COMMITMSG
}

# ===== PBI APPROVAL SCRIPT =====
approvepbi() {
  #Inputs: $PBI and $KEEPVERNUM

  #Get the full file path on the build tree
  BPATH=`find ${PBIRT_BUILDBASE} | grep ${PBI}`
  
  #Get the directory to place the file in the distribution tree
  DPATH=
  
  #Get the PBI info
  info=`pbi_add -i ${PBIRT_BUILDBASE}/${BPATH}/${PBI}`
  cVer=`echo ${info} | grep Version | cut -d : -f 2 | tr -d " "`
  cName=`echo ${info} | grep Name | cut -d : -f 2-10`
  cArch=`echo ${info} | grep Arch | cut -d : -f 2 | tr -d " "`
  unset info
  
  #Copy the file over
  cp ${PBIRT_BUILDBASE}/${BPATH}/${PBI} ${PBIRT_DISTTREE}/${DPATH}/${PBI}
  
  #clean up the old file
  rm ${PBIRT_DISTBASE}/${DPATH}/*.pbp #remove all old patches
  NUM=0
  OLDVERS="" #keep track of the version numbers for the PBI's we are keeping
  for i in `ls -t ${PBIRT_DISTTREE}/${DPATH}/*.pbi`; do
    if [ ${NUM} .ge. ${KEEPVERNUM} ]; then
      rm ${i}
    else
      #Get the version that we are keeping
      VER=`pbi_add -i ${i} | grep Version | cut -d : -f 2 | tr -d " "`
      OLDVERS=${OLDVERS},${VER}
        #copy over any .pbp files that match
      for j in `ls ${PBIRT_BUILDBASE}/${BPATH}/*${VER}*.pbp`; do
        cp ${j} ${PBIRT_DISTTREE}/${DPATH}/.
      done
    fi
    NUM=${NUM}+1
  done

  updateIndexMetaFiles
  #Now add the PBI to the index
  pbi_indextool add -b ${OLDVERS} -f ${PBI} -k ${PBIRT_KEEPOLD} -u ${PBIRT_DISTURLBASE}/${DPATH}/${PBI} ${PBIRT_INDEXFILE}
  #Now commit the index file
  COMMITMSG="PBI-${PBIRT_PBIVERSION} Approval: ${cName} ${cVer} (${cArch})"
  commitIndex
  
  #Now clean up the variables to prevent contamination (and memory hogging)
  unset BPATH DPATH NUM OLDVERS i j cVer cName cArch PBI KEEPVERNUM
}

#=======================================
#======= MAIN PROCESS FUNCTION =========
#=======================================
PENDINGFILE="./PENDINGCHANGES"
RUNNINGFILE="./RUNNING"
STOPFILE="./STOPDAEMON"
ERRORFILE="./ERRORS"
#Make sure that we do not start up two of these concurrently or if there are errors logged
if [ ! -e ${RUNNINGFILE} ] && [ ! -e ${ERRORFILE} ]; then 
  #Remove any leftover stop file if it exists
  if [ -e ${STOPFILE} ]; then
    rm ${STOPFILE}
  fi
  while [ -e ${PENDINGFILE} ] && [ ! -e ${STOPFILE} ]
  do
    unset line # make sure this is empty to start off
    contents=`cat ${PENDINGFILE}`
    rm ${PENDINGFILE}
    for i in ${contents}
    do
      if [ -z ${line} ]; then
        line=${i};
        echo ${i} > ${RUNNINGFILE}
      else
        echo ${i} >> ${PENDINGFILE}
      fi
    done
    unset contents #save memory during the run
  
    #Run the appropriate subroutine
    echo "Action Requested: ${line}"
    sleep 1

  done
  #Clean up the display files
  rm ${RUNNINGFILE}
  if [ -e ${STOPFILE} ]; then
    rm ${STOPFILE}
  fi
fi



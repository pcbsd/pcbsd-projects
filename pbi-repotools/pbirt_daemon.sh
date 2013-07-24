#!/bin/sh

#This is the daemon portion of the pbi_repotools suite
#Make sure to adjust the parameters here at the top of the script to fit it to your repository structure

setparams() {
  #Local Repo Paths
  PBIRT_DISTBASE=/path/to/distibution/tree/base
  PBIRT_DISTURLBASE=/base/distribution/tree/url #for webserver distribution via the pbi index
  PBIRT_BUILDBASE=/path/to/build/tree/base
  PBIRT_BUILDURLBASE=/base/build/tree/download/url
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
    (cd ${PBIRT_VCSDIR}; git pull)  #need to verify this cmd
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

# ===== VCS COMMIT META FILE =====
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

# ===== GET PBI BUILD/DIST CATEGORIES =====
getBuildTreePaths() {  #Search the build tree for the PBI
#inputs: $PBI
#outputs: $BPATH

  #Get the full file path on the build tree
  BPATH=`find ${PBIRT_BUILDBASE} | grep ${PBI} | tr -d "${PBIRT_BUILDBASE}/" | tr -d "/${PBI}"`
  
  #Make sure the same directory path exists in the distribution tree
  if [ ! -e ${PBIRT_DISTBASE}/${BPATH} ]; then
    echo "Creating ditribution directory: ${BPATH}"
    mkdir -p "${PBIRT_DISTBASE}/${BPATH}"
  fi
}


getDistTreePaths() { #Search the Dist tree for the PBI
#inputs: $PBI
#outputs: $BPATH

  #Get the full file path on the build tree
  BPATH=`find ${PBIRT_DISTBASE} | grep ${PBI} | tr -d "${PBIRT_DISTBASE}/" | tr -d "/${PBI}"`
  

}

# ===== PBI APPROVAL SCRIPT =====
# (move a PBI from the build server over to the distibution tree and update the repo files)
approvepbi() {
  #Inputs: $PBI and $KEEPVERNUM (integer)

  getBuildTreePaths
  
  #Get the PBI info
  info=`pbi_add -i ${PBIRT_BUILDBASE}/${BPATH}/${PBI}`
  cVer=`echo "${info}" | grep Version | cut -d : -f 2 | tr -d " "`
  cName=`echo "${info}" | grep Name | cut -d : -f 2-10`
  cArch=`echo "${info}" | grep Arch | cut -d : -f 2 | tr -d " "`
  unset info
  
  #Remove any leftover PBI's with the same version number
  # This is in case the PBI was not removed from the repo entirely, but just had the index entry removed
  for i in `ls ${PBIRT_DISTBASE}/${BPATH}/*.pbi | grep ${cVer}`
  do
    rm ${i}
  done
  #Copy the PBI over
  cp ${PBIRT_BUILDBASE}/${BPATH}/${PBI} ${PBIRT_DISTBASE}/${BPATH}/${PBI}
  
  #Clean up the PBP's
  rm ${PBIRT_DISTBASE}/${BPATH}/*.pbp #remove all old patches
  for j in `ls ${PBIRT_BUILDBASE}/${BPATH}/*.pbp | grep ${cVer}`; do
    cp ${j} ${PBIRT_DISTBASE}/${BPATH}/. #copy over any .pbp's for the approved version
  done
  
  #clean up the old PBI's
  declare -i NUM #Make sure NUM is an integer
  NUM=0
  OLDVERS="" #keep track of the version numbers for the PBI's we are keeping
  for i in `ls -t ${PBIRT_DISTBASE}/${DPATH}/*.pbi`; do
    if [ ${NUM} -ge ${KEEPVERNUM} ]; then
      rm ${i}
    else
      #Get the version that we are keeping and add it to the list
      VER=`pbi_add -i ${i} | grep Version | cut -d : -f 2 | tr -d " "`
      OLDVERS=${OLDVERS},${VER}
    fi
    NUM=${NUM}+1
  done

  updateIndexMetaFiles
  #Now add the PBI to the index
  pbi_indextool add -b ${OLDVERS} -f ${PBI} -k ${PBIRT_KEEPOLD} -u ${PBIRT_DISTURLBASE}/${BPATH}/${PBI} ${PBIRT_INDEXFILE}
  #Now commit the index file
  COMMITMSG="PBI-${PBIRT_PBIVERSION} Approval: ${cName} ${cVer} (${cArch})"
  commitIndex
  
  #Now clean up the variables to prevent contamination (and memory hogging)
  unset BPATH NUM OLDVERS VER i j cVer cName cArch PBI KEEPVERNUM
}

# ===== PBI DISAPPROVAL SCRIPT =====
# (Remove a PBI that was previously approved)
disapprovepbi() {
  #Inputs: $PBI
  
  #Get the path to the PBI
  getDistTreePaths
  #Get the PBI info
  info=`pbi_add -i ${PBIRT_DISTBASE}/${BPATH}/${PBI}`
  cVer=`echo "${info}" | grep Version | cut -d : -f 2 | tr -d " "`
  cName=`echo "${info}" | grep Name | cut -d : -f 2-10`
  cArch=`echo "${info}" | grep Arch | cut -d : -f 2 | tr -d " "`
  unset info
  #Remove the PBI and any PBP's for it from the distibution tree
  rm ${PBIRT_DISTBASE}/${BPATH}/${PBI}
  for i in `ls ${PBIRT_DISTBASE}/${BPATH}/*.pbp | grep ${cVer}`
  do
    rm ${i}
  done
  #Remove the PBI from the index
  updateIndexMetaFiles
  pbi_indextool rem -m cArch -n cName -v cVer
  commitIndex
  #Clean up
  unset cArch cName cVer BPATH PBI
}

# ===== REMOVE FLAGS =====
removeFlags() {
  #Inputs: $BPATH
  if [ -e ${PBIRT_BUILDBASE}/${BPATH}/${BROKENFLAG} ]; then
    rm ${PBIRT_BUILDBASE}/${BPATH}/${BROKENFLAG}
  fi
  if [ -e ${PBIRT_BUILDBASE}/${BPATH}/${FIXEDFLAG} ]; then
    rm ${PBIRT_BUILDBASE}/${BPATH}/${FIXEDFLAG}
  fi
}

# ===== FLAG PBI AS BROKEN =====
flagBroken() {
  #Inputs: $PBI
  getBuildTreePaths
  #Get the current version of the PBI
  info=`pbi_add -i ${PBIRT_BUILDBASE}/${BPATH}/${PBI}`
  cVer=`echo "${info}" | grep Version | cut -d : -f 2 | tr -d " "`
  cArch=`echo "${info}" | grep Arch | cut -d : -f 2 | tr -d " "`
  unset info
  #Now set the flag in the build directory
  removeFlags
  echo "${cVer}" >> ${PBIRT_BUILDBASE}/${BPATH}/${BROKENFLAG}
  #Clean up
  unset cVer cArch BPATH PBI
}

# ===== FLAG PBI AS FIXED =====
flagFixed() {
  #Inputs: $PBI
  getBuildTreePaths
  #Get the current version of the PBI
  info=`pbi_add -i ${PBIRT_BUILDBASE}/${BPATH}/${PBI}`
  cVer=`echo "${info}" | grep Version | cut -d : -f 2 | tr -d " "`
  cArch=`echo "${info}" | grep Arch | cut -d : -f 2 | tr -d " "`
  unset info
  #Now set the flag in the build directory
  removeFlags
  echo "${cVer}" >> ${PBIRT_BUILDBASE}/${BPATH}/${FIXEDFLAG}
  #Clean up
  unset cVer cArch BPATH PBI
}

# ===== BUILD A TEXT FILE OF AVAILABLE PBI's =====
buildAvailablePBIList() {
  #Clear the Current Lists
  rm ${ALIST} ${CLIST}
  #Load the Index file
  updateIndexMetaFiles
  INDEX=`cat ${PBIRT_INDEXFILE}`
  #Traverse the Build Tree looking for any PBI's
  for i in `find ${PBIRT_BUILDBASE} -name "*.pbi" -print`
  do
    #Get the PBI
    PBI=`basename "${i}"`
    #Get the base directory
    BPATH=`echo "${i}" |  tr -d "${PBIRT_BUILDBASE}/" | tr -d "/${PBI}"`
    #Get the PBI info
    info=`pbi_add -i ${PBIRT_DISTBASE}/${BPATH}/${PBI}`
    cVer=`echo "${info}" | grep Version | cut -d : -f 2 | tr -d " "`
    cName=`echo "${info}" | grep Name | cut -d : -f 2-10 | tr '[:upper:]' '[:lower:]' | tr -d " "`  #Make sure it is all lower case
    cArch=`echo "${info}" | grep Arch | cut -d : -f 2 | tr -d " "`
    unset info
    #Check the index to see if it is current
    if [ -n `echo ${INDEX} | grep "${cName}:${cArch}:${cVer}"` ]; then
      #Is already in the index
      continue
    else if [ -n `grep "approve::${PBI}" ${PENDINGFILE}` ]; then
      #Has operations Pending
      continue
    fi
    #Get any current flags
    if [ -e ${PBIRT_BUILDBASE}/${BPATH}/${BROKENFLAG}] && [ -n `find ${PBIRT_BUILDBASE}/${BPATH} -name ${PBI} -newer ${PBIRT_BUILDBASE}/${BPATH}/${BROKENFLAG} -print` ]; then
      FLAG="BROKEN"
    else if [ -e ${PBIRT_BUILDBASE}/${BPATH}/${FIXEDFLAG}] && [ -n `find ${PBIRT_BUILDBASE}/${BPATH} -name ${PBI} -newer ${PBIRT_BUILDBASE}/${BPATH}/${FIXEDFLAG} -print` ]; then
      FLAG="FIXED"
    else 
      FLAG="NEW"
      removeFlags
    fi
    #Assign a type to it based upon current distribution
    OLDPBI=`ls -t ${PBIRT_DISTBASE}/${BPATH}/*.pbi`
    if [ -n ${OLDPBI} ]; then
      OLDPBI=`echo "$OLDPBI" | cut -f 1` 
      if [ `pbi_add -i ${OLDPBI} | grep Version | cut -d : -f 2 | tr -d  " " | cut -d _ -f 1` == `echo "${cVer}" | cut -d _ -f 1` ]; then
        TYPE="REVISION"
      else
        TYPE="UPDATE"
      fi
    else
      TYPE="NEW"  #This is a new application for the repo
    fi
    #Get the creation time for the PBI file
    TIME=`ls -al -D %x ${PBIRT_BUILDBASE}/${BPATH}/${PBI} | awk '{print $6}'`
    #Save the file to the available PBI list
    echo ${TYPE}::${cName}::${cArch}::${cVer}::${TIME}::${FLAG}::${PBIRT_BUILDURLBASE}/${BPATH}/${PBI}>> ${ALIST}
  done
  
  #Clean up
  unset INDEX PBI BPATH cVer cName cArch FLAG OLDPBI TYPE TIME i
}

#=======================================
#======= MAIN PROCESS FUNCTION =========
#=======================================
PENDINGFILE="./PENDINGCHANGES"
RUNNINGFILE="./RUNNING"
STOPFILE="./STOPDAEMON"
ERRORFILE="./ERRORS"
#FLAG FILENAMES
BROKENFLAG="BROKEN.flag"
FIXEDFLAG="FIXED.flag"
#LIST FILENAMES
ALIST="./AVAILABLEPBI"
CLIST="./CURRENTPBI"

if [ ${1} == "-update" ] || [ ! -e ${ALIST} ]; then
  buildAvailablePBIList 
fi
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
    ACTION=`echo ${line} | cut -d :: -f 1`
    PBI=`echo ${line} | cut -d :: -f 2`
    if [ "${ACTION}" == "approve" ]; then
      declare -i KEEPVERNUM
      KEEPVERNUM=`echo ${line} | cut -d :: -f 3`
      approvepbi
    else if [ "${ACTION}" == "disapprove" ]; then
      disapprovepbi
    else if [ "${ACTION}" == "broken" ]; then
      flagBroken
    else if [ "${ACTION}" == "fixed" ]; then
      flagFixed
    else if [ "${ACTION}" == "clear" ]; then
      getBuildTreePaths
      removeFlags
      unset PBI BPATH
    else
     #invalid action - go to the next one
      continue
    fi
    sleep 1
    buildAvailablePBIList  #Rebuild the lists of available PBI's after each action
  done
  #Clean up the display files
  rm ${RUNNINGFILE}
  if [ -e ${STOPFILE} ]; then
    rm ${STOPFILE}
  fi
fi



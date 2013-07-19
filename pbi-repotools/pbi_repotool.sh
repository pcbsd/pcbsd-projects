#!/bin/sh

#Tool for easy modification and management of a PBI repository


help() {
  #Show the help info
  echo "pbi_repotool: The easy way to manage a PBI repository"
  echo " This function allows a user to queue up actions for a backend daemon to perform them on a repository."
  echo " If there was an error in the backend approval, the file ${ERRORS} will contain the relevant information."
  echo " The 'restart' command will need to be issued to manually restart the backend if an error file exists."
  echo " "
  echo "  Usage: pbi_repotool [action] <pbi-file> [--keep <number>]"
  echo "  actions:"
  echo "    add <num> -- Add the PBI file to the repository, keeping <num> older version (default: 1)"
  echo "    remove    -- Remove the PBI file from the repository"
  echo "    bad       -- Flag the PBI as tested and broken"
  echo "    fixed     -- Flag the PBI as tested and fixed"
  echo " "
  echo " Special actions: (no PBI file required)"
  echo "    restart   -- Remove any error logs and startup the backend daemon"
  echo "    stop      -- Stop the backend daemon process from proceeding to the next item"

  echo " "
}

addPBI() {
  echo "approve::${PBI}::${KEEPOLD}" >> ${WAITFILE}
  echo "${PBI} is now in the approval queue (keep ${KEEPOLD} older versions)"
}

rmPBI() {
  echo "disapprove::${PBI}" >> ${WAITFILE}
  echo "${PBI} is now pending removal from the repository"
}

badPBI() {
  #might bypass the WAITFILE and make the change to the main list directly later
  echo "broken::${PBI}" >> ${WAITFILE}
  echo "${PBI} has been marked as broken"
}

fixedPBI() {
  #might bypass the WAITFILE and make the change to the main list directly later
  echo "fixed::${PBI}" >> ${WAITFILE}
  echo "${PBI} has been marked as fixed"
}

#Check for inputs, and display man page otherwise
if [ $1 ]; then
  WAITFILE="./PENDINGCHANGES"
  RUNNINGFILE="./RUNNING"
  STOPFILE="./STOPDAEMON"
  ERRORFILE="./ERRORS"
  
  if [ "${1}" == "stop" ]; then
    touch ${STOPFILE}
  else if[ "${1}" == "restart" ]; then
    rm ${ERRORFILE}
    STARTDAEMON="YES"
  else if [ ! $2 ]; then
    echo "Missing PBI argument:"
    help
    
  else
    #Good input arguments
    
    if [ ! -e ${WAITFILE} ] && [ ! -e ${RUNNINGFILE} ]; then
      #Nothing running at the moment, be sure to start up the daemon
      STARTDAEMON="YES"
    else
      STARTDAEMON="NO"
    fi
    PBI=${2}
    case "$1" in
     add)
         if [ $3 ]; then
           KEEPOLD=${3}
           addPBI
         else
           KEEPOLD=1
           addPBI
         fi
     ;;
     remove)
         rmPBI
     ;;
     bad)
         badPBI
     ;;
     fixed)
         fixedPBI
     ;;
     *)
       echo "Invalid command: ${1}"
       help
     ;;
    esac
  fi
  
else
  help
fi

#Startup the daemon process if necessary
if [ ${STARTDAEMON} == "YES" ]; then
  ./pbirt_daemon.sh &
fi


read -r line<${WAITFILE}
echo ${line}

exit 0


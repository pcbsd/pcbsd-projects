#!/bin/sh

#Tool for easy modification and management of a PBI repository


help() {
  #Show the help info
  echo "pbi_repotool: The easy way to manage a PBI repository"
  echo "  Usage: pbi_repotool [action] <pbi-file> [--keep <number>]"
  echo "  actions:"
  echo "    add      -- Add the PBI file to the repository"
  echo "    remove   -- Remove the PBI file from the repository"
  echo "    bad      -- Flag the PBI as tested and broken"
  echo "    fixed    -- Flag the PBI as tested and fixed"
  echo " --keep <number>"
  echo "     For adding a PBI, keep <number> older versions in the repository (default: 1)"
  echo " "
}

addPBI() {
  echo "approve ${PBI} ${KEEPOLD}" >> ${WAITFILE}
  echo "${PBI} is now in the approval queue (keep ${KEEPOLD} older versions)"
}

rmPBI() {
  echo "disapprove ${PBI}" >> ${WAITFILE}
  echo "${PBI} is now pending removal from the repository"
}

badPBI() {
  #might bypass the WAITFILE and make the change to the main list directly later
  echo "broken ${PBI}" >> ${WAITFILE}
  echo "${PBI} has been marked as broken"
}

fixedPBI() {
  #might bypass the WAITFILE and make the change to the main list directly later
  echo "fixed ${PBI}" >> ${WAITFILE}
  echo "${PBI} has been marked as fixed"
}

#Check for inputs, and display man page otherwise
if [ $1 ]; then
  WAITFILE=./PENDINGCHANGES
  if [ ! $2 ]; then
    echo "Missing PBI argument:"
    help
    
  else
    #Good input arguments
    PBI=${2}
    case "$1" in
     add)
         if [ $3 ] && [ $4 ] && [ "$3" == "--keep" ]; then
           KEEPOLD=${4}
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

exit 0


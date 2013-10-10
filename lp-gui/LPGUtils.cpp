#include "LPGUtils.h"

LPDataset LPGUtils::loadPoolData(QString zpool){
  //Load the current information for the given zpool
  qDebug() << "New Dataset: " << zpool;
  LPDataset DSC;
  //List all the mountpoints in this dataset
  QStringList subsets = LPBackend::listDatasetSubsets(zpool);
  QStringList lpsnaps = LPBackend::listLPSnapshots(zpool);
  //populate the list of snapshots available for each mountpoint
  for(int i=0; i<subsets.length(); i++){
    //qDebug() << "Subset:" << subsets[i];
    QStringList snaps = LPBackend::listSnapshots(subsets[i]);
    //qDebug() << " - Snapshots:" << snaps;
    if(snaps.isEmpty()){
      //invalid subset - remove it from the list
      subsets.removeAt(i);
      i--;
    }else{
      QStringList subsnaps;
      //only list the valid snapshots that life preserver created
      for(int s=0; s<lpsnaps.length(); s++){
	int index = snaps.indexOf(lpsnaps[s]);
        if(index > -1){ subsnaps << lpsnaps[s]; snaps.removeAt(index); }
      }
      //Now list all the other available snapshots (no certain ordering)
      if(!snaps.isEmpty()){
	subsnaps << "--"; //so we know that this is a divider between the sections
	subsnaps << snaps;
      }
      DSC.subsetHash.insert(subsets[i],subsnaps); //add it to the internal container hash
    }
  }
  //Get the time for the latest life-preserver snapshot (and total number)
  /*
  //Find the index for the current list
  int ci = 0;
  while(ci < CLIST.length()){
    if(CLIST[ci].startsWith(zpool+":::")){ break; }
    else{ ci++; }
  }
  if(CLIST.isEmpty()){ ci = -1; } //catch for empty list
  if(DSC.subsetHash.size() < 1){
    DSC.numberOfSnapshots = "0";
    DSC.latestSnapshot= "";
  }else{
    DSC.numberOfSnapshots = QString::number(lpsnaps.length());
    if(lpsnaps.isEmpty()){ DSC.latestSnapshot=""; }
    else if(ci > -1 && ci < CLIST.length()){ 
      QString sna = CLIST[ci].section(":::",1,1);
      if(sna != "-"){ DSC.latestSnapshot= sna; }
      else{ DSC.latestSnapshot = ""; }      
    }else{ DSC.latestSnapshot=lpsnaps[0]; }
  }
  //List the replication status
  if(RLIST.contains(zpool) && (ci > -1)){ 
    QString rep = CLIST[ci].section(":::",2,2);
    if(rep != "-"){ DSC.latestReplication = rep; }
    else{ DSC.latestReplication= QObject::tr("Enabled"); }
  }else{ 
    DSC.latestReplication= QObject::tr("Disabled");
  }
  */
  //Now parse the "zpool status <zpool>" output
  
  //Return the dataset
  return DSC;
}

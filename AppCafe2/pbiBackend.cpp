/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
 //ID FORMATS: [<name> = Extras::nameToID(formalName)]
 //  pbiID: <name>-<version>-<arch> (of locally installed PBI)
 //  catID: <name> (of category in repo)
 //  appID: <name> (of app in repo)
 
 #include "pbiBackend.h"

 
 PBIBackend::PBIBackend() : QObject(){
   //initialize the background processes
   dlProc = new WorkProcess(); instProc = new WorkProcess(); 
   remProc = new WorkProcess(); updProc = new WorkProcess();
   QProcessEnvironment sysenv = QProcessEnvironment::systemEnvironment();
   dlProc->setProcessEnvironment(sysenv); instProc->setProcessEnvironment(sysenv); 
   remProc->setProcessEnvironment(sysenv); updProc->setProcessEnvironment(sysenv);
   PENDINGDL.clear(); PENDINGINSTALL.clear(); PENDINGREMOVAL.clear(); PENDINGUPDATE.clear();
   connect(this,SIGNAL(ProcessFinished()),this,SLOT(checkProcesses()) );
   //setup the base paths
   baseDBDir = "/var/db/pbi/";
   baseDlDir = "/tmp/";
   sysDB = new PBIDBAccess();
   noRepo=FALSE;
   //Filesystem watcher
   watcher = new QFileSystemWatcher();
   connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(slotSyncToDatabase()) );
   
 }
 
 // ==============================
 // ====== PUBLIC FUNCTIONS ======
 // ==============================
 void PBIBackend::setWardenMode(QString dir, QString ip){
   wardenDir = dir; wardenIP = ip;
   //wardenDir: used for direct access to a directory in the jail
   //wardenIP: used to chroot into the jail to run commands
   if( wardenDir.isEmpty() || wardenIP.isEmpty() ){ wardenMode = FALSE; }
   else{ 
     if(!wardenDir.endsWith("/")){wardenDir.append("/"); } //make sure it has a / on the end
     wardenMode = TRUE; 
   }
 }
 
 void PBIBackend::setDownloadDir(QString dir){
   if(QFile::exists(dir)){
     baseDlDir = dir;
   }else{
     qDebug() << "Invalid Download Directory requested:" << dir;	   
   }
 }
 
 bool PBIBackend::start(){
   sysArch = Extras::getSystemArch(); //get the architecture for the current system
   //Setup the base paths for the system database and downloads
   if(wardenMode){ 
     dlDir = wardenDir + baseDlDir;
     DBDir = wardenDir + baseDBDir;
   }else{ 
     dlDir = baseDlDir; 
     DBDir = baseDBDir;
   }
   //Now setup the database access class
   sysDB->setDBPath(DBDir);
   if(repoNumber.isEmpty()){
     QStringList repos = sysDB->availableRepos();
     if(repos.length() > 0){ repoNumber = repos[0]; }
     else{
       qDebug() << "No PBI Repositories available: disabling the browser";
       emit ErrorNoRepo();
       noRepo = TRUE;
     }
   }
   if(!noRepo){ sysDB->setRepo(repoNumber); }
   //Now start the filesystem watcher
   QStringList watched = watcher->directories();
   if(!watched.isEmpty()){ watcher->removePaths(watched); }//clear the watcher first
   watcher->addPath( DBDir+"installed" );   
   //Now initialize the hash lists with the database info
   QTimer::singleShot(1,this,SLOT(slotSyncToDatabase()) );

   return TRUE;
 }

 QStringList PBIBackend::installedList(){
   return QStringList( PBIHASH.keys() ); 
 }
 
 QStringList PBIBackend::browserCategories(){
   return QStringList( CATHASH.keys() ); 
 }
  
QStringList PBIBackend::browserApps( QString catID ){
  QStringList output;
  if(!CATHASH.contains(catID)){ return output; }
  QStringList apps = APPHASH.keys();
  for(int i=0; i<apps.length(); i++){
    if(Extras::nameToID(APPHASH[apps[i]].category)==catID){ output << apps[i]; }
  }
  return output;
}

QStringList PBIBackend::getRecentApps(){
  //List all new/updated applications in the last 10 days (sorted by oldest first)
  QStringList output;
  //Generate a string for today's date
  QDate date = QDate::currentDate();
  date = date.addDays(-10);
  QString chk = date.toString(Qt::ISODate);
  chk.remove("-"); chk.append(" 000000"); //don't specify a particular time
  //Now compare the latest apps to this date
  QStringList apps = APPHASH.keys();
  QStringList  unsorted;
  for(int i=0; i<apps.length(); i++){
    if(Extras::newerDateTime(APPHASH[apps[i]].latestDatetime, chk)){
      unsorted << APPHASH[apps[i]].latestDatetime + "::"+apps[i];	    
    }
  }
  //Now sort them by datetime and then remove the datetime stamp
  unsorted.sort(); //oldest->newest 
  for(int i=(unsorted.length()-1); i>=0; i--){ //go in reverse order
    output << unsorted[i].section("::",1,50);
  }
  return output; //newest->oldest
}

// ===== Local/Repo Interaction Functions =====
QString PBIBackend::isInstalled(QString appID){
  QString output;
  if(!APPHASH.contains(appID)){
    qDebug() << "Invalid application ID:" << appID;
    return output;
  }
  QStringList pbiID = PBIHASH.keys(); //get list of installed PBI's
  for(int i=0; i<pbiID.length();i++){
    QString pbi = Extras::nameToID(PBIHASH[pbiID[i]].name);
    if( pbi == appID){
      output = PBIHASH[pbiID[i]].version;
      break;
    }
  }
  return output;
}

QString PBIBackend::upgradeAvailable(QString pbiID){
  QString output;
  if(!PBIHASH.contains(pbiID)){return output;}
  QString appID = Extras::nameToID(PBIHASH[pbiID].name);
  if(APPHASH.contains(appID)){
    if(APPHASH[appID].latestVersion != PBIHASH[pbiID].version){output = APPHASH[appID].latestVersion;}  
  }
  return output;
}

 // === Information Retrieval functions ===
QStringList PBIBackend::PBIInfo( QString pbiID, QStringList infoList){
  QStringList output;
  if(!PBIHASH.contains(pbiID)){ return output; }
  for(int i=0; i<infoList.length(); i++){
    if(infoList[i]=="name"){ output << PBIHASH[pbiID].name; }
    else if(infoList[i]=="version"){ output << PBIHASH[pbiID].version; }
    else if(infoList[i]=="author"){ output << PBIHASH[pbiID].author; }
    else if(infoList[i]=="website"){ output << PBIHASH[pbiID].website; }
    else if(infoList[i]=="arch"){ output << PBIHASH[pbiID].arch; }
    else if(infoList[i]=="path"){ output << PBIHASH[pbiID].path; }
    else if(infoList[i]=="icon"){ output << PBIHASH[pbiID].icon; }
    else if(infoList[i]=="license"){ output << PBIHASH[pbiID].license; }
    else if(infoList[i]=="metaid"){ output << PBIHASH[pbiID].metaID; }
    else if(infoList[i]=="status"){ output << PBIHASH[pbiID].statusString; }
    //Now the boolians
    else if(infoList[i]=="requiresroot"){ 
      if(PBIHASH[pbiID].rootInstall){output<<"true";}
      else{ output<<"false";}
    }
    else if(infoList[i]=="autoupdate"){ 
      if(PBIHASH[pbiID].autoUpdate){output<<"true";}
      else{ output<<"false";}
    }
    else if(infoList[i]=="hasdesktopicons"){ 
      if(PBIHASH[pbiID].desktopIcons){output<<"true";}
      else{ output<<"false";}
    }
    else if(infoList[i]=="hasmenuicons"){ 
      if(PBIHASH[pbiID].menuIcons){output<<"true";}
      else{ output<<"false";}
    }
    else{ output << ""; }
  }
  //qDebug()<< "Info Requested for:" << pbiID << infoList << "Info:" << output;
  return output;
}	

QStringList PBIBackend::CatInfo( QString catID, QStringList infoList){
  QStringList output;
  if(!CATHASH.contains(catID)){ return output; }
  for(int i=0; i<infoList.length(); i++){
    if(infoList[i]=="name"){ output << CATHASH[catID].name; }
    else if(infoList[i]=="icon"){ output << CATHASH[catID].localIcon; }
    else if(infoList[i]=="description"){ output << CATHASH[catID].description; }   
    else{ output << ""; }
  }
  //qDebug()<< "Info Requested for:" << catID << infoList << "Info:" << output;
  return output;
}

QStringList PBIBackend::AppInfo( QString appID, QStringList infoList){
  QStringList output;
  if(!APPHASH.contains(appID)){ return output; }
  for(int i=0; i<infoList.length(); i++){
    if(infoList[i]=="name"){ output << APPHASH[appID].name; }
    else if(infoList[i]=="author"){ output << APPHASH[appID].author; }
    else if(infoList[i]=="website"){ output << APPHASH[appID].website; }
    else if(infoList[i]=="icon"){ output << APPHASH[appID].localIcon; }
    else if(infoList[i]=="license"){ output << APPHASH[appID].license; }
    else if(infoList[i]=="type"){ output << APPHASH[appID].appType; }
    else if(infoList[i]=="description"){ output << APPHASH[appID].description; }
    else if(infoList[i]=="category"){ output << APPHASH[appID].category; }
    else if(infoList[i]=="latestversion"){ output << APPHASH[appID].latestVersion; }
    else if(infoList[i]=="latestarch"){ output << APPHASH[appID].latestArch; }
    else if(infoList[i]=="latestsize"){ output << APPHASH[appID].latestSizeK; }
    else if(infoList[i]=="backupversion"){ output << APPHASH[appID].backupVersion; }
    else if(infoList[i]=="backuparch"){ output << APPHASH[appID].backupArch; }
    else if(infoList[i]=="backupsize"){ output << APPHASH[appID].backupSizeK; }
    //Now the boolians
    else if(infoList[i]=="requiresroot"){ 
      if(APPHASH[appID].requiresroot){output<<"true";}
      else{ output<<"false";}
    }
    else{ output << ""; }
  }
  //qDebug()<< "Info Requested for:" << appID << infoList << "Info:" << output;
  return output;
}

 // ==========================
 // ====== PUBLIC SLOTS ======
 // ==========================
 // NOTE: These functions should only be called from a QTimer::singleShot()
 //    Because outputs come via different signals (due to time for these functions to run)
 
void PBIBackend::startAppSearch(){
 //  Outputs come via the "SearchComplete(QStringList best,QStringList rest)" signal
 QString search = searchTerm; //This public variable needs to be set beforehand by the calling process
 QStringList best, rest;
 //Now perform the search and categorize it
 search = search.toLower();
 QStringList namematch, tagmatch, descmatch;
 QStringList app = APPHASH.keys();
 for(int i=0; i<app.length(); i++){
   if(APPHASH[app[i]].name.toLower() == search){ best << app[i]; } //exact match - top of the "best" list
   else if(APPHASH[app[i]].name.toLower().contains(search)){ namematch << app[i]; }
   else if(APPHASH[app[i]].tags.contains(search)){ tagmatch << app[i]; }
   else if(APPHASH[app[i]].description.contains(search)){ descmatch << app[i]; }
 }
 //Now sort the lists and assign a priority
 namematch.sort(); tagmatch.sort(); descmatch.sort();
 best << namematch;
 if(best.isEmpty()){ best << tagmatch; }
 else{ rest << tagmatch; }
 if(best.isEmpty()){ best << descmatch; }
 else{ rest << descmatch; }

 //Now emit the signal with the results
 emit SearchComplete(best,rest);
}

void PBIBackend::startSimilarSearch(){
  //  Outputs come via the "SimilarFound(QStringList results)" signal
  QString sID = searchSimilar; // this public variable needs to be set beforehand by the calling process
  QStringList output;  
  if(!APPHASH.contains(sID)){ return; } 
  //Now find the tags on the given ID
  QStringList stags = APPHASH[sID].tags;
  QStringList apps = APPHASH.keys();
  QStringList unsorted;
  int maxMatch=0;
  for(int i=0; i<apps.length(); i++){
    if(apps[i]==sID){continue;} //skip the app we were given for search parameters
    QStringList tags = APPHASH[apps[i]].tags;
    int match=0;
    for(int j=0; j<stags.length(); j++){
       if(tags.indexOf(stags[j]) != -1){ match++; }
    }
    if(match > 1){ unsorted << QString::number(match)+"::"+apps[i]; }
    if(match > maxMatch){ maxMatch = match; }
  }
  unsorted.sort();
  for(int i=unsorted.length(); i>0; i--){
    if( unsorted[i-1].section("::",0,0).toInt() > (maxMatch-1) ){
      output << unsorted[i-1].section("::",1,50);
    }else if( output.length() < 5 ){ //not enough matches, grab the next set too
      output << unsorted[i-1].section("::",1,50);
      maxMatch--;
    }else{ break; } //already sorted and the rest are even lower match rating
  }
  //Now emit the signal with the results
  emit SimilarFound(output);
}

 // ===============================
 // ====== PRIVATE FUNCTIONS ======
 // ===============================
 

 // ===============================
 // ======   PRIVATE SLOTS   ======
 // ===============================
 
 // Internal Process Management
 void PBIBackend::checkProcesses(){
   if( dlProc->isReady && !PENDINGDL.isEmpty() ){
     QTimer::singleShot(0,this,SLOT(startDownloadProcess(PENDINGDL[0])) );
     PENDINGDL.removeAt(0);
   }
   if( instProc->isReady && !PENDINGINSTALL.isEmpty() ){
     QTimer::singleShot(0,this,SLOT(startInstallProcess(PENDINGINSTALL[0])) );
     PENDINGINSTALL.removeAt(0);
   }
   if( remProc->isReady && !PENDINGREMOVAL.isEmpty() ){
     QTimer::singleShot(0,this,SLOT(startRemovalProcess(PENDINGREMOVAL[0])) );	
     PENDINGREMOVAL.removeAt(0);
   }
   if( updProc->isReady && !PENDINGUPDATE.isEmpty() ){
     QTimer::singleShot(0,this,SLOT(startUpdateProcess(PENDINGUPDATE[0])) );	
     PENDINGUPDATE.removeAt(0);	   
   }
 }
 
 void PBIBackend::startDownloadProcess(QString settings){
   qDebug() << "Download Process needs to be finished:" << settings;
   return;
   //DO NOT CALL THIS FUNCTION MANUALLY: Should be called asynchronously from checkProcesses()
   dlProc->isReady=FALSE; //flag that this process is currently working
   //dlProc->pbiID = 
   //Generate the command for the system call
   QString name = settings.section(":::",0,0,QString::SectionSkipEmpty);
   QString version = settings.section(":::",1,1,QString::SectionSkipEmpty);
   QString update = settings.section(":::",2,2,QString::SectionSkipEmpty).toLower();
   if( (update == "yes") || (update=="true") ){
     //Updates cannot be downloaded seperately - add it to the pending installs instead
   }else{
     //Download a new PBI
     QString cmd = "pbi_add -R";
     if(!version.isEmpty()){ cmd.append(" --rVer "+version); }
     cmd.append(" "+name);
     //Move the process to the downloads directory
     dlProc->setWorkingDirectory(dlDir);
     //Start the process
     dlProc->start(cmd);
     //Wait for the process to finish
     bool ok = dlProc->waitForFinished(-1);
     if(!ok){
       // emit error message
       QString error = QString( dlProc->readAllStandardError() );
       emit DownloadError(error);
     }else{
       //Add this file to the pending installs
     }
   }
   dlProc->isReady=TRUE; //flag that this process is now ready again
   emit ProcessFinished();
 }
 
 void PBIBackend::startInstallProcess(QString settings){
   qDebug() << "Install Process needs to be finished:" << settings;
   return;
   //DO NOT CALL THIS FUNCTION MANUALLY: Should be called asynchronously from checkProcesses()
   instProc->isReady=FALSE; //flag that this process is currently working
   //instProc->pbiID =
   //Generate the command for the system call
   QString cmd;
     //Start the process
     instProc->start(cmd);
     //Wait for the process to finish
     bool ok = instProc->waitForFinished(-1);
     if(!ok){
       // emit error message
       QString error = QString( instProc->readAllStandardError() );
       emit InstallationError(error);
     }
   instProc->isReady=TRUE; //flag that this process is now ready again
   emit ProcessFinished();
 }
 
 void PBIBackend::startRemovalProcess(QString pbiID){
   qDebug() << "Removal Process needs to be finished:" << pbiID;
   return;
   //DO NOT CALL THIS FUNCTION MANUALLY: Should be called asynchronously from checkProcesses()
   remProc->isReady=FALSE; //flag that this process is currently working
   remProc->pbiID = pbiID;
   //Make sure the PBI is actually installed
   if( PBIHASH.contains(pbiID) ){
     //Generate the command for the system call
     QString cmd = "pbi_delete "+pbiID;
     //Modify the command if it requires root access
     if(wardenMode){
       cmd.prepend("warden chroot "+wardenIP+" \""); cmd.append("\"");
     }else if( PBIHASH[pbiID].rootInstall ){
       cmd.prepend("pc-su \""); cmd.append("\"");	     
     }
     //Start the process
     remProc->start(cmd);
     //Wait for the process to finish
     bool ok = remProc->waitForFinished(-1);
     if(!ok){
       // emit error message
       QString error = QString( remProc->readAllStandardError() );
       emit RemovalError(error);
     }
   }
   remProc->isReady=TRUE; //flag that this process is now ready again
   emit ProcessFinished();	 
 }
 
  void PBIBackend::startUpdateProcess(QString pbiID){
   qDebug() << "Update Process needs to be finished:" << pbiID;
   return;
   //DO NOT CALL THIS FUNCTION MANUALLY: Should be called asynchronously from checkProcesses()
   updProc->isReady=FALSE; //flag that this process is currently working
   updProc->pbiID = pbiID;
   //Make sure the PBI is actually installed
   if( PBIHASH.contains(pbiID) ){
     //Generate the command for the system call
     QString cmd = "pbi_update "+pbiID;
     //Modify the command if it requires root access or running in warden mode
     if(wardenMode){
       cmd.prepend("warden chroot "+wardenIP+" \""); cmd.append("\"");
     }else if( PBIHASH[pbiID].rootInstall ){
       cmd.prepend("pc-su \""); cmd.append("\"");	     
     }
     //Start the process
     updProc->start(cmd);
     //Wait for the process to finish
     bool ok = updProc->waitForFinished(-1);
     if(!ok){
       // emit error message
       QString error = QString( updProc->readAllStandardError() );
       emit UpdateError(error);
     }
   }
   updProc->isReady=TRUE; //flag that this process is now ready again
   emit ProcessFinished();	 
 }
 
 // === Database Synchronization ===
 void PBIBackend::slotSyncToDatabase(){
   //Locally Installed PBI Changes 
   //qDebug() << "Sync Local Database Info";
   QStringList currInst = installedList();
   QStringList sysList = sysDB->installed();
    //new and existing entries
   for(int i=0; i<sysList.length(); i++){
     int index = currInst.indexOf(sysList[i]);
     if( index == -1){ //New entry
       PBIHASH.insert(sysList[i],InstalledPBI()); //add a blank entry
       syncPBI(sysList[i]); //Now update the info
       //Add it to the watcher
       watcher->addPath(DBDir+"installed/"+sysList[i]);
     }else{  //Existing entry - remove it from the currInst list
       syncPBI(sysList[i]); //synchronize the data
       currInst.removeAt(index);
     }
   }
   // entries that need to be removed
   for(int i=0; i<currInst.length(); i++){
     //qDebug() << "PBI's to remove from the hash:" << currInst;
     if(PBIHASH[currInst[i]].status == (InstalledPBI::DOWNLOADING | InstalledPBI::INSTALLING | InstalledPBI::PENDINGDOWNLOAD | InstalledPBI::PENDINGINSTALL)){
     }else{ PBIHASH.remove(currInst[i]); }//only remove it if it is not awaiting installation/download	     
   }
   numInstalled = QStringList(PBIHASH.keys()).length();
   emit LocalPBIChanges(); //Let others know that the local PBI's have been updated
   //qDebug() << "Sync Repo Info";
   //Repo Changes
   if(!noRepo && CATHASH.isEmpty() && APPHASH.isEmpty() ){
     //If successful, the repo data should only be loaded once
     syncCurrentRepo(); 
     //If the sync was successful, re-run the PBI sync process to update the license info
     if( !APPHASH.isEmpty() ){ 
       numAvailable = QStringList(APPHASH.keys()).length();
       emit RepositoryInfoReady(); //let the UI know that the repo info is ready     	     
       QTimer::singleShot(10,this,SLOT(slotSyncToDatabase())); 
     }
   }  
 }
 
 void PBIBackend::syncPBI(QString pbiID){
   //Get the PBI structure
   if( !PBIHASH.contains(pbiID) ){ return; }
   InstalledPBI pbi = PBIHASH[pbiID];
   //Now get the database info
   QStringList info = sysDB->installedPbiInfo(pbiID); //info[name,version,arch,date,author,web,path,icon]
     //Generate the meta-info ID for this PBI
     QString appName = pbiID.section("-"+info[1],0,0); // pbiID = <name>-<version>-<arch>, just need <name>

   bool autoUp = sysDB->installedPbiAutoUpdate(pbiID);
   bool root = sysDB->installedPbiNeedsRoot(pbiID);
   bool desktop = sysDB->installedPbiHasXdgDesktop(info[6]);
   bool menu = sysDB->installedPbiHasXdgMenu(info[6]);
   //Now add this info to the PBI structure
   pbi.name    = info[0];
   pbi.version = info[1];
   pbi.arch    = info[2];
   pbi.mdate   = info[3];
   pbi.author  = info[4];
   pbi.website = info[5];
   pbi.path    = info[6];
   pbi.icon    = info[7];
   pbi.metaID  = appName;
   if(APPHASH.contains(appName)){
     pbi.license = APPHASH[appName].license;	   
   }else{
     pbi.license = tr("Unknown");
   }
   pbi.rootInstall = root;
   pbi.autoUpdate  = autoUp;
   pbi.desktopIcons= desktop;
   pbi.menuIcons   = menu;
   //Now update the status as appropriate
   QString upgrade = upgradeAvailable(pbiID);
   if(dlProc->pbiID == pbiID){pbi.setStatus(InstalledPBI::DOWNLOADING);}
   else if(instProc->pbiID == pbiID){pbi.setStatus(InstalledPBI::INSTALLING);}
   else if(remProc->pbiID == pbiID){pbi.setStatus(InstalledPBI::REMOVING);}
   else if(updProc->pbiID == pbiID){pbi.setStatus(InstalledPBI::UPDATING);}
   else if(PENDINGDL.contains(pbiID)){pbi.setStatus(InstalledPBI::PENDINGDOWNLOAD);}
   else if(PENDINGINSTALL.contains(pbiID)){pbi.setStatus(InstalledPBI::PENDINGINSTALL);}
   else if(PENDINGREMOVAL.contains(pbiID)){pbi.setStatus(InstalledPBI::PENDINGREMOVAL);}
   else if(PENDINGUPDATE.contains(pbiID)){pbi.setStatus(InstalledPBI::PENDINGUPDATE);}
   else if( !upgrade.isEmpty() ){pbi.setStatus(InstalledPBI::UPDATEAVAILABLE, upgrade); }
   else{ pbi.status=InstalledPBI::NONE; }
   //Now add this pbi structure back into the hash
   PBIHASH.insert(pbiID, pbi);
 }
 
 void PBIBackend::syncCurrentRepo(){
   //Calling this function will automatically clear and re-populate the APPHASH and CATHASH fields
   APPHASH.clear(); CATHASH.clear();
   //First do the meta data (app/cat info)
   QStringList metaFile = Extras::readFile(sysDB->metaFilePath());
   //qDebug() << "Sync Meta File Info";
   for(int i=0; i<metaFile.length(); i++){
     if(metaFile[i].startsWith("App=")){
       QStringList info = sysDB->parseAppMetaLine(metaFile[i].section("=",1,50,QString::SectionSkipEmpty));
       //qDebug() << "App Meta data:" << info;
       if(!info.isEmpty()){ //Make sure the info list is valid
         //info[name,category,remoteIcon,author,website,license,apptype,tags,description,requiresroot] (5/1/2013)
         //Simplify a couple pieces of info
         QString metaID = Extras::nameToID(info[0]);
         QString localIcon = sysDB->remoteToLocalIcon(info[0],info[2]);
         //Create the container and fill it
         MetaPBI app;
         app.name=info[0]; app.category=info[1]; app.remoteIcon=info[2];
         app.localIcon=localIcon; app.author=info[3]; app.website=info[4];
         app.license=info[5]; app.appType=info[6]; app.tags=info[7].toLower().split(","); 
         app.description=info[8];
         if(info[9]=="true"){ app.requiresroot=TRUE; }
         else{ app.requiresroot=FALSE; }
         //Fix the website if needed
         if(app.website.endsWith("/")){ app.website.chop(1); }
         //Add it to the hash
         APPHASH.insert(metaID,app);
       }
     }else if(metaFile[i].startsWith("Cat=")){
       QStringList info = sysDB->parseCatMetaLine(metaFile[i].section("=",1,50,QString::SectionSkipEmpty));
       //qDebug() << "Cat Meta Data:" << info;
       if(info.length() > 2){ //Make sure the info list is complete
         //info[name,remoteicon,description,?] (5/1/2013)
         QString catID = Extras::nameToID(info[0]);
         QString localIcon = sysDB->remoteToLocalIcon(info[0],info[1]);
         //Create the container and fill it
         MetaCategory cat;
         cat.name=info[0]; cat.remoteIcon=info[1]; cat.localIcon=localIcon; cat.description=info[2];
         //Add it to the hash
         CATHASH.insert(catID,cat);
       }
     }
     //qDebug() << " - done with meta line";
   }
   //qDebug() << "Sync Index File Info";
   //Then do the list of available PBI's
   QStringList indexFile = Extras::readFile(sysDB->indexFilePath());
   bool sys64 = (sysArch=="amd64");
   for(int i=0; i<indexFile.length(); i++){
     QStringList info = sysDB->parseIndexLine(indexFile[i]);
       //info[name, arch, version, datetime, size, isLatest(true/false)]
     if(!info.isEmpty()){
       QString metaID = Extras::nameToID(info[0]);
       if(APPHASH.contains(metaID)){
       	 bool islatest = (info[5]=="true");
       	 bool is64 = (info[1] == "amd64");
       	 bool save=FALSE;
       	 //Determine whether to save the data or not
       	 if( !sys64 && is64 ){}   // do not save 64-bit apps on a 32-bit system
       	 else if(info[1] == sysArch){ save=TRUE; }  // high priority for identical architecture
       	 else{ //lower priority for 32-bit App on 64-bit system
       	   if(islatest){ 
       	     if(APPHASH[metaID].latestDatetime.isEmpty()){ save=TRUE; } //nothing saved yet, go ahead
       	     else if(APPHASH[metaID].latestArch==sysArch){} // do not save over an app that is the proper arch
       	     else if(Extras::newerDateTime(info[3], APPHASH[metaID].latestDatetime) ){ save=TRUE; } //save over older app
       	   }else{
       	     if(APPHASH[metaID].backupDatetime.isEmpty()){ save=TRUE; } //nothing saved yet, go ahead
       	     else if(APPHASH[metaID].backupArch==sysArch){} // do not save over an app that is the proper arch
       	     else if(Extras::newerDateTime(info[3], APPHASH[metaID].backupDatetime) ){ save=TRUE; } //save over older app
       	   }
       	 }
       	 //Save the data if appropriate
       	 if(save && islatest){
       	   APPHASH[metaID].latestVersion=info[2];
       	   APPHASH[metaID].latestDatetime=info[3];
       	   APPHASH[metaID].latestArch=info[1];  
       	   APPHASH[metaID].latestSizeK=info[4];
       	 }else if(save){
       	   APPHASH[metaID].backupVersion=info[2];
       	   APPHASH[metaID].backupDatetime=info[3];
       	   APPHASH[metaID].backupArch=info[1]; 
       	   APPHASH[metaID].backupSizeK=info[4];
       	 } 
       	 //if(save){ qDebug() << "APP:" << metaID << info[1] << info[2] << info[3] << info[4] << info[5]; }
       } //end check for ID in hash
     } //end check for info available
   } //end loop over index file
   
   //Now clean up the APPHASH to remove any entries that do not have a PBI associated with them
   QStringList apps = APPHASH.keys();
   for(int i=0; i<apps.length(); i++){
     if(APPHASH[apps[i]].latestVersion.isEmpty()){
       APPHASH.remove(apps[i]);	     
     }
   }
 }

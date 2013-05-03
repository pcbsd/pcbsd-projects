#include "containers.h"

//Initialization functions for the container classes

// --- INSTALLEDPBI ---
InstalledPBI::InstalledPBI(){
      name=""; version=""; author=""; website=""; arch=""; path=""; icon=""; license=""; mdate="";
      metaID=""; statusString="";rootInstall=FALSE; autoUpdate=FALSE; desktopIcons=FALSE; menuIcons=FALSE;
      status = NONE;
}

void InstalledPBI::setStatus(PBISTATUS stat, QString addinfo){
  //addinfo: additional info (download statistics or update version available)
  status = stat;
  if(stat == DOWNLOADING){ 
    if(addinfo.isEmpty()){ statusString = QObject::tr("Downloading..."); }
    else{ statusString = QString(QObject::tr("Downloading: %1")).arg(addinfo); }
  }
  else if(stat == INSTALLING){ statusString = QObject::tr("Installing..."); }
  else if(stat == REMOVING){ statusString = QObject::tr("Removing..."); }
  else if(stat == UPDATING){ statusString = QObject::tr("Updating..."); }
  else if(stat == PENDINGDOWNLOAD){ statusString = QObject::tr("Pending Download"); }
  else if(stat == PENDINGINSTALL){ statusString = QObject::tr("Pending Install"); }
  else if(stat == PENDINGREMOVAL){ statusString = QObject::tr("Pending Removal"); }
  else if(stat == PENDINGUPDATE){ statusString = QObject::tr("Pending Update"); }
  if(stat == UPDATEAVAILABLE){ 
    if(addinfo.isEmpty()){ statusString = QObject::tr("Update Available!"); }
    else{ statusString = QString(QObject::tr("Version %1 Available")).arg(addinfo); }
  }
  else{ statusString = ""; }
}

// --- MetaPBI ---
MetaPBI::MetaPBI(){
      name=""; category=""; remoteIcon=""; author=""; website="";
      license=""; appType=""; description=""; tags=QStringList();  
      latestVersion=""; latestDatetime=""; latestArch=""; latestSizeK="";
      backupVersion=""; backupDatetime=""; backupArch=""; backupSizeK="";
      requiresroot=FALSE;
}

// --- MetaCategory ---
MetaCategory::MetaCategory(){
      name=""; remoteIcon=""; description=""; localIcon="";    
}


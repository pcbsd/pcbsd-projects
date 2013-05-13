#ifndef _APPCAFE_CONTAINERS_H
#define _APPCAFE_CONTAINERS_H

#include <QString>
#include <QProcess>
#include <QObject>
#include <QDebug>

// Info Container for Installed PBI's (pbi_info <pbi-name> data)
class InstalledPBI{
	
  
  
  public:
    enum PBISTATUS{ NONE, DOWNLOADING, INSTALLING, REMOVING, UPDATING, PENDINGDOWNLOAD, PENDINGINSTALL, PENDINGREMOVAL, PENDINGUPDATE, UPDATEAVAILABLE, WORKING };
    QString name, version, author, website, arch, path, icon, license, mdate, metaID, statusString, downloadfile;
    PBISTATUS status;
    bool rootInstall, autoUpdate, desktopIcons, menuIcons;
    
    InstalledPBI();
    ~InstalledPBI(){}
    void setStatus(PBISTATUS stat, QString addInfo="");
  
};

// Info Container for PBI Meta information on a repo
class MetaPBI{
	
  public:
    QString name, category, remoteIcon, localIcon, author, website, license, appType, description;
    QStringList tags;
    QString latestVersion, latestDatetime, latestArch, latestSizeK, latestFilename; //newest version in the index
    QString backupVersion, backupDatetime, backupArch, backupSizeK, backupFilename; //next-to-newest version in the index
    bool requiresroot;
    
    MetaPBI(){ requiresroot=FALSE; }
    ~MetaPBI(){}
		
};

// Info Container for Category Meta information on a repo
class MetaCategory{
	
  public:
    QString name, remoteIcon, localIcon, description;
    
    MetaCategory(){}
    ~MetaCategory(){}
    
};

#endif

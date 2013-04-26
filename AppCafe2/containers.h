#ifndef _APPCAFE_CONTAINERS_H
#define _APPCAFE_CONTAINERS_H

#include <QString>

// Info Container for Installed PBI's (pbi_info <pbi-name> data)
class InstalledPBI{
  public:
    QString name, version, author, website, arch, path, icon, license;
    int repoID;
    bool rootInstall, autoUpdate, desktopIcons, menuIcons;
    
    InstalledPBI(){
      name=""; version=""; author=""; website=""; arch=""; path=""; icon=""; license="";
      repoID=-1; rootInstall=FALSE; autoUpdate=FALSE; desktopIcons=FALSE; menuIcons=FALSE;	    
    }
    
    virtual ~InstalledPBI();
  
};

#endif

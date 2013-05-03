#include "pbiDBAccess.h"

// ================================
// =======  SETUP FUNCTIONS =======
// ================================
bool PBIDBAccess::setDBPath(QString fullPath){
  //Make sure the directory exists first
  if(QFile::exists(fullPath)){
    DBPath = fullPath;
    if(!DBPath.endsWith("/")){DBPath.append("/");}
    DBDir->setPath(fullPath);
    return TRUE;
  }else{
    return FALSE;	  
  }
}

bool PBIDBAccess::setRepo(QString repoNum){
  //Make sure the repo is available
  bool ok = DBDir->cd(DBPath+"repos");
  if(ok){ //directory exists
    QStringList rL = DBDir->entryList(QStringList()<<repoNum+"*",QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable);
    if(rL.length() == 1){ //this repo exists
      currentRepoNumber=rL[0].section(".",0,0,QString::SectionSkipEmpty);
      currentRepoID=rL[0].section(".",1,1,QString::SectionSkipEmpty);
      return TRUE;
    }
  }
  return FALSE;
}

QStringList PBIDBAccess::availableRepos(){
  QStringList output;
  bool ok = DBDir->cd(DBPath+"repos");
  if(ok){ //directory exists
    QStringList repoFiles = DBDir->entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable);
    for(int i=0; i<repoFiles.length(); i++){
      output << repoFiles[i].section(".",0,0,QString::SectionSkipEmpty);
    }
  }
  return output;
}

// ========================================
// =======  PUBLIC ACCESS FUNCTIONS =======
// ========================================
QStringList PBIDBAccess::installed(){
  QStringList output;
  bool ok = DBDir->cd(DBPath+"installed");
  if(ok){
    output = DBDir->entryList( QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable);
  }
  return output;
}

QStringList PBIDBAccess::installedPbiInfo(QString pbiID){
  //Output format: output[ name, version, arch, date created, author, website, installpath, iconpath]
  QStringList output;
  QString path = DBPath+"installed/"+pbiID;
  bool ok = DBDir->cd(path);
  if(ok){
    output << readOneLineFile(path+"/pbi_name");
    output << readOneLineFile(path+"/pbi_version");
    output << readOneLineFile(path+"/pbi_arch");
    output << readOneLineFile(path+"/pbi_mdate");
    output << readOneLineFile(path+"/pbi_author");
    output << readOneLineFile(path+"/pbi_web");
    output << readOneLineFile(path+"/pbi_installedpath");
    if(DBDir->exists("pbi_icon.png")){ output << path+"/pbi_icon.png"; }
    else{ output << ""; }
  }
  return output;
}

bool PBIDBAccess::installedPbiAutoUpdate(QString pbiID){
   bool ok = FALSE;
   if( QFile::exists(DBPath+"installed/"+pbiID+"/autoupdate-enable") ){ ok = TRUE; }
   //qDebug() << "AutoUpdate:" << pbiID << ok;
   return ok;
}

bool PBIDBAccess::installedPbiNeedsRoot(QString pbiID){
  bool ok=FALSE;
  if( QFile::exists(DBPath+"installed/"+pbiID+"/pbi_requiresroot") ){ ok=TRUE; }
  else{
    //Also check who installed the PBI if not flagged directly
    QFileInfo fInfo(DBPath+"installed/"+pbiID);
    if( fInfo.owner() == "root" ){ ok=TRUE; }
  }
  //qDebug() << pbiID << "requires root:" << ok;
  return ok;
}

bool PBIDBAccess::installedPbiHasXdgDesktop(QString installPath){
  if(!installPath.endsWith("/")){ installPath.append("/"); }
  bool ok = DBDir->cd(installPath+".xdg-desktop");
  if(ok){
    if( DBDir->entryList(QStringList()<<"*.desktop",QDir::Files).length() > 0 ){ return TRUE; }	  
  }
  return FALSE;
}

bool PBIDBAccess::installedPbiHasXdgMenu(QString installPath){
  if(!installPath.endsWith("/")){ installPath.append("/"); }
  bool ok = DBDir->cd(installPath+".xdg-menu");
  if(ok){
    if( DBDir->entryList(QStringList()<<"*.desktop",QDir::Files).length() > 0 ){ return TRUE; }	  
  }
  return FALSE;
}

QString PBIDBAccess::indexFilePath(){
  return DBPath+"index/"+currentRepoID+"-index";
}

QString PBIDBAccess::metaFilePath(){
  return DBPath+"index/"+currentRepoID+"-meta";	
}

QStringList PBIDBAccess::parseIndexLine(QString line){
  //output[name, arch, version, datetime, isLatest(bool)]
  //line format 5/1/2013: [name,arch,version,checksum,datetime,mirrorPathToPBI,sizeInK,?,current/active,?,?]
      // NOTE: last two entries missing quite often
  QStringList lineInfo = line.split(":");
  QStringList output;
  if(lineInfo.length() < 9 ){ return output; } //skip incomplete entries
  output << lineInfo[0]; //name
  output << lineInfo[1]; //architecture
  output << lineInfo[2]; //version
  output << lineInfo[4]; //datetime
  if(lineInfo[8].simplified() == "current"){ output << "true"; } //is most recent version
  else{ output << "false"; }  //is an older version
  return output;
}

QStringList PBIDBAccess::parseAppMetaLine(QString line){
  // line format 5/1/2013: [name,category,remoteIcon,author,website,license,apptype,tags,description,requiresroot]
  QStringList output = line.split(";");
  if(output.length() < 10){ output.clear();} //invalid line
  else if(output[9]=="YES"){ output[9]="true"; } //change to the same true/false syntax as elsewhere
  return output;
}

QStringList PBIDBAccess::parseCatMetaLine(QString line){
  // line format 5/1/2013: [name,remoteicon,description,?]
  QStringList output = line.split(";");
  if(output.length() < 3){output.clear(); } //incomplete line
  return output;
}
	
QString PBIDBAccess::remoteToLocalIcon(QString name, QString remoteIconPath){
  QString output = DBPath+"repo-icons/"+currentRepoID+"-"+name+"."+remoteIconPath.section(".",-1);
  //qDebug() << "Remote to Local Icon Path conversion:" << remoteIconPath << output;
  return output;
}


// ========================================
// =======  PRIVATE ACCESS FUNCTIONS ======
// ========================================
QString PBIDBAccess::readOneLineFile(QString path){
  QFile file(path);
  if(!file.exists()){ return ""; } //Return nothing for missing file
  //Now read the file
  QString output;
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while(!in.atEnd()){
      output.append(in.readLine());
    }
    file.close();
  }
  return output;
}

#include "portStruct.h"

PortStruct::PortStruct(){
  SETTINGS = new QSettings("PC-BSD", "EasyPort");
  basepath.clear();
}

PortStruct::~PortStruct(){
  delete SETTINGS;
}

// ==============
//    Port information
// ==============
bool PortStruct::loadPort(QString portPath){
  QDir dir(portPath);
  if(dir.exists()){
    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    if(files.contains("Makefile") && (files.contains("pkg-descr") || files.contains("pkg-plist") || files.contains("distinfo") ) ){
      basepath = dir.absolutePath();
      return true;
    }
  }
  return false;
}

QString PortStruct::portPath(){
  return basepath;
}

QStringList PortStruct::mainFiles(){
  QStringList files;
	files << "Makefile" << "pkg-descr" << "pkg-plist" << "distinfo";
  return files;
}

QStringList PortStruct::extraFiles(){
  QDir dir(basepath);
  QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
  QStringList main = this->mainFiles();
  for(int i=0; i<main.length(); i++){ files.removeAll(main[i]); }
  if(dir.exists("files")){
    dir.cd("files");
    QStringList NF = dir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    for(int i=0; i<NF.length(); i++){ files << "files/"+NF[i]; }
  }
  return files;
}


// ===============
//     General Settings
// ===============
QString PortStruct::portsDir(){
  return SETTINGS->value("PORTSDIR", "/usr/ports").toString();
}

void PortStruct::setPortsDir(QString dir){
  SETTINGS->setValue("PORTSDIR", dir);
}

QString PortStruct::newPortDir(){
  return SETTINGS->value("NEWDIR", QDir::homePath()+"/EasyPort").toString();
}

void PortStruct::setNewPortDir(QString dir){
  SETTINGS->setValue("NEWDIR", dir);	
}

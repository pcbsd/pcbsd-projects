#include "LPBackend.h"

// ==============
//     Informational
// ==============
QStringList LPBackend::listPossibleDatasets(){
  QString cmd = "zpool list -H -o name";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output (one dataset per line - no headers)
  QStringList list;
  for(int i=0; i<out.length(); i++){ //skip the first two lines (headers)
    QString ds = out[i].section("/",0,0).simplified();
    if(!ds.isEmpty()){ list << ds; }
  }
  list.removeDuplicates();
  return list;	
}

QStringList LPBackend::listDatasets(){
  QString cmd = "lpreserver listcron";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  QStringList list;
  for(int i=2; i<out.length(); i++){ //skip the first two lines (headers)
    QString ds = out[i].section(" - ",0,0).simplified();
    if(!ds.isEmpty()){ list << ds; }
  }
  return list;
}

QStringList LPBackend::listSnapshots(QString dataset){
  QString cmd = "lpreserver listsnap "+dataset;
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  QStringList list;
  for(int i=0; i<out.length(); i++){
    if(out[i].startsWith(dataset+"@")){
      QString snap = out[i].section("@",1,3).section(" ",0,0).simplified();;
      if(!snap.isEmpty()){ list << snap; }
    }
  }
  return list;	
}

QStringList LPBackend::listReplicationTargets(){
  QString cmd = "lpreserver replicate list";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  QStringList list;
  for(int i=0; i<out.length(); i++){
    if(out[i].contains("->")){
      QString ds = out[i].section("->",0,0).simplified();
      if(!ds.isEmpty()){ list << ds; }
    }
  }
  return list;		
}

// ==================
//    Snapshop Management
// ==================
void LPBackend::newSnapshot(QString dataset){
  QString cmd = "lpreserver mksnap "+dataset;
  system(cmd.toUtf8());
}

void LPBackend::removeSnapshot(QString dataset, QString snapshot){
  QString cmd = "lpreserver rmsnap "+dataset +" "+snapshot;
  system(cmd.toUtf8());	
}

void LPBackend::revertSnapshot(QString dataset, QString snapshot){
  QString cmd = "lpreserver revertsnap "+dataset +" "+snapshot;
  system(cmd.toUtf8());
}

void LPBackend::browseSnapshot(QString dataset, QString snapshot){
  //Not implemented yet
}

// ==================
//    Replication Management
// ==================
void LPBackend::setupReplication(QString dataset, QString remotehost, QString user, int port, QString remotedataset, int time){
  QString stime = "sync"; //synchronize on snapshot creation
  if(time < 0 || time > 24){
	  
  }else{ stime = QString::number(time); } //daily at a particular hour (24 hour notation)
  
  QString cmd = "lpreserver replicate add "+remotehost+" "+user+" "+ QString::number(port)+" "+dataset+" "+remotedataset+" "+stime;
  system(cmd.toUtf8());
}

void LPBackend::removeReplication(QString dataset){
  QString cmd = "lpreserver replicate remove "+dataset;
  system(cmd.toUtf8());	
}

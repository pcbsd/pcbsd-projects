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
  for(int i=0; i<out.length(); i++){
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

QStringList LPBackend::listDatasetSubsets(QString dataset){
  QString cmd = "zfs list -H -t filesystem -o name";
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
  for(int i=out.length()-1; i>=0; i--){ //go in reverse order for proper time format (newest first)
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
//    Dataset Management
// ==================
bool LPBackend::setupDataset(QString dataset, int time, int numToKeep){
  //Configure inputs
  QString freq;
  if(time == -30){ freq = "30min"; }
  else if(time == -10){ freq = "10min"; }
  else if(time == -5){ freq = "5min"; }
  else if(time >= 0 && time < 24){ freq = "daily@"+QString::number(time); }
  else{ freq = "hourly"; }
  
  //Create the command
  QString cmd = "lpreserver cronsnap "+dataset+" start "+freq+" "+QString::number(numToKeep);
  int ret = system(cmd.toUtf8());
  return (ret == 0);
}

bool LPBackend::removeDataset(QString dataset){
  QString cmd = "lpreserver cronsnap "+dataset+" stop";
  int ret = system(cmd.toUtf8());	
  return (ret == 0);
}

bool LPBackend::datasetInfo(QString dataset, int& time, int& numToKeep){
  QString cmd = "lpreserver cronsnap";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  bool ok = false;
	qDebug() << "lpreserver cronsnap:\n" << out;
  return ok;
}

// ==================
//    Snapshop Management
// ==================
bool LPBackend::newSnapshot(QString dataset){
  QString cmd = "lpreserver mksnap "+dataset;
  int ret = system(cmd.toUtf8());
  return (ret == 0);
}

bool LPBackend::removeSnapshot(QString dataset, QString snapshot){
  QString cmd = "lpreserver rmsnap "+dataset +" "+snapshot;
  int ret = system(cmd.toUtf8());	
  return (ret == 0);
}

bool LPBackend::revertSnapshot(QString dataset, QString snapshot){
  QString cmd = "lpreserver revertsnap "+dataset +" "+snapshot;
  int ret  = system(cmd.toUtf8());
  return (ret == 0);
}

bool LPBackend::browseSnapshot(QString dataset, QString snapshot){
  //Not implemented yet
  return false;
}

// ==================
//    Replication Management
// ==================
bool LPBackend::setupReplication(QString dataset, QString remotehost, QString user, int port, QString remotedataset, int time){
  QString stime = "sync"; //synchronize on snapshot creation (default)
  if(time >= 0 || time < 24){ stime = QString::number(time); } //daily at a particular hour (24 hour notation)
  
  QString cmd = "lpreserver replicate add "+remotehost+" "+user+" "+ QString::number(port)+" "+dataset+" "+remotedataset+" "+stime;
  int ret = system(cmd.toUtf8());
  return (ret == 0);
}

bool LPBackend::removeReplication(QString dataset){
  QString cmd = "lpreserver replicate remove "+dataset;
  int ret = system(cmd.toUtf8());	
  return (ret == 0);
}

bool LPBackend::replicationInfo(QString dataset, QString& remotehost, QString& user, int& port, QString& remotedataset, int& time){
  QString cmd = "lpreserver replicate list";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  bool ok = false;
  for(int i=0; i<out.length(); i++){
    if(out[i].contains("->") && out[i].startsWith(dataset)){
      QString data = out[i].section("->",1,1);
      user = data.section("@",0,0);
      remotehost = data.section("@",1,1).section("[",0,0);
      port = data.section("[",1,1).section("]",0,0).toInt();
      remotedataset = data.section(":",1,1).section(" Time",0,0);
      QString synchro = data.section("Time:",1,1).simplified();
	if(synchro == "sync"){ time = -1; }
	else{ time = synchro.toInt(); }
      ok = true;
      break;
    }
  }	  
  return ok;
}

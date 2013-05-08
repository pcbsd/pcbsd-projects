#include "processManager.h"

ProcessManager::ProcessManager(){
  //Get the system environment for all the processes
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PBI_FETCH_PARSING","YES"); //For readable download notifications
    //Initialize the UPDATE Process
    upProc = new QProcess; upProc->setProcessEnvironment(env);
    upProc->setProcessChannelMode(QProcess::MergedChannels);
    connect(upProc, SIGNAL(readyRead()),this,SLOT(slotUpProcMessage()) );
    connect(upProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotUpProcFinished()) );
    //Initialize the REMOVE Process
    remProc = new QProcess; remProc->setProcessEnvironment(env);
    remProc->setProcessChannelMode(QProcess::MergedChannels);
    connect(remProc, SIGNAL(readyRead()),this,SLOT(slotRemProcMessage()) );
    connect(remProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotRemProcFinished()) );
    //Initialize the DOWNLOAD Process
    dlProc = new QProcess; dlProc->setProcessEnvironment(env);
    dlProc->setProcessChannelMode(QProcess::MergedChannels);
    connect(dlProc, SIGNAL(readyRead()),this,SLOT(slotDlProcMessage()) );
    connect(dlProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotDlProcFinished()) );
    //Initialize the INSTALL Process
    inProc = new QProcess; inProc->setProcessEnvironment(env);
    inProc->setProcessChannelMode(QProcess::MergedChannels);
    connect(inProc, SIGNAL(readyRead()),this,SLOT(slotInProcMessage()) );
    connect(inProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotInProcFinished()) );
}

ProcessManager::~ProcessManager(){
}

void ProcessManager::goToDirectory(ProcessID ID, QString dir){
  if( ID == UPDATE ){
    upProc->setWorkingDirectory(dir);	  
  }else if( ID == REMOVE ){
    remProc->setWorkingDirectory(dir);	   	  
  }else if( ID == DOWNLOAD ){
    dlProc->setWorkingDirectory(dir);  	  
  }else if( ID == INSTALL ){
    inProc->setWorkingDirectory(dir);   	  
  }	
}
// =========================
// ===== PUBLIC ACCESS =====
// =========================
void ProcessManager::startProcess(ProcessID ID, QString cmd){
  if( ID == UPDATE ){
    qDebug() << "Update Process Started:" << cmd;
    upProc->start(cmd);	  
  }else if( ID == REMOVE ){
    qDebug() << "Removal Process Started:" << cmd;
    remProc->start(cmd);	   	  
  }else if( ID == DOWNLOAD ){
    qDebug() << "Download Process Started:" << cmd;
    dlProc->start(cmd);	   	  
  }else if( ID == INSTALL ){
    qDebug() << "Install Process Started:" << cmd;
    inProc->start(cmd);	   	  
  }
	
}

void ProcessManager::stopProcess(ProcessID ID){
  if((ID == ALL) || (ID == UPDATE)){
    upProc->terminate();	  
  }else if((ID == ALL) || (ID == REMOVE)){
    remProc->terminate();	  
  }else if((ID == ALL) || (ID == DOWNLOAD)){
    dlProc->terminate();	  
  }else if((ID == ALL) || (ID == INSTALL)){
    inProc->terminate();	  
  }	
}


// =========================
// ===== PRIVATE SLOTS =====
// =========================
QString ProcessManager::parseDlLine(QString line){
  QString out;
  if(!line.startsWith("SIZE:")){ return out; }
  //Line format: SIZE:  <KB> DOWNLOADED:  <KB> SPEED:  <KB/s> KB/s
  line = line.simplified();
  line.replace("SIZE: ","");
  line.replace("DOWNLOADED: ", "");
  line.replace("SPEED: ","");
  line.replace("KB/s","");
  bool totok, curok, spdok;
  double tot, cur, spd;
  tot = line.section(" ",0,0).toDouble(&totok);
  cur = line.section(" ",1,1).toDouble(&curok);
  spd = line.section(" ",2,2).toDouble(&spdok);
  //qDebug() << "DownloadStats:" << line << tot << cur << spd;
  if(totok && tot==0){totok=FALSE;}
  if(curok && cur==0){curok=FALSE;}
  if(spdok && spd==0){spdok=FALSE;}
  //Now format the output string
  QString stats;
  if(totok && curok){
    int i=0;
    QStringList lab; lab << "KB" <<"MB"<<"GB"<<"TB"<<"PB";
    while( (cur>1024) && (tot/1024) && (i<lab.length()) ){
      cur=cur/1024; tot=tot/1024; i++;
    }
    float percent = int(cur*1000)/(tot*10); //rounded to one decimel place
    //round other numbers to one decimel place as well
    cur = int(cur*10)/10.0;
    tot = int(tot*10)/10.0;
    stats = QString::number(cur)+"/"+QString::number(tot)+" "+lab[i]+" ("+QString::number(percent)+"%)";
    // Format:  <current>/<total> <size label> (<percent>%)
  }else if(curok){
    stats = Extras::sizeKToDisplay(QString::number(cur));
  }
  QString speed;
  if(spdok){
    speed = Extras::sizeKToDisplay(QString::number(spd))+"/s";	  
  }
  //Now put the output string together and return it
  if(stats.isEmpty() && speed.isEmpty()){}
  else if(stats.isEmpty()){ out = speed; }
  else if(speed.isEmpty()){ out = stats; }
  else{ out = QString( tr("%1 at %2") ).arg(stats,speed); }
  return out;
}

// == UPDATE PROCESS ==
void ProcessManager::slotUpProcMessage(){
    while( upProc->canReadLine() ){
    QString line = upProc->readLine().simplified();
    QString dl = parseDlLine(line);
    if(!dl.isEmpty()){ emit ProcessMessage(UPDATE,dl); }
  }
}

void ProcessManager::slotUpProcFinished(){
  if(upProc->exitStatus() != QProcess::NormalExit){
    QString msg = upProc->readAllStandardError();
    if(msg.isEmpty()){ msg = upProc->readAllStandardOutput(); }
    if(msg.isEmpty()){ msg = tr("Unknown Error"); }
    qDebug() << "Update Process Error:"<<msg;
    emit ProcessError(UPDATE, msg);
  }else{
    qDebug() << "Update Process Finished";
    emit ProcessFinished(UPDATE);	  
  }
}

// == REMOVE PROCESS ==
void ProcessManager::slotRemProcMessage(){
  QString msg = remProc->readAllStandardOutput();
  emit ProcessMessage(REMOVE,msg);
}

void ProcessManager::slotRemProcFinished(){
  if(remProc->exitStatus() != QProcess::NormalExit){
    QString msg = remProc->readAllStandardError();
    if(msg.isEmpty()){ msg = remProc->readAllStandardOutput(); }
    if(msg.isEmpty()){ msg = tr("Unknown Error"); }
    qDebug() << "Removal Process Error:"<<msg;
    emit ProcessError(REMOVE, msg);
  }else{
    qDebug() << "Removal Process Finished";
    emit ProcessFinished(REMOVE);	  
  }
}

// == DOWNLOAD PROCESS ==
void ProcessManager::slotDlProcMessage(){
  while( dlProc->canReadLine() ){
    QString line = dlProc->readLine().simplified();
    QString dl = parseDlLine(line);
    if(!dl.isEmpty()){ emit ProcessMessage(DOWNLOAD,dl); }
  }
}

void ProcessManager::slotDlProcFinished(){
  if(dlProc->exitStatus() != QProcess::NormalExit){
    QString msg = dlProc->readAllStandardError();
    if(msg.isEmpty()){ msg = dlProc->readAllStandardOutput(); }
    if(msg.isEmpty()){ msg = tr("Unknown Error"); }
    qDebug() << "Download Process Error:"<<msg;
    emit ProcessError(DOWNLOAD, msg);
  }else{
    qDebug() << "Download Process Finished";
    emit ProcessFinished(DOWNLOAD);	  
  }
}

// == INSTALL PROCESS ==
void ProcessManager::slotInProcMessage(){
  QString msg = inProc->readAllStandardOutput();
  emit ProcessMessage(INSTALL,msg);
}

void ProcessManager::slotInProcFinished(){
  if(inProc->exitStatus() != QProcess::NormalExit){
    QString msg = inProc->readAllStandardError();
    if(msg.isEmpty()){ msg = inProc->readAllStandardOutput(); }
    if(msg.isEmpty()){ msg = tr("Unknown Error"); }
    qDebug() << "Install Process Error:"<<msg;
    emit ProcessError(INSTALL, msg);
  }else{
    qDebug() << "Install Process Finished";
    emit ProcessFinished(INSTALL);	  
  }
}

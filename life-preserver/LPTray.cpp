#include "LPTray.h"

//PUBLIC
LPTray::LPTray() : QSystemTrayIcon(){
  //Start up the log file watcher
  QString logfile = "/var/log/lpreserver.log";
  watcher = new QFileSystemWatcher();
	if(!QFile::exists(logfile)){ system( QString("touch "+logfile).toUtf8() ); }
	watcher->addPath(logfile);
	connect(watcher, SIGNAL(fileChanged(QString)),this,SLOT(slotNewLogMessage()) );
  //Setup the context menu
  menu = new QMenu;
	menu->addAction(new QAction(QIcon(":/images/application-exit.png"),tr("Close Life Preserver Tray"),this) );
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(slotClose()) );
  this->setContextMenu(menu);
  //Setup the animated icon timer
  timer = new QTimer();
	timer->setInterval(300);
	connect(timer, SIGNAL(timeout()), this, SLOT(displayWorkingIcon()) );
  //Setup initial icon for the tray
  this->setIcon( QIcon(":/images/tray-icon-idle.png") );
  //Create the configuration GUI
  GUI = new mainUI();
  //connect other signals/slots
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayClicked(QSystemTrayIcon::ActivationReason)) );
}

LPTray::~LPTray(){
  delete watcher;
  delete menu;
  delete timer;
}

// ===============
//  PRIVATE FUNCTIONS
// ===============
void LPTray::parseLogMessage(QString log){
  //Divide up the log into it's sections
  QString timestamp = log.section(":",0,2).simplified();
  QString message = log.section(":",3,3).toLower().simplified();
  QString dev = log.section(":",4,4).simplified(); //dataset/snapshot/nothing
  //Now decide what to do/show because of the log message
  if(message.contains("creating snapshot")){
    dev = message.section(" ",-1).simplified();
    this->showMessage( "", QString(tr("Creating snapshot for %1")).arg(dev), QSystemTrayIcon::Information, 1000);
    //Just set the standard idle icon
    this->setIcon( QIcon(":/images/tray-icon-idle.png") );   
//  }else if(message.contains("pruning snapshot")){
  }else if(message.contains("starting replication")){
    startWorkingIcon();
  }else if(message.contains("finished replication")){
    stopWorkingIcon();
    dev = message.section(" ",-1).simplified();
    this->showMessage( "", QString(tr("Finished replication for %1")).arg(dev), QSystemTrayIcon::Information, 1000);
  }else{
    //Just set the standard idle icon
    this->setIcon( QIcon(":/images/tray-icon-idle.png") );    
  }
}

void LPTray::startWorkingIcon(){
  wNum = 1; //start on the first image
  timer->start();
}

void LPTray::stopWorkingIcon(){
  timer->stop();
  this->setIcon( QIcon(":/images/tray-icon-idle.png") );      
}

// ===============
//     PRIVATE SLOTS
// ===============
void LPTray::slotNewLogMessage(){
  //get the last line from the log file
  QProcess proc;
  proc.setProcessChannelMode(QProcess::MergedChannels);
  proc.start("tail /var/log/lpreserver.log");
  proc.waitForFinished();
  QString log = proc.readAllStandardOutput();
  //Now parse the log line and do stuff with it
  parseLogMessage(log);
}

void LPTray::slotTrayClicked(QSystemTrayIcon::ActivationReason reason){
  if(reason == QSystemTrayIcon::Trigger){ startGUI(); }
  else if( reason == QSystemTrayIcon::Context){
    this->contextMenu()->popup(QCursor::pos());
  }
}

void LPTray::slotClose(){
  exit(0);
}

void LPTray::slotSingleInstance(){
  this->show();
}

void LPTray::startGUI(){
  //Start up the GUI
    GUI->setupUI();
    GUI->raise();
    GUI->show();
}

void LPTray::displayWorkingIcon(){
  QString ico = ":/images/tray-icon-active"+QString::number(wNum)+".png";
  this->setIcon(QIcon(ico));
  if(wNum == 16){ wNum = 1; } //go back to the beginning of the loop
  else{ wNum++; }
}

#include "pbitools.h"


/*  Backend Toolbox class to use when reading/writing pbi data
 * 
*/

QStringList currPBIlist, availPBIlist, currPBImetaList;
QString saveBuildRepo, savePublishRepo;

PBIT::PBIT(QString localDir, QString buildRepo, QString distRepo){
}

PBIT::~PBIT(){
}

// ***************************************
// ******** PUBLIC FUNCTIONS *************
// ***************************************

void PBIT::setupServers(QString localDir, QString buildRepo, QString distRepo,bool lhapprove){
  if(!localDir.endsWith("/")){localDir.append("/");}
  if(!buildRepo.endsWith("/")){buildRepo.append("/");}
  if(!distRepo.endsWith("/")){distRepo.append("/");}
  saveLocalDir = localDir;
  saveBuildRepo = buildRepo;
  saveDistRepo = distRepo;
  // - Make sure the build/dist local directories exist
  QDir(localDir) dir;
  if(!dir.exists()){ dir.mkpath(localDir); }
  if(!dir.exists(buildRepo)){ dir.mkdir(buildRepo); }
  if(!dir.exists(distRepo)){ dir.mkdir(distRepo); }
  approveFromLocalHost=lhapprove;
}

bool PBIT::approvePBI(QString name, QString version, QString arch){

}

bool PBIT::downloadFile(QString URL, QProgressBar* progressBar){
    if(!URL.startsWith("http://")){ URL.prepend("http://"); }
    //Save the appropriate variables for later use
    saveDLProgressBar = progressBar;

    //Start the Download
    QNetworkReply *currentDL;
    currentDL = netmanager->get(QNetworkRequest(QUrl(URL)));
    //Make the Progress Bar visible and start updating    
    progressBar->setText(tr("Downloading")+": "+URL);
    progressBar->setValue(0);
    progressBar->setVisible(TRUE);
    connect(currentDL, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDLProgress(qint64,qint64)) );
    //Wait for the download to finish
    QEventLoop loop;
    connect(currentDL, SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
    //See if the Download was successful
    qDebug() << " - Download completion code:" << currentDL->error();
    if( currentDL->error() != QNetworkReply::NoError){
      QMessageBox::warning(this, tr("Download Failed"),tr("Error downloading file")+": "+URL+"\n"+currentDL->error());
      return FALSE;
    }
    //Download Succesful, save it to file
      //Determine where to save the file
      QString filename = URL.section("/",-1);
      QString fileDir=QDir::homePath()+"/Downloads/";
      if(filename.endsWith(".pbi")){} //use current downloads folder
      else if(URL.contains(saveBuildRepo)){ fileDir=saveLocalDir+saveBuildRepo; }
      else if(URL.contains(saveDistRepo)){ fileDir=saveLocalDir+saveDistRepo; }
      QDir(fileDir) dir;
      if(!dir.exists()){dir.mkpath(fileDir);} //make sure the download directory exists.
    QFile file(fileDir+filename);
    if( !file.open(QIODevice::WriteOnly)){   //if file could not be opened
      QMessageBox::warning(this, tr("Saving Failed"),tr("Invalid Permissions to save the file")+": "+fileDir+filename);
      currentDL->deleteLater();
      return FALSE;
    }
    file.write(currentDL->readAll());
    file.close();
    //Saving the file successful, delete the downloaded data from memory
    currentDL->deleteLater();
    //Disable the progress bar
    progressBar->setVisible(FALSE);
    return TRUE;
}

// *****************************************
// ********* PRIVATE FUNCTIONS *************
// *****************************************

bool PBIT::loadPBIindexLine(QString line){
  //Parse out the info from the pbi-index-9 file and add it to the internal lists
  QString name = line.section(":",0,0,QString::sectionSkipEmpty);
  QString arch = line.section(":",1,1,QString::sectionSkipEmpty);
  QString version = line.section(":",2,2,QString::sectionSkipEmpty);
  //section 3 is the MD5
  QString date = line.section(":",4,4,QString::sectionSkipEmpty).section(" ",0,0,QString::sectionSkipEmpty);
  //Convert the date into human readable format (MM/DD/YYYY)
  // -- not done yet
  QString filepath = line.section(":",5,5,QString::sectionSkipEmpty);
  QString size = line.section(":",9,9,QString::sectionSkipEmpty); //in bytes
  //convert the size into human readable format (MB)
  if(!size.isEmpty() ){ 
    float num = size.toFloat()/1024; //in MB
    num = int(num*100 + 0.5)/100.0; //round to 2 decimel places
    size.setNum( num ).append(" MB"); 
  }
  //Add this info into the internal lists
  QString entry = name+":::"+arch+":::"+version+":::"+date+":::"+size+":::"+filepath;
  currPBIlist << entry;
}

bool PBIT::loadPBImetaLine(QString line){
  
  // Get the PBI information
  if(line.startsWith("App="){
    tmp = line.remove(0,4); //remove the "App=" at the beginning of the line
    name = tmp.section(';', 0, 0);
    cat = tmp.section(';', 1, 1);
    icon = tmp.section(';', 2, 2);
      //tmpicon = icon.remove(0, icon.lastIndexOf("."));
      //icon = "/var/db/pbi/repo-icons/" + pbiRepos->getMd5(r) + "-" + name + tmpicon;
    auth = tmp.section(';', 3, 3);
    url = tmp.section(';', 4, 4);
    lic = tmp.section(';', 5, 5);
    type = tmp.section(';', 6, 6);
    keys = tmp.section(';', 7, 7);
    desc = tmp.section(';', 8, 8);
    desc = getTranslation(pbiRepos->getID(r), "App", name, desc);

    needRoot = tmp.section(';', 9, 9); //YES or NO
    if(needRoot!="YES"){ needRoot="NO"; } 
    QString entry = name+":::"+cat+":::"+icon+":::"+auth+":::"+url+":::"+lic+":::"+type+":::"+keys+":::"+desc+":::"+needRoot;
    currPBImetaList << entry;
  }

}

void PBIT::updateDLProgress(qint64 bytesReceived, qint64 bytesTotal){
  //Determine the percentage
  int percent = (bytesReceived*100)/bytesTotal;
  //Update the progress bar
  saveDLProgressBar->setValue(percent);
  saveDLProgressBar->update();
}
}

#include "downloadWidget.h"

DownloadWidget::DownloadWidget(QWidget* parent) : QWidget(parent)
{
  //Create the layout
  QHBoxLayout* layout = new QHBoxLayout();
  //Create the widgets	
  progressBar = new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
  pushClose = new QToolButton();
    pushClose->setDefaultAction( new QAction(QIcon(":/png/icons/close.png"),tr("Close"),this) );
  labelFile = new QLabel();	
  //Add the widgets to the layout
  layout->addWidget(labelFile);
  layout->addWidget(labelSpeed);
  layout->addWidget(progressBar);
  layout->addWidget(pushCancel);
  //Now set the layout for the main widget
  this->setLayout(layout);

  //Setup the network framework
  manager = new QNetworkAccessManager(this);
  //connect the appropriate signals/slots
  connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotDLFinished(QNetworkReply*)) );
  connect(pushClose,SIGNAL(triggered(QAction*)),this,SLOT(slotCancel(QAction*)));

  //Other things to setup
  timer = new QTime();

}

DownloadWidget::~DownloadWidget(){
}

void DownloadWidget::setDownloadURL(QString url){
	DLURL = url;
	DLFILENAME = url.section("/",-1);
	labelFile->setText( DLFILENAME +"("+tr("Not Started")+")" );
}

void DownloadWidget::setFilePath(QString path){
	DLFILEPATH = path;
}

void DownloadWidget::startDownload(){
    if( !DLURL.isEmpty() ){
      //Make sure we don't have duplicate connections
      currentDL->disconnect();
      //Start the download
      currentDL = manager->get(QNetworkRequest(QUrl(DLURL)));
      timer->start();
      tLast = timer->elapsed(); //initial value for timer comparison
      bytesLast = 0; //initial value for download speed calc
      //Connect to the download progress monitor
      connect(currentDL, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgressBar(qint64,qint64)) );
    }else{
      qDebug() << "DownloadWidget: Ignoring attempt to download from an unset URL";
    }
}

void DownloadWidget::slotCancel(QAction* act){
    //Cancel any currently running downloads.
    if(currentDL->isRunning()){ currentDL->abort(); }
    //Now destroy this widget
    this->close();
}

void DownloadWidget::slotDLFinished(QNetworkReply* reply){
  //See if the Download was successful
  qDebug() << "Ports download completion code:" << reply->error();
  if( reply->error() != QNetworkReply::NoError){
    this->setToolTip(tr("Error during download. Please try again later."));
    return;
  }
  //Download Succesful, save it to file
  QFile file(DLFILEPATH);
  if( !file.open(QIODevice::WriteOnly)){   //if file could not be opened
    QString newpath = QDir::homePath()+"/"+DLFILENAME;
    qDebug() << "DownloadWidget: Invalid permissions to save file in designated location:"<<DLFILEPATH;
    qDebug() << "  - Saving file to user's home directory instead:"<< newpath;
    file.setFileName(newpath);
    if(!file.open(QIODevice::WriteOnly)){
      this->setToolTip(tr("Error: The file could not be saved to disk"));
      return;
    }
  }
  file.write(reply->readAll());
  file.close();
  //Set the tooltip
  this->setToolTip( QString(tr("Successfully downloaded to %1")).arg(file->fileName()) );
  //Saving the file successful, delete the downloaded data from memory
  reply->deleteLater();
    //Emit the finished signal
    emit downloadFinished();
}

void DownloadWidget::updateProgressBar(qint64 bytesReceived, qint64 bytesTotal){
  //Do the timer stuff first to prevent errors
  int ct = timer->elapsed; 
  int tdiff = ct - tLast; tLast = ct; //get the time difference (milliseconds)
  //Estimate the download speed
  int bytesdiff = bytesReceived - bytesLast; bytesLast = bytesReceived;
  int kbps = bytesdiff/tdiff; // b/ms = kb/s
  //Determine the percentage
  int percent = 0;
  if(bytesTotal != 0){
    percent = (bytesReceived*100)/bytesTotal;
  }
  //Update the progress bar
  ui->progressBar->setValue(percent);
  ui->progressBar->update();
  //Update the speed indicator
  labelFile->setText( DLFILENAME + "("+QString::number(kbps)+" kb/s)" );
  
}
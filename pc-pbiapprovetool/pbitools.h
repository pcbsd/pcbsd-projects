#ifndef _PBIAPPROVETOOLS_H
#define _PBIAPPROVETOOLS_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QApplication>
#include <pcbsd-utils.h>  //libpcbsd utilities

class PBIT : public QObject {

public:
  //Constructors/destructors
  PBIT();
  ~PBIT();
  //Variables
  bool approveFromLocalHost;
  //Main functions
  void setupServers(QString,QString,QString,bool);
  bool approvePBI(QString,QString,QString);
  bool removePBI(QString,QString,QString);
  static void downloadFile(QString,QProgressBar*);

private:
  //Variables
  QString saveLocalDir, saveBuildRepo, saveDistRepo, indexfile, metafile;


  //Functions
  void loadPBIindexLine(QString);
  void loadPBImetaLine(QString);
  void updateDLProgress(qint64,qint64);
  //Variables used in downloading/uploading files
  QNetworkAccessManager *netmanager
  QProgressBar* saveDLProgressBar;
  
private slots:


};

#endif


/*
 Sub-classed widget for a self-managed file downloader
*/

#ifndef DOWNLOAD_WIDGET_H
#define DOWNLOAD_WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QProgressBar>
#include <QToolButton>
#include <QAction>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class DownloadWidget: public QWidget
{
	Q_OBJECT

  public:
	DownloadWidget(QWidget* parent = 0);
	~DownloadWidget();
	
	//Main Interactions
        void setDownloadURL(QString);
        void setFilePath(QString);
        void startDownload();
        bool isFinished();
	
  private:
	QProgressBar* progressBar;
	QToolButton* pushClose;
	QLabel* labelFile;
	QString DLURL, DLFILEPATH, DLFILENAME;
        QNetworkReply *currentDL;
	QNetworkAccessManager *manager;
        QTime* timer;
        int tLast;
	qint64 bytesLast;
        int status;

  private slots:
	void slotCancel(QAction*);
	void slotDLFinished(QNetworkReply*);
        void updateProgressBar(qint64, qint64);
	
  signals:
	void downloadFinished();

};
#endif

#ifndef _LP_TRAY_H
#define _LP_TRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QFileSystemWatcher>
#include <QString>
#include <QTimer>
#include <QProcess>
#include <QFile>

#include "mainUI.h"

class LPTray : public QSystemTrayIcon{
		Q_OBJECT
public:
	LPTray();
	~LPTray();

private:
	QFileSystemWatcher *watcher;
	QMenu *menu;
	QTimer *timer;
	mainUI *GUI;
	int wNum; //internal tracking of which frame of the icon animation we are on

	void parseLogMessage(QString);
	void startWorkingIcon();
	void stopWorkingIcon();

private slots:
	void slotNewLogMessage();
	void slotTrayClicked(QSystemTrayIcon::ActivationReason);
	void slotClose();
	void slotSingleInstance();
	void startGUI();
	void displayWorkingIcon();
};

#endif

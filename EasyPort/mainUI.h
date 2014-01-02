#ifndef _EASYPORT_MAINUI_H
#define _EASYPORT_MAINUI_H

#include <QMainWindow>
#include <QString> 
#include <QStringList>
#include <QLineEdit>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>

#include "portUtils.h"
#include "portStruct.h"
#include "newPortWizard.h"

namespace Ui{
  class MainUI;	
};

class MainUI : public QMainWindow{
	Q_OBJECT
public:
	MainUI(QWidget* parent = 0);
	~MainUI();


private:
	Ui::MainUI *ui;
	QLineEdit *portOpened;
	PortStruct *PORT;

private slots:
	void slotSingleInstance();
	void updateGUI(QString tab = "all");

	//Menu functions
	void on_actionPreferences_triggered();
	void on_actionClose_triggered();
	void on_actionPackage_Current_Port_triggered();
	void on_actionSubmit_FreeBSD_Port_triggered();
	void on_actionPorter_s_Handbook_triggered();
	void on_actionFreshports_triggered();
	void on_actionRedports_triggered();
	//Toolbar functions
	void slotNewPort();
	void slotLoadPort();


};

#endif

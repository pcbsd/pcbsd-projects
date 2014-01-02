#ifndef _EASYPORT_NEW_PORT_WIZARD_H
#define _EASYPORT_NEW_PORT_WIZARD_H

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QMessageBox>
#include <QUrl>
#include <QDesktopServices>
#include <QMenu>
#include <QAction>

#include "portUtils.h"

namespace Ui{
	class NewPortWizard;
};

class NewPortWizard : public QDialog{
	Q_OBJECT
public:
	NewPortWizard(QWidget *parent = 0, QString pDir = "/usr/ports");
	~NewPortWizard();
	//Input functions
	void setNewPortBaseDir(QString);
	//Output functions
	bool portCreated;
	QString newPortDir;

private:
	Ui::NewPortWizard *ui;
	QString portdir, fbsdPortsDir;
	QMenu *menu_mastersite, *menu_distfile, *menu_category;

	void setupMenu(QMenu *menu, QString variable);

private slots:
	//general GUI setup
	void slotGetOptionHelps(); //"help" options for quick usage
	void slotCheckInputs(); //check whether the required fields are filled yet
	//Accept/cancel buttons
	void slotCreatePort();
	void slotCancel();
	//Toolbutton actions
	void slotOpenWebsite();
	void slotSetMasterSite(QAction*);
	void slotSetDistFile(QAction*);
	void slotSetCategory(QAction*);
};

#endif

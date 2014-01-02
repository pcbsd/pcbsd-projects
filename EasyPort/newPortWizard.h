#ifndef _EASYPORT_NEW_PORT_WIZARD_H
#define _EASYPORT_NEW_PORT_WIZARD_H

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QMessageBox>

#include "portUtils.h"

namespace Ui{
	class NewPortWizard;
};

class NewPortWizard : public QDialog{
	Q_OBJECT
public:
	NewPortWizard(QWidget *parent = 0);
	~NewPortWizard();
	//Input functions
	void setNewPortBaseDir(QString);
	//Output functions
	bool portCreated;
	QString newPortDir;

private:
	Ui::NewPortWizard *ui;
	QString portdir;

private slots:
	void slotGetOptionHelps(); //"help" options for quick usage
	void slotCheckInputs(); //check whether the required fields are filled yet
	void slotCreatePort();
	void slotCancel();
};

#endif

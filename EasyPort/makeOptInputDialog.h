#ifndef _EASYPORT_MAKE_OPT_INPUT_DIALOG_H
#define _EASYPORT_MAKE_OPT_INPUT_DIALOG_H

#include <QDialog>
#include <QString>
#include <QMenu>
#include <QAction>

#include "portUtils.h"

namespace Ui{
	class MakeOptInputDialog;
};

class MakeOptInputDialog : public QDialog{
	Q_OBJECT
public:
	MakeOptInputDialog(QWidget *parent = 0);
	~MakeOptInputDialog();

	//Input functions
	void setPortsDir(QString pdir);
	void setupOpt(QString var, QString vartype, QString info);

	//Output variables
	bool accepted;
	QString value;

private:
	QString portsdir, variable, type;
	bool useLine;
	QMenu *menu_hints;
	Ui::MakeOptInputDialog *ui;

private slots:
	void slotCancel();
	void slotAccept();
	void slotHintSelected(QAction*); //when a menu item is selected
	void slotHintClicked(); //when no menu and the button is clicked
};

#endif

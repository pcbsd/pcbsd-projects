#ifndef _LP_MAIN_H
#define _LP_MAIN_H

#include <QMainWindow>
#include <QComboBox>
#include <QToolButton>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QRadioButton>
#include <QWidgetAction>
#include <QFileSystemModel>

#include "LPBackend.h"
#include "LPContainers.h"
#include "LPGUtils.h"

namespace Ui{
	class LPMain;
};

class LPMain : public QMainWindow{
	Q_OBJECT
public:
	LPMain(QWidget *parent = 0);
	~LPMain();

public slots:
	void slotSingleInstance();

private:
	Ui::LPMain *ui;
	QRadioButton *viewBasic, *viewAdvanced;
	QFileSystemModel *fsModel;
	bool poolSelected;
	LPDataset POOLDATA;

private slots:
	void updatePoolList();  //re-load available pools
	void viewChanged();
	void updateTabs();      //load current pool info and update tabs
	void updateDataset();  //restore dataset changed
	void updateSnapshot(); //selected snapshot changed
	void nextSnapshot();
	void prevSnapshot();
	void restoreFiles();

protected:
	
};

#endif

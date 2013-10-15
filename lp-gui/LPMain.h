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
#include <QInputDialog>

#include "LPBackend.h"
#include "LPContainers.h"
#include "LPGUtils.h"
#include "LPWizard.h"

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

	void showErrorDialog(QString title, QString message, QString errors);

private slots:
	void updatePoolList();  //re-load available pools
	void viewChanged();
	void updateTabs();      //load current pool info and update tabs
	void updateDataset();  //restore dataset changed
	void updateSnapshot(); //selected snapshot changed
	void nextSnapshot();
	void prevSnapshot();
	void setFileVisibility();
	void restoreFiles();
	void openConfigGUI();
	void openBackupGUI();
	// -- Menu Actions --
	//File Menu
	void menuAddPool(QAction*);
	void menuRemovePool(QAction*);
	void menuSaveSSHKey();
	void menuCloseWindow();
	//Classic Backups
	void menuCompressHomeDir();
	void menuExtractHomeDir();
	//Disk Menu
	void menuAddDisk();
	void menuRemoveDisk();
	void menuOfflineDisk();
	void menuStartScrub();
	//Snapshots Menu
	void menuNewSnapshot();
	void menuRemoveSnapshot(QAction*);

protected:
	
};

#endif

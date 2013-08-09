#ifndef _LP_MAINUI_H
#define _LP_MAINUI_H

#include <QMainWindow>
#include <QHash>
#include <QStringList>
#include <QString>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QCloseEvent>

#include "LPBackend.h"

//TERMINOLOGY NOTE: DS=DataSet, SNAP=Snapshot

namespace Ui {
    class mainUI;
}

class mainUI : public QMainWindow{
	Q_OBJECT
	
public:
	explicit mainUI(QWidget* parent = 0);
	~mainUI();
	void setupUI();

private:
	Ui::mainUI *ui;
	QHash<QString,QStringList> HLIST;
	QStringList RLIST; //datasets that have replication enabled
	QMenu *revMenu, *brMenu; //revert/browse menu's

	void updateHash(QString ds="");
	void updateUI();
	void updateMenus();
	QString getSelectedDS();

private slots:
	void on_treeWidget_itemSelectionChanged();
	void on_tool_add_clicked();
	void on_tool_browse_clicked();
	void on_tool_config_clicked();
	void on_tool_copy_clicked();
	void on_tool_remove_clicked();
	void on_tool_revert_clicked();

	void slotRevertToSnapshot(QAction*);
	void slotBrowseSnapshot(QAction*);

protected:
	void closeEvent(QCloseEvent*);

};
#endif

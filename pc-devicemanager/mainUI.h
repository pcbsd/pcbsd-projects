#ifndef _PC_DEVICEMANAGER_H
#define _PC_DEVICEMANAGER_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QFile>

namespace Ui{
	class mainUI;
};

class mainUI : public QMainWindow{
	Q_OBJECT
public:
	mainUI();
	~mainUI();

public slots:
	void slotSingleInstance();

private:
	Ui::mainUI *ui;
	void updateRemovableDeviceList();

private slots:
	void on_actionQuit_triggered();
	void on_combo_rd_devices_currentIndexChanged(QString);
	void on_combo_rd_changelist_currentIndexChanged(int);
	void on_tool_rd_reformatdevice_clicked();
};
#endif

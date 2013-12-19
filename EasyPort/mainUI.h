#ifndef _EASYPORT_MAINUI_H
#define _EASYPORT_MAINUI_H

#include <QMainWindow>
#include <QString> 
#include <QStringList>


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
private slots:
	void slotSingleInstance();
};

#endif

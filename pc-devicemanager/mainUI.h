#ifndef _PC_DEVICEMANAGER_H
#define _PC_DEVICEMANAGER_H

#include <QMainWindow>

namespace Ui{
	class mainUI;
};

class mainUI : public QMainWindow{
	Q_OBJECT
public:
	mainUI();
	~mainUI();

private:
	Ui::mainUI *ui;

private slots:
	
};
#endif
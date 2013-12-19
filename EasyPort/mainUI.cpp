#include "mainUI.h"
#include "ui_mainUI.h"

MainUI::MainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainUI){
  ui->setupUi(this);
	
}

MainUI::~MainUI(){
	
}

void MainUI::slotSingleInstance(){
  this->show();
  this->raise();
}

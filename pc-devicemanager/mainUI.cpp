#include "mainUI.h"
#include "ui_mainUI.h"

mainUI::mainUI() : QMainWindow(), ui(new Ui::mainUI){
  ui->setupUi(this); //load the designer template
	
}

mainUI::~mainUI(){
	
}

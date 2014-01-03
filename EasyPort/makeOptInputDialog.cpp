#include "makeOptInputDialog.h"

MakeOptIntputDialog::MakeOptInputDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MakeOptInputDialog){
  ui->setupUi(this); //load the designer file
  //Set the initial values
  accepted = false;
  value.clear();
  menu_hints = new QMenu(this);
  connect(menu_hints, SIGNAL(triggered(QAction*)), this, SLOT(slotHintSelected(QAction*)) );
}

MakeOptIntputDialog::~MakeOptInputDialog(){
  delete menu_hints;
}

// ==========
//      INPUTS
// ==========
void MakeOptIntputDialog::setPortsDir(QString pdir){
  portsdir = pdir;	
}

void MakeOptIntputDialog::setupOpt(QString var, QString vartype){
  variable = var;
  type = vartype;
  //Now adjust the UI elements appropriately
  
}

// ==============
//     PRIVATE SLOTS
// ==============
void MakeOptIntputDialog::slotCancel(){
	
}

void MakeOptIntputDialog::slotAccept(){
	
}

void MakeOptIntputDialog::slotHintSelected(QAction*){
	
}

void MakeOptIntputDialog::slotHintClicked(){
	
}

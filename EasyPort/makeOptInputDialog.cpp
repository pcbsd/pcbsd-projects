#include "makeOptInputDialog.h"
#include "ui_makeOptInputDialog.h"

MakeOptInputDialog::MakeOptInputDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MakeOptInputDialog){
  ui->setupUi(this); //load the designer file
  //Set the initial values
  accepted = false;
  value.clear();
  menu_hints = new QMenu(this);
  connect(menu_hints, SIGNAL(triggered(QAction*)), this, SLOT(slotHintSelected(QAction*)) );
  connect(ui->push_ok, SIGNAL(clicked()), this, SLOT(slotAccept()) );
  connect(ui->push_cancel, SIGNAL(clicked()), this, SLOT(slotCancel()) );
}

MakeOptInputDialog::~MakeOptInputDialog(){
  delete menu_hints;
}

// ==========
//      INPUTS
// ==========
void MakeOptInputDialog::setPortsDir(QString pdir){
  portsdir = pdir;	
}

void MakeOptInputDialog::setupOpt(QString var, QString vartype, QString info){
  variable = var;
    ui->label_var->setText(var);
    ui->label_var->setToolTip(info);
  type = vartype;
  //Now adjust the UI elements appropriately
  if(type == "text" || type == "url" || type == "cats"){ useLine=true; }
  else{ useLine = false; }
  ui->line_val->setVisible(useLine);
  ui->text_val->setVisible(!useLine);
  //Now get any EasyPort recommendations
  if(type.contains("port")){
    //Do not use the menu
    connect(ui->tool_help, SIGNAL(clicked()), this, SLOT(slotHintClicked()) );
  }else{
    QStringList hints;
    if(type == "cats"){
      hints = PortUtils::findPortCategories(portsdir);
    }else{
      hints = PortUtils::getMakefileOptHints(variable);
    }
    for(int i=0; i<hints.length(); i++){
      QAction *act = new QAction(hints[i].section(":::",0,0), this);
	    act->setWhatsThis(hints[i].section(":::",1,50));
      menu_hints->addAction(act);
    }
    ui->tool_help->setMenu(menu_hints); //make sure we use the menu for these hints
    ui->tool_help->setEnabled(!menu_hints->isEmpty());
  }
}

// ==============
//     PRIVATE SLOTS
// ==============
void MakeOptInputDialog::slotCancel(){
  accepted = false;
  this->close();
}

void MakeOptInputDialog::slotAccept(){
  if(useLine){ value = ui->line_val->text().simplified(); }
  else{ value = ui->text_val->toPlainText(); }
  accepted = true;
  this->close();
}

void MakeOptInputDialog::slotHintSelected(QAction* act){
  QString val = act->whatsThis();
  if(val.isEmpty()){ val = act->text(); }
  
  if(useLine){ 
    if(type=="cats"){
      QStringList cats = ui->line_val->text().split(" ");
      cats << val;
      cats.removeDuplicates();
      ui->line_val->setText(cats.join(" "));
    }else{
      ui->line_val->insert(val);
    }
  }else{
    ui->text_val->insertPlainText(val);
  }
}

void MakeOptInputDialog::slotHintClicked(){
	
}

#include "newPortWizard.h"
#include "ui_newPortWizard.h"

NewPortWizard::NewPortWizard(QWidget *parent, QString pDir) : QDialog(parent), ui(new Ui::NewPortWizard){
  ui->setupUi(this);
  portCreated = false; //output variable
  newPortDir.clear(); //output variable
  portdir.clear(); //internal variable
  fbsdPortsDir = pDir; //internal variable
  //Make sure create button is disabled initially
  ui->push_create->setEnabled(false);
  connect(ui->push_create, SIGNAL(clicked()), this, SLOT(slotCreatePort()) );
  connect(ui->push_cancel, SIGNAL(clicked()), this, SLOT(slotCancel()) );
	
  //Setup the required inputs for value checking
  connect(ui->line_name, SIGNAL(textChanged(QString)), this, SLOT(slotCheckInputs()) );
  connect(ui->line_version, SIGNAL(textChanged(QString)), this, SLOT(slotCheckInputs()) );
  connect(ui->line_summary, SIGNAL(textChanged(QString)), this, SLOT(slotCheckInputs()) );
  connect(ui->line_mastersite, SIGNAL(textChanged(QString)), this, SLOT(slotCheckInputs()) );
  connect(ui->line_maintainer, SIGNAL(textChanged(QString)), this, SLOT(slotCheckInputs()) );
  connect(ui->line_category, SIGNAL(textChanged(QString)), this, SLOT(slotCheckInputs()) );
  //Setup all the other signals/slots
  connect(ui->tool_website, SIGNAL(clicked()), this, SLOT(slotOpenWebsite()) );
  //Now try to add all the suggestions
  slotGetOptionHelps();
}

NewPortWizard::~NewPortWizard(){
	
}

void NewPortWizard::setNewPortBaseDir(QString dir){
  portdir = dir;
}

// ==============
//            PRIVATE
// ==============
void NewPortWizard::setupMenu(QMenu *menu, QString variable){
  QStringList helps;
  if(variable=="CATEGORY"){
    helps = PortUtils::findPortCategories(fbsdPortsDir);
  }else{
    helps = PortUtils::getMakefileOptHints(variable);
  }
  menu->clear();
  for(int i=0; i<helps.length(); i++){
    QAction *act = new QAction(helps[i].section(":::",0,0), this);
      act->setWhatsThis(helps[i].section(":::",1,50));
    menu->addAction(act);
  }
}
// ==============
//     PRIVATE SLOTS
// ==============


void NewPortWizard::slotGetOptionHelps(){
  //MASTER_SITE suggestions
  menu_mastersite = new QMenu();
  setupMenu(menu_mastersite, "MASTER_SITES");
  connect(menu_mastersite, SIGNAL(triggered(QAction*)), this, SLOT(slotSetMasterSite(QAction*)) );
  ui->tool_mastersite->setMenu(menu_mastersite);
  ui->tool_mastersite->setEnabled(!menu_mastersite->isEmpty());
  //DISTFILE suggestions
  menu_distfile = new QMenu();
  setupMenu(menu_distfile, "DISTFILES");
  connect(menu_distfile, SIGNAL(triggered(QAction*)), this, SLOT(slotSetDistFile(QAction*)) );
  ui->tool_distfile->setMenu(menu_distfile);
  ui->tool_distfile->setEnabled(!menu_distfile->isEmpty());
  //CATEGORY suggestions
  menu_category = new QMenu();
  setupMenu(menu_category, "CATEGORY");
  connect(menu_category, SIGNAL(triggered(QAction*)), this, SLOT(slotSetCategory(QAction*)) );
  ui->tool_category->setMenu(menu_category);
  ui->tool_category->setEnabled(!menu_category->isEmpty());
}

void NewPortWizard::slotCheckInputs(){
  bool invalid = (ui->line_name->text().simplified().isEmpty() 
	|| ui->line_version->text().simplified().isEmpty()
	|| ui->line_summary->text().simplified().isEmpty()
	|| ui->line_mastersite->text().simplified().isEmpty()
	|| ui->line_maintainer->text().simplified().isEmpty()
	|| ui->line_category->text().simplified().isEmpty() );
	
  ui->push_create->setEnabled(!invalid);	  
}

void NewPortWizard::slotCreatePort(){
  //Go through all the inputs and create the port
  QStringList vars, vals;
  //All the required MAKEFILE variables
  // - PORTNAME is strict requirement for the function -- used below
  vars << "PORTVERSION"; vals << ui->line_version->text().simplified();
  vars << "CATEGORIES"; vals << ui->line_category->text().simplified();
  vars << "COMMENT"; vals << ui->line_summary->text().simplified();
  vars << "MASTER_SITES"; vals << ui->line_mastersite->text();
  vars << "MAINTAINER"; vals << ui->line_maintainer->text().simplified();
  //Now optional MAKEFILE variables
  if(!ui->line_masterSub->text().isEmpty()){ vars << "MASTER_SITE_SUBDIR"; vals << ui->line_masterSub->text(); }
  if(!ui->line_distfile->text().isEmpty()){ vars << "DISTFILES"; vals << ui->line_distfile->text(); }
  //Create the MAKEFILE
  QString name = ui->line_name->text().simplified();
  QString portPath = portdir+"/"+name;
  QStringList contents = PortUtils::generateNewMakefile(name, vars, vals);
  portCreated = PortUtils::createFile(portPath+"/Makefile", contents);
  if(!portCreated){
    QMessageBox::warning(this, tr("Error"), QString(tr("Could not create the port: %1")).arg(portPath) );
    return;
  }
  //Generate pkg-descr based on inputs
  contents = PortUtils::generatePkgDescr(ui->text_description->toPlainText(), ui->line_website->text());
  if(!contents.isEmpty()){
    PortUtils::createFile(portPath+"/pkg-descr", contents);
  }
  // Don't generate the pkg-plist - this will be empty anyway for the moment
  // Don't generate the distinfo here - let the main GUI do it here in a moment
  newPortDir = portPath; //fill output variable
  this->close();
}

void NewPortWizard::slotCancel(){
  portCreated = false;
  this->close();
}

void NewPortWizard::slotOpenWebsite(){
  QString url = ui->line_website->text();
  bool ok = QDesktopServices::openUrl( QUrl(url) );
  if(!ok){
    QMessageBox::warning(this, tr("Error Opening URL"), QString(tr("Could not open the application website URL: %1")).arg(url) );
  }
}

void NewPortWizard::slotSetMasterSite(QAction* act){
  ui->line_mastersite->insert(act->whatsThis());
}

void NewPortWizard::slotSetDistFile(QAction* act){
  ui->line_distfile->insert(act->whatsThis());	
}

void NewPortWizard::slotSetCategory(QAction* act){
  QStringList cats = ui->line_category->text().split(" ");
  cats << act->text();
  cats.removeDuplicates();
  ui->line_category->setText( cats.join(" ") );	
}

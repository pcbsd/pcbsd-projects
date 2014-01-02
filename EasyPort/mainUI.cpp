#include "mainUI.h"
#include "ui_mainUI.h"

MainUI::MainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainUI){
  ui->setupUi(this);
  PORT = new PortStruct(); //backend data structure
	
  //Setup the line edit on the toolbar
  portOpened = new QLineEdit(this);
  ui->toolBar->addWidget(portOpened);

  //Connect the signals/slots
  connect(ui->actionNewPort, SIGNAL(triggered(bool)), this, SLOT(slotNewPort()) );
  connect(ui->actionLoad_Port, SIGNAL(triggered(bool)), this, SLOT(slotLoadPort()) );
}

MainUI::~MainUI(){
	
}

void MainUI::slotSingleInstance(){
  this->show();
  this->raise();
}

void MainUI::updateGUI(QString tab){
if(tab=="makefile" || tab=="all"){
  if(ui->list_make_config->count() <= 0){
    //Fill the list with the available options
    QStringList opts = PortUtils::getMakefileConfigOpts();
    for(int i=0; i<opts.length(); i++){
      QListWidgetItem *it = new QListWidgetItem(opts[i].section(":::",0,0));
	it->setToolTip(opts[i].section(":::",2,50)); //description of the variable
	it->setWhatsThis(opts[i].section(":::",1,1)); //type of variable
      ui->list_make_config->addItem(it);
      ui->tool_make_addconf->setVisible(false); //nothing selected yet
      ui->tool_make_replaceconf->setEnabled(false); //nothing selected yet
    }
  }
  //Load the makefile for the current port
  QStringList contents = PortUtils::readFile(PORT->portPath()+"/Makefile");
  ui->text_makefile->clear();
  ui->text_makefile->setPlainText(contents.join("\n"));
  ui->tool_make_save->setEnabled(false); //nothing changed yet
}
if(tab=="distinfo" || tab=="all"){
   //Load the distinfo for the current port
   QStringList contents = PortUtils::readFile(PORT->portPath()+"/distinfo");
   ui->text_distinfo->clear();
   ui->text_distinfo->setPlainText(contents.join("\n"));
   ui->tool_dist_save->setEnabled(false); //nothing changed yet
}
if(tab=="pkg-plist" || tab=="all"){
   //Load the distinfo for the current port
   QStringList contents = PortUtils::readFile(PORT->portPath()+"/pkg-plist");
   ui->text_pkgplist->clear();
   ui->text_pkgplist->setPlainText(contents.join("\n"));
   ui->tool_plist_save->setEnabled(false); //nothing changed yet	
}
if(tab=="pkg-descr" || tab=="all"){
   //Load the distinfo for the current port
   QStringList contents = PortUtils::readFile(PORT->portPath()+"/pkg-descr");
   ui->text_pkgdesc->clear();
   ui->text_pkgdesc->setPlainText(contents.join("\n"));
   ui->tool_desc_save->setEnabled(false); //nothing changed yet	
}
	
	
}

// =================
//       MENU SLOTS
// =================
void MainUI::on_actionPreferences_triggered(){
  qDebug() << "triggered";
}

void MainUI::on_actionClose_triggered(){
  qDebug() << "Closing down EasyPort";
  this->close();
}

void MainUI::on_actionPackage_Current_Port_triggered(){
  qDebug() << "triggered";
}

void MainUI::on_actionSubmit_FreeBSD_Port_triggered(){
  qDebug() << "triggered";
}

void MainUI::on_actionPorter_s_Handbook_triggered(){
  QDesktopServices::openUrl( QUrl("http://www.freebsd.org/doc/en_US.ISO8859-1/books/porters-handbook/") );
  qDebug() << "Opening Porter's handbook";
}

void MainUI::on_actionFreshports_triggered(){
  QDesktopServices::openUrl( QUrl("www.freshports.org") );
  qDebug() << "Opening Freshports";
}

void MainUI::on_actionRedports_triggered(){
  QDesktopServices::openUrl( QUrl("www.redports.org") );
  qDebug() << "Opening Redports";
}


// =================
//      TOOLBAR SLOTS
// =================
void MainUI::slotNewPort(){
  qDebug() << "New Port toggled" ;
  NewPortWizard dlg(this);
  dlg.setNewPortBaseDir( PORT->newPortDir() );
  dlg.exec();
  //Now check outputs to see if a port was created
  if(dlg.portCreated){
    PORT->loadPort(dlg.newPortDir);
    portOpened->setText(PORT->portPath().replace(QDir::homePath(), "~"));
    updateGUI();
  }	  
}

void MainUI::slotLoadPort(){
	qDebug() << "Load Port toggled";
  //Prompt the user to select a port
  QString portdir = QFileDialog::getExistingDirectory(this, tr("Select Port"), PORT->portsDir());
  if(portdir.isEmpty() || !QFile::exists(portdir)){ return; }
  if(PORT->loadPort(portdir)){
    qDebug() << "Port Loaded:" << portdir;
    portOpened->setText(PORT->portPath().replace(QDir::homePath(), "~"));
    updateGUI(); //make sure every tab is up to date with the new port
  }else{
    QMessageBox::warning(this, tr("Invalid Port"), QString(tr("Could not open up the following directory: %1")).arg(portdir)+"\n"+tr("Please make sure it is a valid FreeBSD port") );
  }
}

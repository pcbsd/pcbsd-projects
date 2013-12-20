#include "mainUI.h"
#include "ui_mainUI.h"

MainUI::MainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainUI){
  ui->setupUi(this);
  //Setup the line edit on the toolbar
  portOpened = new QLineEdit(this);
  ui->toolBar->addWidget(portOpened);

  //Connect the signals/slots
  connect(ui->actionNewPort, SIGNAL(triggered(bool)), this, SLOT(slotNewPort()) );
}

MainUI::~MainUI(){
	
}

void MainUI::slotSingleInstance(){
  this->show();
  this->raise();
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
}


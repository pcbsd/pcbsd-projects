#include "mainUI.h"
#include "ui_mainUI.h"

mainUI::mainUI() : QMainWindow(), ui(new Ui::mainUI){
  ui->setupUi(this); //load the designer template
  //Setup the removable devices change list
  QStringList rdactions; rdactions << tr("Format Device");
  ui->combo_rd_changelist->addItems(rdactions);
  ui->combo_rd_changelist->setIndex(0);
    //Setup the format removable device filesystem list
    QStringList rdfilesys; rdfilesys << tr("FAT32 (Recommended)") << tr("UFS (PC-BSD/FreeBSD only)");
    ui->combo_rd_format_fs->addItems(rdfilesys);	
	
}

mainUI::~mainUI(){
  delete ui;
}

void mainUI::slotSingleInstance(){
  this->show();
  this->raise();
}

// ==================
//     PRIVATE FUNCTIONS
// ==================
void mainUI::updateRemovableDeviceList(){
	
}

// ==================
//          PRIVATE SLOTS
// ==================
void mainUI::on_actionQuit_triggered(){
  this->close();
}

// ==== REMOVABLE DEVICES TAB ====
void mainUI::on_combo_rd_devices_currentIndexChanged(QString txt){
	
}

void mainUI::on_combo_rd_changelist_currentIndexChanged(int index){
  if(index == 0){
    //Show the disk format page
    ui->stack_rd_changepages->setCurrentWidget(ui->page_reformat);
  }
}

//-------- FORMAT REMOVABLE DEVICE ---------
void mainUI::on_tool_rd_reformatdevice_clicked(){
  //Get the UI inputs
  QString label = ui->line_rd_format_label->text();
  QString filesys;
    if(ui->combo_rd_format_fs->currentIndex() == 0){ filesys = "FAT32"; }
    else if(ui->combo_rd_format_fs->currentIndex() == 1){ filesys = "UFS"; }
  QString device = ui->combo_rd_devices->currentText().section("<",0,0).simplified();
  if(!device.startsWith("/dev/")){ device.prepend("/dev/"); }
  //Check for valid inputs
  if(!QFile::exists(device) || Backend::isMounted(device)){
    QMessageBox::warning(this,tr("Invalid Device"),tr("The device is not longer attached. Please refresh the list of available devices and pick a different one.") );
    return;
  }else if(Backend::isMounted(device)){
    QMessageBox::warning(this,tr("Device Currently Mounted"),tr("The device is currently mounted and/or in use. Please unmount the device first and try again.") );
    return;
  }else if(filesys.isEmpty()){
    QMessageBox::warning(this,tr("Invalid Filesystem"),tr("Please select a filesystem for the device and try again.") );
    return;
  }
  //Verify the format action
  QString summary = QString(tr("Device: %1")).arg(device) +"\n"+QString(tr("New Label: %1")).arg(label)+"\n"+QString(tr("FileSystem: %1")).arg(filesys);
  if( QMessageBox::Yes != QMessageBox::question(this,tr("Verify Format Procedure"),
				tr("Are you sure you wish to format this device? All data currently on this device will be permanently erased.") +"\n"+summary,
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){ return; }
  //Format the device
    //put up a "working" dialog

    //call the backend
	
    //close the "working" dialog
	
  //Display results
	
}

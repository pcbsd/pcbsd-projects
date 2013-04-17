
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pbitools.h"
#include <QDebug>

//PBI_Struct *pbi_data = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Setup Initial state of GUI objects and connect signal/slots
    ui->setupUi(this);
    ui->progressBar->setVisible(FALSE);
    ui->pushKillBuild->setEnabled(FALSE);
    ui->gui_app->setEnabled(FALSE);
    ui->create_button->setEnabled(FALSE);
    ui->pushBuildPBI->setEnabled(FALSE);
    ui->pushFindIcon->setEnabled(FALSE);
    ui->pushSaveLog->setEnabled(FALSE);
    ui->pushOpenPortInfo->setEnabled(FALSE);
    ui->pushPackageModule->setEnabled(FALSE);
    ui->tabModuleEditor->setEnabled(FALSE);
    ui->tabWidget->setCurrentIndex(0); //default to the Modules Tab
    //ui->tabModuleEditor->setCurrentIndex(0); //default to the pbi.conf editor
    connect(ui->pushResetModule,SIGNAL(clicked()),this,SLOT(Reset_Form()) );
    connect(ui->pushClearChanges,SIGNAL(clicked()),this,SLOT(editRefreshPbiConf()) );
    connect(ui->pushSavePbiConf,SIGNAL(clicked()),this,SLOT(editSavePbiConf()) );
    connect(ui->pushRefreshResources,SIGNAL(clicked()),this,SLOT(editRefreshResources()) );
    SetupDefaults();
}

MainWindow::~MainWindow()
{
  
}

void MainWindow::slotSingleInstance(){
  this->showNormal();
  this->activateWindow();
  this->raise();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::SetupDefaults(){
  //pbi_data = NULL;
  PROGVERSION = "1.2";
  PROG_DIR = QDir::homePath() + "/EasyPBI";
  PBIOUT_DIR= PROG_DIR + "/PBI";
  MODOUT_DIR= PROG_DIR + "/Modules";  
  PBISETTINGS_FILE= PROG_DIR + "/.savedPBISettings.conf";
  DEFAULTICON_FILE= PROG_DIR + "/defaulticon.png";
  PORTS_DIR = "";
  PORT_SELECTED = "";

  //Check for EasyPBI directory structure and create it if it is not there
  if( !QDir(PROG_DIR).exists() ){
    qDebug() << "Creating the EasyPBI directory in current user's home folder";
    QDir dir;
    dir.root();
    if( !dir.mkdir(PROG_DIR) ){ //Create the program directory
      //Could not create the Program directory
      QMessageBox::warning(this, tr("Error: Permissions"), tr("Invalid permissions to create EasyPBI directory in home folder"));
      return;
    }
  }  
  if( !QDir(MODOUT_DIR).exists() ){
    QDir dir;
    dir.root();
    if( !dir.mkdir(MODOUT_DIR) ){ //Create the program directory
      //Could not create the Program directory
      QMessageBox::warning(this, tr("Error: Permissions"), tr("Invalid permissions to create EasyPBI directory in home folder"));
      return;
    }
  }  
  if( !QDir(PBIOUT_DIR).exists() ){
    QDir dir;
    dir.root();
    if( !dir.mkdir(PBIOUT_DIR) ){ //Create the program directory
      //Could not create the Program directory
      QMessageBox::warning(this, tr("Error: Permissions"), tr("Invalid permissions to create EasyPBI directory in home folder"));
      return;
    }
  }  
  //Check for the ports Directory and Set the internal variable 'PORTS_DIR'
  if(!Check_For_Ports(true)){ return; }

  //Check for the defaulticons.png file
  if( !QFile(DEFAULTICON_FILE).exists() ){
    //file does not exist, copy the included resource to the file
    qDebug() << "Copying the default icon to the EasyPBI directory";
    QFile::copy(":/png/defaulticon.png",DEFAULTICON_FILE);
    
    QFile::setPermissions(DEFAULTICON_FILE,QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOwner | QFile::WriteOwner | QFile::ReadOther | QFile::WriteOther);
  }

  //Load the defaults for the PBI build settings
  if( QFile(PBISETTINGS_FILE).exists() ){
    qDebug() << "PBI Settings file detected at:"<< PBISETTINGS_FILE;
    loadPBISettings();
  }else{
    ui->linePBIOutDir->setText(PBIOUT_DIR);
  }
  
}

void MainWindow::Reset_Form()
{
    ui->port_name->setText("");
    ui->port_category->setText("");
    ui->author->setText("");
    ui->icon_path->setText("");
    ui->gui_app->setChecked(true);
    ui->website->setText("");
    ui->status_bar->setText(tr("Please push 'New Module' to get started."));
    ui->port_selected->setText("");
    //Reset GUI buttons
    ui->gui_app->setEnabled(FALSE);
    ui->create_button->setEnabled(FALSE);
    ui->pushFindIcon->setEnabled(FALSE);
}

void MainWindow::on_pushFindIcon_clicked()
{
    QString icon_name;
    icon_name = QFileDialog::getOpenFileName(this, tr("Select Icon"), QDir::homePath(), tr("Image Files (*.png)"));
    if( !icon_name.isEmpty() ){
      ui->icon_path->setText(icon_name);
    }
}

void MainWindow::on_actionAbout_EasyPBI_triggered()
{
    QMessageBox::about(this, tr("About EasyPBI"), tr("EasyPBI ")+PROGVERSION+tr("\nCreate PBI modules quickly and easily.\nhttp://makeapbi.sourceforge.net\nWritten by Jesse Smith and Ken Moore"));
}

void MainWindow::on_actionAppCafe_Integration_triggered(){
  QString instructions = tr("Steps to integrate your PBI modules with the official PC-BSD Repository:\n\n  1) Ensure the module works by building and testing the PBI with EasyPBI, making adjustments to the module as needed.\n  2) Check that the module does not already exist in the PC-BSD repository at: http://trac.pcbsd.org/browser/pbi/modules\n  3) E-mail the compressed module file to: pbi-dev@lists.pcbsd.org\nThe module will require re-compression (tar czf 'new-filename.tar.gz' 'module-directory') if modifications were made to the module.\n\nTo place the module into an alternate PBI repository, please contact the maintainer of that repository for instructions.");
  QMessageBox::about(this, tr("EasyPBI: AppCafe Integration"), instructions);
}

void MainWindow::on_pushNewModule_clicked()
{
    // check to see if the ports tree exists
    if (!Check_For_Ports(true)){return;}

    // get new port to work on
    QString module_path = QFileDialog::getExistingDirectory(this, tr("Select Port"), PORTS_DIR);
    if(module_path.isEmpty()){return;} //action cancelled or closed
    PORT_SELECTED = module_path.section("ports/",1,1);

    //get the port information
    QString port_name = ModBuild::getPortInfo(module_path,"portname").join("");
    if(port_name.isEmpty() ){
      //Invalid port directory Selected
      QMessageBox::warning(this, tr("Error: Invalid Port"), tr("Invalid Port. Please select a valid port directory.\nExample: /usr/ports/archivers/unzip is a valid port directory."));
      return;
    }
    QString author_name = ModBuild::getPortInfo(module_path,"maintainer").join("");
    QString website = ModBuild::getPortInfo(module_path,"website").join("");
    QString menu_cat = ModBuild::getPortInfo(module_path,"menucategory").join("");
    
    // Display the detected info
    ui->author->setText(author_name);
    ui->port_category->setText(menu_cat);
    ui->port_selected->setText(PORT_SELECTED);
    ui->icon_path->setText(DEFAULTICON_FILE);

    ui->port_name->setText(port_name);
    ui->website->setText(website);
    ui->status_bar->setText(tr("Please fill out the above fields and click \"Create Module\"."));
    //Enable the GUI Buttons
    ui->gui_app->setEnabled(TRUE);
    ui->create_button->setEnabled(TRUE);
    ui->pushFindIcon->setEnabled(TRUE);
    bool status = TRUE; //Is_GUI(pbi_data);
    if (status)
        ui->gui_app->setChecked(true);
    else
        ui->gui_app->setChecked(false);
}

void MainWindow::on_create_button_clicked()
{
    // Build Module
    bool success = FALSE;
    ModBuild::makeStructs(ui->port_name->text(),ui->website->text(),ui->author->text(), \
                          ui->icon_path->text(),ui->port_selected->text(),"","","","",FALSE,"",FALSE);
    QString newModulePath = ModBuild::makeModuleDir( MODOUT_DIR,ModBuild::getFilenameFromPath(ui->port_selected->text()) );
    if(!newModulePath.isEmpty()){
      qDebug() << "Module Directory:" << newModulePath;
      if( ModBuild::writePBIconf(newModulePath) ){
        ModBuild::addResource(newModulePath,ModBuild::readValue("PBI_PROGICON"));
        QStringList binaries, linkOpts;
        ModBuild::makeExternalLinks(newModulePath,binaries,linkOpts); //should work since pbi.conf was created successfully
        //Add desktop/menu entries 
        QStringList  binList = getPortBinaries(ui->port_selected->text());
        if(ui->gui_app->isChecked() && !binList.isEmpty() ){
          for(int i=0; i<binList.length(); i++){
            QString name;
            if(i==0){ name = ModBuild::readValue("PBI_PROGNAME"); }
            else{ name = ModBuild::readValue("PBI_PROGNAME")+"-"+ModBuild::getFilenameFromPath(binList[i]); }
            ModBuild::makeMenuEntry( newModulePath, binList[i], name, ModBuild::readValue("PBI_PROGICON"), ui->port_category->text() );
            ModBuild::makeDesktopEntry( newModulePath, binList[i], name, ModBuild::readValue("PBI_PROGICON") );
          }
        }
        success = TRUE;
        // create tarball of module
        //ModBuild::compressModule(newModulePath);
      } // pbi.conf success
    } //noModulePath check
    QString result;
    if(success){ 
      result = tr("Done. Your module is located in the EasyPBI folder.\nPlease view the 'AppCafe Integration' page for further instructions"); 
      qDebug() << "Finished building Module";
    }
    else{ 
      result = tr("There was an error creating the module. There may be a permission error creating new directories in your home directory.");
      qDebug() << "Error building module: see log above";
    }
    ui->status_bar->setText(result);
}

void MainWindow::on_port_info_button_clicked()
{
   //Check if a port is selected
   if(ui->port_selected->text().isEmpty()){
     QMessageBox::warning(this, tr("Error: Port Information"), tr("Please select a port first"));
     return;
   }
   //Open up the freshports.org page for the selected port
   QString target_url = "http://freshports.org/" + PORT_SELECTED;
   QDesktopServices::openUrl(QUrl(target_url));
}


int MainWindow::Check_For_Ports(bool show_warning)
{
    QStringList portsLocations;
    //Set the locations to search for the ports tree
    portsLocations << "/usr/ports" << PROG_DIR +"/ports";
    //Search the locations
    for(int i=0; i<portsLocations.size(); i++){
      if( QDir(portsLocations[i]).exists() ){
	PORTS_DIR=portsLocations[i];
	return TRUE;
      }
    }
    //No ports tree detected - Display warning
    if (show_warning)
        QMessageBox::warning(this, tr("Ports Not Installed"),tr("The FreeBSD ports tree is not installed for this system or user.\nPlease Install the ports tree as root to make it available for all users, or push 'File'->'Get Ports' to retrieve the ports tree for this user ONLY"));
    return FALSE;
}

void MainWindow::on_actionGet_Ports_triggered()
{
    //Check to see if the ports are already installed
    if(Check_For_Ports(false)){
      if(PORTS_DIR=="/usr/ports"){
	QMessageBox::warning(this, tr("Ports Installed"),tr("The FreeBSD ports tree is already available on this system\nTo update: run \"portsnap fetch extract\" as root"));
	return;
      }else{
	QMessageBox verify;
	verify.setText(tr("The FreeBSD ports tree is already available for this user"));
	verify.setInformativeText(tr("Do you wish to update the user ports tree? (requires re-download)"));
	verify.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	verify.setDefaultButton(QMessageBox::Yes);
	int ret = verify.exec();
	if(ret != QMessageBox::Yes){return;}
      }
    }
    //Change to the 'Modules' Tab for download status
    ui->tabWidget->setCurrentIndex(0);
    
    //Start the Download
    QNetworkReply *currentDL;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)) );
    currentDL = manager->get(QNetworkRequest(QUrl("ftp://ftp.freebsd.org/pub/FreeBSD/ports/ports/ports.tar.gz")));
    
    ui->status_bar->setText(tr("Downloading the FreeBSD ports tree: Please Wait."));
    
    //Make the Progress Bar visible and start updating
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(TRUE);
    connect(currentDL, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)) );
}

void MainWindow::downloadFinished(QNetworkReply *reply){
  //Disable the progress bar
  ui->progressBar->setVisible(FALSE);
  //See if the Download was successful
  qDebug() << "Ports download completion code:" << reply->error();
  if( reply->error() != QNetworkReply::NoError){
    QMessageBox::warning(this, tr("Download Failed"),tr("Downloading the FreeBSD ports tree failed. Please try again."));
    return;
  }
  //Download Succesful, save it to file
  QFile file(PROG_DIR+"/ports.tar.gz");
  if( !file.open(QIODevice::WriteOnly)){   //if file could not be opened
    QMessageBox::warning(this, tr("Saving Failed"),tr("Saving the FreeBSD ports tree file failed. Please try again."));
    reply->deleteLater();
    return;
  }
  file.write(reply->readAll());
  file.close();
  //Saving the file successful, delete the downloaded data from memory
  reply->deleteLater();
  ui->status_bar->setText(tr("Download finished. Extracting the ports tree into the EasyPBI directory."));
  QTimer::singleShot(10,this, SLOT(extractPT()) );
  return;
}

void MainWindow::extractPT(){
  //See if the directory already exists, and remove it if it does
  if(QDir(PROG_DIR+"/ports").exists()){
    QString tmp = "cd "+PROG_DIR+"; rm -rf ports/";
    system(tmp.toLatin1());
  }
  //Unpack the file
  QString cmd = "cd " + PROG_DIR + "; tar xvf ports.tar.gz";
  system(cmd.toLatin1());
  
  //Delete the file
  QFile file(PROG_DIR+"/ports.tar.gz");  
  file.remove();
  //Let the user know that the ports tree was successfully added
  ui->status_bar->setText(tr("The FreeBSD ports tree has been successfully added to the EasyPBI directory.\nPlease select 'New Module' from the file menu to continue."));
  return;
}

void MainWindow::updateProgress(qint64 bytesReceived, qint64 bytesTotal){
  //Determine the percentage
  int percent = (bytesReceived*100)/bytesTotal;
  //Update the progress bar
  ui->progressBar->setValue(percent);
  ui->progressBar->update();
}

// -------- BUILD PBI'S -----------

void MainWindow::on_pushChangeOutDir_clicked(){
  QString outDir_path; 
  outDir_path = QFileDialog::getExistingDirectory(this,tr("Select PBI Output Directory"),PROG_DIR+"/PBI");
  if(outDir_path.isEmpty()){return;} //action cancelled or closed
  PBIOUT_DIR = outDir_path;
  ui->linePBIOutDir->setText(outDir_path);
}

void MainWindow::on_pushChangeSigFile_clicked(){
  QString outDir_path;
  outDir_path = QFileDialog::getOpenFileName(this, tr("Select Digital Signiture File"), PROG_DIR,tr("Signature Files (*)"));
  if(outDir_path.isEmpty()){return;} //action cancelled or closed
  PBIOUT_DIR = outDir_path;
  ui->linePBIDigSigFile->setText(outDir_path);
}

void MainWindow::on_pushSavePBISettings_clicked(){
  QFile *file = new QFile(PBISETTINGS_FILE);
  //Open the File
  file->open(QIODevice::WriteOnly);
  QTextStream out(file);
  //Save the default lines
  out << "PBI default build settings: ---DELETE FILE TO RESET DEFAULTS, DO NOT MODIFY--\n";
  out << PROGVERSION+"\n";
  out << ui->linePBIOutDir->text() + "\n";
  out << ui->linePBIDigSigFile->text() + "\n";
  if(ui->checkTMPFS->isChecked()){out << 1;}
  else{out << 0;}
  
  file->close();
  //Inform the user that the save is complete
  QMessageBox::information(this, tr("Settings Saved"),tr("PBI build settings successfully saved as the user defaults."));
}

void MainWindow::loadPBISettings(){
  qDebug() << "Loading Previously Saved PBI Settings";
  QFile *file = new QFile(PBISETTINGS_FILE);
  //Open the File
  file->open(QIODevice::ReadOnly);
  QTextStream in(file);
  //Load the lines into the GUI
  in.readLine(); //skip the label line
  QString ver = in.readLine(); //version of the program when it saved the settings
  ui->linePBIOutDir->setText(in.readLine()); //output directory
  ui->linePBIDigSigFile->setText(in.readLine()); //Digital Signiture File
  QString line = in.readLine(); //TMPFS checked
  int chked = line.toInt();
  if(chked==1){ui->checkTMPFS->setChecked(TRUE);}
  else{ui->checkTMPFS->setChecked(FALSE);}

  file->close();
}

void MainWindow::on_pushSelectModule_clicked(){
  QString outDir_path;
  outDir_path = QFileDialog::getExistingDirectory(this, tr("Select Module to Build"), PROG_DIR+"/Modules");
  if(outDir_path.isEmpty()){return;} //action cancelled or closed
  ui->linePBIModuleSelected->setText(outDir_path);
  ui->pushBuildPBI->setEnabled(TRUE);
}

void MainWindow::on_pushBuildPBI_clicked(){

  //Check for the Port tree
  if(!Check_For_Ports(true)){return;}
  
  //Check GUI to make sure settings are set before running
  bool gostatus = TRUE;
  QString outdir = ui->linePBIOutDir->text();
  if (outdir.isEmpty() ){gostatus=FALSE;}
  QString sigfile = ui->linePBIDigSigFile->text(); //this one can be empty
  QString modDir = ui->linePBIModuleSelected->text();
  if(modDir.isEmpty()){gostatus=FALSE;}
  if(!gostatus){
    QMessageBox::warning(this,tr("Error"),tr("Invalid PBI Settings"));
    return;
  }
  
  //Receive User verification before beginning build process due to:
  //  -- long time required, internet connection required, root permissions required
  QMessageBox verify;
  verify.setText(tr("Are you sure you wish to start the PBI build?"));
  verify.setInformativeText(tr("This requires an active internet connection and root permissions. The time required to build a PBI varies depending upon system specifications and ports to be compiled."));
  verify.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  verify.setDefaultButton(QMessageBox::Yes);
  int ret = verify.exec();
  if(ret != QMessageBox::Yes){return;}
  
  //Setup the pbi_makeport command from GUI settings
  QString cmd = "gksu -a \"pbi_makeport";
  cmd += " -c " + modDir;
  if(PORTS_DIR != "/usr/ports"){ cmd += " -d " + PORTS_DIR; }
  cmd += " -o " + outdir;
  cmd += " --delbuild";
  if( ui->checkTMPFS->isChecked() ){ cmd += " --tmpfs"; }
  if(!sigfile.isEmpty()){ cmd += " --sign " + sigfile; }
  cmd += "\"";
  qDebug() << "Build PBI command created:"<<cmd;
  
  //Setup the displays
  ui->pushKillBuild->setEnabled(TRUE);
  ui->pushSaveLog->setEnabled(FALSE);
  ui->pushBuildPBI->setEnabled(FALSE); //disable the button so they do not start more than 1 build at a time
  ui->buildOutputArea->clear(); //clear the display in case this is not the first run
  
  //Setup Process connections
  p = new QProcess(this);
  p->setProcessChannelMode(QProcess::MergedChannels);
  //Remove the LD_LIBRARY_PATH environment variable (use system environment) for this process
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.remove("LD_LIBRARY_PATH");
  //env.insert ("LD_LIBRARY_PATH",""); //alternate method
  p->setProcessEnvironment(env);
  connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(updatePBIBuild()) );
  connect(ui->pushKillBuild,SIGNAL(clicked()),this,SLOT(killPBIBuild()) );
  connect(p,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(PBIbuildFinished(int,QProcess::ExitStatus)) );
  connect(p,SIGNAL(error(QProcess::ProcessError)),this,SLOT(buildError(QProcess::ProcessError)) );
  
  //Start the Process
  qDebug() << "Starting the PBI build process...";
  p->start(cmd);  

}

void MainWindow::updatePBIBuild(){
  QString tmp = p->readAllStandardOutput();
  if( tmp.startsWith("\n") ){tmp.remove(0,0);} //remove newline at the beginning (too much whitespace in log)
  if( tmp.endsWith("\n") ){tmp.chop(1);} //remove newline at the end (already accounted for by appending)
  if(!tmp.isEmpty()){ ui->buildOutputArea->append( tmp ); }
  //qDebug() << "Update output: " << tmp;
}

void MainWindow::PBIbuildFinished(int exitCode,QProcess::ExitStatus exitStatus){
  //Check to see if the PBI process finished successfully
  qDebug() << "PBI build process Finished" << exitStatus << exitCode;
  if(exitCode == 0){
    if(exitStatus==QProcess::NormalExit){
      QMessageBox::information(this,tr("PBI Build Success"),tr("The PBI finished building successfully"));
    }else{
      //The user killed the process - No Message
    }
  }else{
    QMessageBox::warning(this,tr("PBI Build Failure"),tr("The PBI failed to build.\nPlease check the build log to find the cause of the failure and adjust the module accordingly"));
  }
  ui->pushBuildPBI->setEnabled(TRUE);
  ui->pushSaveLog->setEnabled(TRUE);
  ui->pushKillBuild->setEnabled(FALSE);
  p->close();
}

void MainWindow::killPBIBuild(){
  if( p->state()==QProcess::Running ){
    //qDebug() << "Process PID:" << p->pid(); 
    p->terminate();
    ui -> buildOutputArea->append("---PBI Build Terminated---");
  }
}

void MainWindow::buildError(QProcess::ProcessError error){
  qDebug() << "QProcess PBI Build error#" << error;
}

void MainWindow::on_pushSaveLog_clicked(){
  //Have user select a filename/location to save the log
  QString filename = QFileDialog::getSaveFileName(this,tr("Save Log"),PROG_DIR+"/build.log",tr("Log Files (*.log *.txt)"));
  if(filename.isEmpty()){return;} //User cancelled the process
  //Open the file (as new)
  QFile *file = new QFile(filename);
  file->open(QIODevice::WriteOnly);
  //Save the log into the file
  file->write(ui->buildOutputArea->toPlainText().toUtf8());
  file->close();
}
// ----------- EDIT PBI MODULES --------------
void MainWindow::on_tabWidget_currentChanged(int index){
  if(index ==2){ //if moving to the Module Editor Tab
    //make sure there is not a mismatch between the selected module on this tab and the data saved in the module builder backend
    ui->tabModuleEditor->setEnabled(FALSE); 
    ui->pushOpenPortInfo->setEnabled(FALSE);
    ui->pushPackageModule->setEnabled(FALSE);
    ui->label_module_selected->setText(tr("No Module Selected"));
  }
}

void MainWindow::on_tabModuleEditor_currentChanged(int index){
  if(index == 0){ //pbi.conf editor
    editRefreshPbiConf();
  }else if(index == 1){ //Resources Tab
    editRefreshResources();
  }else if(index == 2){ //Desktop/menu entries tab
    editRefreshEntries();
  }else if(index ==3){
    editRefreshLinks();
  }
}

void MainWindow::on_pushSelectModuleEdit_clicked(){
  //Get the desired module
  QString module_path;
  module_path = QFileDialog::getExistingDirectory(this, tr("Select Module to Edit"), PROG_DIR+"/Modules");
  if(module_path.isEmpty()){return;} //action cancelled or closed
  //Update Backend structures
  if( !ModBuild::loadPBIconf(module_path) ){
    //Invalid Module Directory (no pbi.conf inside)
    QMessageBox::warning(this,tr("Invalid PBI Module"),tr("You have selected an invalid PBI module, please make a different choice."));
    return;
  }
  //Enable buttons and GUI display
  ui->label_module_selected->setText(module_path);
  ui->pushOpenPortInfo->setEnabled(TRUE);
  ui->pushPackageModule->setEnabled(TRUE);
  ui->tabModuleEditor->setEnabled(TRUE);
  //Refresh all the tabs
  editRefreshPbiConf();
  editRefreshResources();
  editRefreshEntries();
  editRefreshLinks();
}


void MainWindow::on_pushOpenPortInfo_clicked(){
   //Get the currently selected port within the module
   QString currentmakeport = ModBuild::readValue("PBI_MAKEPORT");
   //Open up the freshports.org page for the selected port
   QString target_url = "http://freshports.org/" + currentmakeport;
   QDesktopServices::openUrl(QUrl(target_url));
}

void MainWindow::on_pushPackageModule_clicked(){
   //Get the current Module Directory
   QString modDir = ui->label_module_selected->text();
   //Call the function to compress the module directory
   ModBuild::compressModule(modDir);
   //Display location of the newly packaged module
   QMessageBox::information(this,tr("Module Packaged"),tr("A copy of your module has been compressed as (module-name).tar.gz within the same directory as your PBI module."));
}

void MainWindow::editRefreshPbiConf(){
  //Display the current PBI info in the backend
  ui->newProgName->setText(ModBuild::readValue("PBI_PROGNAME"));
  ui->newProgWeb->setText(ModBuild::readValue("PBI_PROGWEB"));
  ui->newProgAuth->setText(ModBuild::readValue("PBI_PROGAUTHOR"));
  ui->listNewProgIcon->clear();
  QString progicon = ModBuild::getFilenameFromPath(ModBuild::readValue("PBI_PROGICON"));
  QString progiconpath = ui->label_module_selected->text() +"/resources/"+progicon;
  ui->listNewProgIcon->addItem(QIcon(progiconpath),progicon);
  ui->listNewProgIcon->setCurrentIndex(0);
  //Get all the .png files in the resources directory and add them to the list
  QDir dir(ui->label_module_selected->text()+"/resources");
  QFileInfoList iconlist = dir.entryInfoList(QStringList("*.png"));
  for(int i=0;i<iconlist.length(); i++){
    if(iconlist[i].fileName() != progicon){ui->listNewProgIcon->addItem(QIcon(iconlist[i].absoluteFilePath()),iconlist[i].fileName()); }
  }
  QStringList mkportbefore = ModBuild::readValue("PBI_MKPORTBEFORE").split(" ",QString::SkipEmptyParts);
  ui->listMkPortBefore->clear();
  ui->listMkPortBefore->addItems(mkportbefore);
  QStringList mkportafter = ModBuild::readValue("PBI_MKPORTAFTER").split(" ",QString::SkipEmptyParts);
  ui->listMkPortAfter->clear();
  ui->listMkPortAfter->addItems(mkportafter);
  ui->newMkOpts->setText(ModBuild::readValue("PBI_MAKEOPTS"));
  ui->listMkOpts->clear();
  QStringList mkOpts = ModBuild::getPortInfo(PORTS_DIR+"/"+ModBuild::readValue("PBI_MAKEPORT"), "options");
  for(int i=0; i<mkOpts.length(); i++){
    ui->listMkOpts->addItem(mkOpts[i]+"=true");
  }
  if(ModBuild::readValue("PBI_REQUIRESROOT")=="NO"){ui->checkRequiresRoot->setChecked(FALSE); }
  else{ui->checkRequiresRoot->setChecked(TRUE); }
}

void MainWindow::editSavePbiConf(){
  //Save any changes into the backend structure
  qDebug() << "Saving new pbi.conf information";
  // -- edit boxes --
  ModBuild::writeValue("PBI_PROGNAME",ui->newProgName->text());
  ModBuild::writeValue("PBI_PROGWEB",ui->newProgWeb->text());
  ModBuild::writeValue("PBI_PROGAUTHOR",ui->newProgAuth->text());
  ModBuild::writeValue("PBI_MAKEOPTS",ui->newMkOpts->text());
  // -- list boxes --
  ModBuild::writeValue("PBI_PROGICON",ui->listNewProgIcon->currentText() );

  QString mkportbefore;
  for(int i=0; i<ui->listMkPortBefore->count(); i++){
    mkportbefore.append(" "+ui->listMkPortBefore->itemText(i) );
  }
  ModBuild::writeValue("PBI_MKPORTBEFORE",mkportbefore);

  QString mkportafter;
  for(int i=0; i<ui->listMkPortAfter->count(); i++){
    mkportafter.append(" "+ui->listMkPortAfter->itemText(i) );
  }
  ModBuild::writeValue("PBI_MKPORTAFTER",mkportafter);

  // -- root permissions --
  QString requiresroot = "NO";
  if(ui->checkRequiresRoot->isChecked()){requiresroot="YES";}
  ModBuild::writeValue("PBI_REQUIRESROOT",requiresroot);

  //Write the new pbi.conf with the changes
  QString modDir = ui->label_module_selected->text();
  if( !ModBuild::writePBIconf(modDir) ){
    //Error writing the new pbi.conf
    QMessageBox::warning(this,tr("Write Error"),tr("The pbi.conf file in your module could not be overwritten, check the directory permissions and try again."));
    //Return to the original backend values (re-read pbi.conf)
    ModBuild::loadPBIconf(ui->label_module_selected->text());
    //Do NOT clear the new configuration on the GUI!! (User will lose desired changes)
    return;
  }
  //Update the GUI with the new configuration
  editRefreshPbiConf();
}

void MainWindow::on_pushAddPortBefore_clicked(){
  // check to see if the ports tree exists
  if (!Check_For_Ports(true)){return;}

  // get new port to add
  QString newport = QFileDialog::getExistingDirectory(this, tr("Select Port"), PORTS_DIR);
  if(newport.isEmpty()){return;} //action cancelled or closed

  //Add the new port to the list
  ui->listMkPortBefore->addItem(newport.section("ports/",1,1) );
}

void MainWindow::on_pushRemovePortBefore_clicked(){
  //Remove the selected port from the list
  ui->listMkPortBefore->removeItem(ui->listMkPortBefore->currentIndex());
}

void MainWindow::on_pushAddPortAfter_clicked(){
  // check to see if the ports tree exists
  if (!Check_For_Ports(true)){return;}

  // get new port to add
  QString newport = QFileDialog::getExistingDirectory(this, tr("Select Port"), PORTS_DIR);
  if(newport.isEmpty()){return;} //action cancelled or closed

  //Add the new port to the list
  ui->listMkPortAfter->addItem(newport.section("ports/",1,1) );
}

void MainWindow::on_pushRemovePortAfter_clicked(){
  //Remove the selected port from the list
  ui->listMkPortAfter->removeItem(ui->listMkPortAfter->currentIndex());
}

void MainWindow::on_pushAddMkOpt_clicked(){
  QString newOpt = ui->listMkOpts->currentText();
  if(newOpt.isEmpty()){ return; } //nothing selected
  QStringList currOpts = ui->newMkOpts->text().split(" ");
  if(currOpts.contains(newOpt)){ return; } //do nothing if that option is already enabled
  //Add the new option to the list
  currOpts << newOpt;
  ui->newMkOpts->setText(currOpts.join(" "));
}

void MainWindow::editRefreshResources(){
  //Clear the GUI
  ui->listCurrentResources->clear();
  //Scan the resources directory for all the files inside
  QDir dir(ui->label_module_selected->text()+"/resources");
  QFileInfoList fileList = dir.entryInfoList();
  //Display the files in the GUI
  for(int i=0; i<fileList.length(); i++){
    if( fileList[i].fileName().startsWith(".") ){
      //do nothing for "." and ".." entries (and hidden files/folders)
    }else if( fileList[i].isDir() ){
      ui->listCurrentResources->addItem(fileList[i].fileName()); //Add a directory icon here later
    }else if(fileList[i].fileName().endsWith(".png")){
      QListWidgetItem *item = new QListWidgetItem( QIcon(fileList[i].absoluteFilePath()), fileList[i].fileName() );
      ui->listCurrentResources->addItem(item);
    }else{
      ui->listCurrentResources->addItem(fileList[i].fileName());
    }
  }
}

void MainWindow::on_pushAddResource_clicked(){
  //Prompt for the desired file
  QString resourcepath;
  resourcepath = QFileDialog::getOpenFileName(this, tr("Select Resource File"), QDir::homePath());
  if( resourcepath.isEmpty() ){
    return; //action cancelled
  }
  //Add the file to the module
  if(!ModBuild::addResource(ui->label_module_selected->text(),resourcepath) ){
    //Error adding the file
    QMessageBox::warning(this,tr("Write Error"),tr("The resource could not be copied into your module. Please check the file permissions and try again."));
    return;
  }
  //Update the GUI
  editRefreshResources();
}

void MainWindow::on_pushRemoveResource_clicked(){
  //Get the selected filename
  QString filename = ui->listCurrentResources->currentItem()->text();
  if(filename.isEmpty()){return;} //nothing selected
  //Remove the file from the directory
  QString path = ui->label_module_selected->text() +"/resources/"+filename;
  if(!QFile::remove(path) ){
    QMessageBox::warning(this,tr("Remove Error"),tr("The resource could not be removed from your module. Please check the file permissions and try again."));
    return;
  }
  //Refresh the GUI
  editRefreshResources();
}

QStringList MainWindow::getPortBinaries(QString port){
  //port: category/portname (example: www/firefox)
  QStringList bins;
  bins = ModBuild::getPortInfo(PORTS_DIR+"/"+port, "binaries");
 
  return bins;
}

void MainWindow::editRefreshEntries(){
  //Update the Desktop/menu entries editor
  updateEntryLists();
  //Update the detected binaries
  ui->listEntryPortBins->clear();
  ui->listEntryPortBins->addItem(tr("Custom Binary"));
  ui->listEntryPortBins->setCurrentIndex(0);
  QStringList bins = getPortBinaries(ModBuild::readValue("PBI_MAKEPORT"));
  for(int i=0; i<bins.length(); i++){
    ui->listEntryPortBins->addItem(bins[i]);
  }
  //Clear the manual binary box and set it visible
  ui->lineAlternateBin->clear();
  ui->lineAlternateBin->setVisible(TRUE);
  //Get all the .png files in the resources directory and add them to the list
  QDir dir(ui->label_module_selected->text()+"/resources");
  QFileInfoList iconlist = dir.entryInfoList(QStringList("*.png"));
  ui->listEntryIcons->clear();
  for(int i=0;i<iconlist.length(); i++){
    ui->listEntryIcons->addItem(QIcon(iconlist[i].absoluteFilePath()),iconlist[i].fileName());
  }
  //Add the valid Menu categories to the list
  ui->listMenuCategories->clear();
  QStringList menuCats;
  menuCats <<"AudioVideo"<<"Development"<<"Education"<<"Game"<<"Graphics"<<"Network"<<"Office"<<"Settings"<<"System"<<"Utility";
  ui->listMenuCategories->addItems(menuCats);
  //Clear the Entry label box
  ui->lineEntryName->clear();
}

void MainWindow::updateEntryLists(){
  //Update the two lists that show what desktop/menu entries are in the module
  ui->listDesktopEntries->clear();
  ui->listMenuEntries->clear();
  //Scan the xdg-desktop directory for all the files inside
  QDir dir(ui->label_module_selected->text()+"/xdg-desktop");
  QFileInfoList fileList = dir.entryInfoList(QStringList("*.desktop"));
  for(int i=0; i<fileList.length(); i++){ 
      QString icon = ui->label_module_selected->text()+"/resources/"+ModBuild::getDesktopEntryValue(fileList[i].absoluteFilePath(),"icon",TRUE);
      QListWidgetItem *item = new QListWidgetItem( QIcon(icon),fileList[i].fileName() );
      ui->listDesktopEntries->addItem(item);
  }
  //Scan the xdg-menu directory for all the files inside
  dir.cd(ui->label_module_selected->text()+"/xdg-menu");
  fileList.clear();
  fileList = dir.entryInfoList(QStringList("*.desktop"));
  for(int i=0; i<fileList.length(); i++){
      QString icon = ui->label_module_selected->text()+"/resources/"+ModBuild::getMenuEntryValue(fileList[i].absoluteFilePath(),"icon",TRUE);
      QListWidgetItem *item = new QListWidgetItem( QIcon(icon),fileList[i].fileName() );
      ui->listMenuEntries->addItem(item);
  }
}

void MainWindow::on_listEntryPortBins_currentIndexChanged(int index){
  if(index == 0){ ui->lineAlternateBin->setVisible(TRUE); }
  else{ ui->lineAlternateBin->setVisible(FALSE); }
}

void MainWindow::on_pushRemoveDesktopEntry_clicked(){
  if(ui->listDesktopEntries->currentRow() != -1){ //make sure an item is selected
    QString filename = ui->listDesktopEntries->currentItem()->text();
    //Remove the file from the directory
    QString path = ui->label_module_selected->text() +"/xdg-desktop/"+filename;
    if(!QFile::remove(path) ){
      QMessageBox::warning(this,tr("Remove Error"),tr("The desktop entry could not be removed from your module. Please check the file permissions and try again."));
      return;
    }
    //Refresh the GUI
    updateEntryLists();
  }
}

void MainWindow::on_pushRemoveMenuEntry_clicked(){
  if(ui->listMenuEntries->currentRow() != -1){ //make sure an item is selected
    QString filename = ui->listMenuEntries->currentItem()->text();
    //Remove the file from the directory
    QString path = ui->label_module_selected->text() +"/xdg-menu/"+filename;
    if(!QFile::remove(path) ){
      QMessageBox::warning(this,tr("Remove Error"),tr("The menu entry could not be removed from your module. Please check the file permissions and try again."));
      return;
    }
    //Refresh the GUI
    updateEntryLists();
  }
}

void MainWindow::on_pushAddDesktopEntry_clicked(){
  //Get the user inputs
  QString modDir = ui->label_module_selected->text();
  QString bin;
  if(ui->listEntryPortBins->currentIndex()==0){ bin = ui->lineAlternateBin->text(); }
  else{ bin = ui->listEntryPortBins->currentText(); }
  if(bin.isEmpty()){
    QMessageBox::warning(this,tr("No Binary"),tr("There is no executable selected, please choose one and try again."));
    return;
  }
  QString name = ui->lineEntryName->text();
  if(name.isEmpty()){
    QMessageBox::warning(this,tr("No Label"),tr("There is no label for the entry, please choose one and try again."));
    return;
  }
  QString icon = ui->listEntryIcons->currentText();
  if(icon.isEmpty()){
    QMessageBox::warning(this,tr("No Icon"),tr("There is no icon for the entry, please add a .png file to the module resources to make it available."));
    return;
  }

  //Create the desktop entry
  if( !ModBuild::makeDesktopEntry(modDir,bin,name,icon) ){
    QMessageBox::warning(this,tr("Add Error"),tr("The desktop entry could not be created. Please check the directory permissions and try again."));
    return;
  }
  //Refresh the GUI
  updateEntryLists();
}

void MainWindow::on_pushAddMenuEntry_clicked(){
  //Get the user inputs
  QString modDir = ui->label_module_selected->text();
  QString bin;
  if(ui->listEntryPortBins->currentIndex()==0){ bin = ui->lineAlternateBin->text(); }
  else{ bin = ui->listEntryPortBins->currentText(); }
  if(bin.isEmpty()){
    QMessageBox::warning(this,tr("No Binary"),tr("There is no executable selected, please choose one and try again."));
    return;
  }
  QString name = ui->lineEntryName->text();
  if(name.isEmpty()){
    QMessageBox::warning(this,tr("No Label"),tr("There is no label for the entry, please choose one and try again."));
    return;
  }
  QString icon = ui->listEntryIcons->currentText();
  if(icon.isEmpty()){
    QMessageBox::warning(this,tr("No Icon"),tr("There is no icon for the entry, please add a .png file to the module resources to make it available."));
    return;
  }
  QString category = ui->listMenuCategories->currentText(); //will always be something here

  //Create the menu entry
  if( !ModBuild::makeMenuEntry(modDir,bin,name,icon,category) ){
    QMessageBox::warning(this,tr("Add Error"),tr("The menu entry could not be created. Please check the directory permissions and try again."));
    return;
  }
  //Refresh the GUI
  updateEntryLists();
}

void MainWindow::editRefreshLinks(){
  //Update the current links in the file
  refreshCurrentLinks(TRUE);
  //Get the binaries available and populate list
  ui->listLinkBins->clear();
  ui->listLinkBins->addItem(tr("Custom File"));
  ui->listLinkBins->setCurrentIndex(0);
  QStringList bins = getPortBinaries(ModBuild::readValue("PBI_MAKEPORT"));
  for(int i=0; i<bins.length(); i++){
    ui->listLinkBins->addItem(bins[i]);
  }
  //Clear the manual binary box and set it visible
  ui->lineLinkBin->clear();
  ui->lineLinkBin->setVisible(TRUE);
  //Clear the current Link types
  ui->lineLinkType->clear();
  //Display the available link types
  QStringList types;
  types <<"binary"<<"linux"<<"keep"<<"replace"<<"nocrash";
  ui->listLinkTypesAvailable->clear();
  ui->listLinkTypesAvailable->addItems(types);
}

void MainWindow::refreshCurrentLinks(bool forceRead){
  //Read external-links file
  QString filePath = ui->label_module_selected->text() + "/external-links";
  QStringList bins = ModBuild::getExternalLinksValues(filePath,"Target",forceRead);
  QStringList types = ModBuild::getExternalLinksValues(filePath,"Action");
  //display info in GUI
  ui->listCurrentLinks->clear();
  //Make sure each line is displayed in the format [(bin) : (types)]
  for(int i=0; i<bins.length(); i++){
    //qDebug() << bins << types;
    if(!bins[i].isEmpty() && !types[i].isEmpty()){
      QString entry = bins[i] + "  :  " + types[i];
      ui->listCurrentLinks->addItem(entry);
    }
  }

}

void MainWindow::on_pushRefreshLinkList_clicked(){
  refreshCurrentLinks(TRUE);
}

void MainWindow::on_pushClearLinkChanges_clicked(){
  editRefreshLinks();
}

void MainWindow::on_pushAddLink_clicked(){
  //Get the user inputs
  QString bin;
  if(ui->listLinkBins->currentIndex() == 0){bin = ui->lineLinkBin->text(); }
  else{ bin = ui->listLinkBins->currentText(); }
  if(bin.isEmpty()){
    QMessageBox::warning(this,tr("No File"),tr("There is no file selected, please choose one and try again."));
    return;
  }
  QString type = ui->lineLinkType->text();
  if(type.isEmpty()){
    QMessageBox::warning(this,tr("No Link Type"),tr("There is no action selected, please add one and try again."));
    return;
  }

  //Read the current links from the list
  QStringList binlist, typelist;
  for(int i=0; i < ui->listCurrentLinks->count(); i++){
    QString line = ui->listCurrentLinks->item(i)->text();
    binlist << line.section(":",0,0,QString::SectionSkipEmpty).simplified();
    typelist << line.section(":",1,1,QString::SectionSkipEmpty).simplified();
  }
  //Add the desired link to the file
  binlist << bin;
  typelist << type;
  if(!ModBuild::makeExternalLinks(ui->label_module_selected->text(),binlist,typelist)){
    QMessageBox::warning(this,tr("Write Error"),tr("Error writing the external-links file. Please check the permissions on your module and try again."));
    return;
  }
  //Refresh the GUI
  editRefreshLinks();
}

void MainWindow::on_pushAddLinkType_clicked(){
  //get the selected link type
  QString type = ui->listLinkTypesAvailable->currentText();
  QString currentTypes = ui->lineLinkType->text();
  //make sure that type is not already added
  if(currentTypes.split(",").contains(type)){
    QMessageBox::warning(this,tr("Duplicate Link Type"),tr("This link type has already been added. Please select a different one."));
    return;
  }
  //Change/Add the type to the current list
  if(currentTypes.split(",").contains("keep") && type=="replace"){
    currentTypes.replace("keep",type);
  }else if(currentTypes.split(",").contains("replace") && type=="keep"){
    currentTypes.replace("replace",type);
  }else{
    //add it to the end
    if(currentTypes.isEmpty()){ currentTypes = type; }
    else{ currentTypes.append(","+type); }

  }
  //Display the types (with changes)
  ui->lineLinkType->setText(currentTypes);
}

void MainWindow::on_pushRemoveLink_clicked(){
  //Get the selected line
  int index = ui->listCurrentLinks->currentRow();
  if(index == -1){ return; } //nothing selected - do nothing
  //Read the current links from the list, skipping the line to remove
  QStringList binlist, typelist;
  for(int i=0; i<ui->listCurrentLinks->count(); i++){
    if(i != index){
      QString line = ui->listCurrentLinks->item(i)->text();
      binlist << line.section(":",0,0,QString::SectionSkipEmpty).simplified();
      typelist << line.section(":",1,1,QString::SectionSkipEmpty).simplified();
    }
  }
  //re-write the external-links file
  if(!ModBuild::makeExternalLinks(ui->label_module_selected->text(),binlist,typelist)){
    QMessageBox::warning(this,tr("Write Error"),tr("Error writing the external-links file. Please check the permissions on your module and try again."));
    return;
  }

  //Refresh the GUI
  refreshCurrentLinks(TRUE);
}

void MainWindow::on_listLinkBins_currentIndexChanged(int index){
  if(index == 0){ ui->lineLinkBin->setVisible(TRUE); }
  else{ ui->lineLinkBin->setVisible(FALSE); }
}


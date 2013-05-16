#include "configDialog.h"
#include "ui_configDialog.h" //Qt-designer file

//Public input/output variables
/*  bool applyChanges;
    QStringList xdgOpts;
    bool keepDownloads;
    QString downloadDir;
    PBIDBAccess *DB; // Input only - current database access class
*/
ConfigDialog::ConfigDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ConfigDialog){
  ui->setupUi(this); //load the Qt-Designer file
  applyChanges = FALSE;
}

ConfigDialog::~ConfigDialog(){
  delete ui;	
}

void ConfigDialog::setupDone(){
  applyChanges = FALSE; //make sure no changes by default
  //Now load the info onto the GUI
  ui->check_desktop->setChecked( xdgOpts.contains("desktop") );
  ui->check_menu->setChecked( xdgOpts.contains("menu") );
  ui->check_mime->setChecked( xdgOpts.contains("mime") );
  ui->check_paths->setChecked( xdgOpts.contains("paths") );
  ui->group_download->setChecked( keepDownloads );
  ui->line_downloadDir->setText( downloadDir.replace(QDir::homePath(),"~") );
  //Get the repo information
  repoID = DB->currentRepo();
  QStringList repoList = DB->availableRepos();
  int index = repoList.indexOf(repoID);
  for(int i=0; i<repoList.length(); i++){
    QStringList info = DB->repoInfo(repoList[i]);
    repoList[i].append(" - "+info[0] );	
  }
  //Now fill the repo tab
  ui->combo_repo->clear();
  ui->combo_repo->addItems( repoList );
  if(index != -1){
    ui->combo_repo->setCurrentIndex(index); //will call the slot automatically
  }else{
    ui->combo_repo->setCurrentIndex(0);	  
  }
}

// === ButtonBox ===
void ConfigDialog::on_buttonBox_accepted(){
  applyChanges = TRUE; //flag that changes are available
  //generate the xdg install Options
  xdgOpts.clear();
  if(ui->check_desktop->isChecked()){ xdgOpts << "desktop"; }
  if(ui->check_menu->isChecked()){ xdgOpts << "menu"; }
  if(ui->check_mime->isChecked()){ xdgOpts << "mime"; }
  if(ui->check_paths->isChecked()){ xdgOpts << "paths"; }
  //Download Directory settings
  keepDownloads = ui->group_download->isChecked();
  downloadDir = ui->line_downloadDir->text();
  downloadDir.replace("~",QDir::homePath());
  //Repo 
  repoID = ui->combo_repo->currentText().section(" - ",0,0).simplified();
  DB->setRepo(repoID);
  //Now close the UI
  this->close();
}

void ConfigDialog::on_buttonBox_rejected(){
  applyChanges = FALSE;
  DB->setRepo(repoID); //just in case it was changed by the UI
  this->close();
}

// === Repo Tab ===
void ConfigDialog::on_combo_repo_currentIndexChanged(QString){
	
}

void ConfigDialog::on_tool_repo_add_clicked(){
  QString rpofile = QFileDialog::getOpenFileName(this,tr("Add PBI Repository"), QDir::homePath(), tr("Repository File (*.rpo)") );
  if(rpofile.isEmpty()){ return; } //cancelled
  qDebug() << "Now need to do something with the new *.rpo file";
}

void ConfigDialog::on_tool_repo_remove_clicked(){
  //Verify the removal
  //remove the repo
}

void ConfigDialog::on_tool_repomirror_add_clicked(){
	
}

void ConfigDialog::on_tool_repomirror_remove_clicked(){
	
}

void ConfigDialog::on_tool_repomirror_up_clicked(){
	
}

void ConfigDialog::on_tool_repomirror_down_clicked(){
	
}
	
// === Config Tab ===
void ConfigDialog::on_group_download_toggled(bool checked){
  ui->frame_dldir->setVisible(checked);
}

void ConfigDialog::on_tool_getDownloadDir_clicked(){
  QString dirpath = QFileDialog::getExistingDirectory(this, tr("Select Download Directory"), QDir::homePath());
  if(dirpath.isEmpty()){return;} //not cancelled
  dirpath.replace(QDir::homePath(),"~");
  ui->line_downloadDir->setText(dirpath);
}


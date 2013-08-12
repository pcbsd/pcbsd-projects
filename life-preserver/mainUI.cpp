#include "mainUI.h"
#include "ui_mainUI.h"

mainUI::mainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainUI){
  //Initialize the graphical items
  ui->setupUi(this);  //load the mainUI.ui file
  revMenu = new QMenu();
  brMenu = new QMenu();
  addMenu = new QMenu();
  //Setup the menu system
  ui->tool_revert->setMenu(revMenu);
  ui->tool_browse->setMenu(brMenu);
  ui->tool_add->setMenu(addMenu);
  connect(revMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotRevertToSnapshot(QAction*)) );
  connect(brMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotBrowseSnapshot(QAction*)) );
  connect(addMenu, SIGNAL(triggered(QAction*)),this,SLOT(slotAddDataset(QAction*)) );
}

mainUI::~mainUI(){
	
}

void mainUI::setupUI(){
  qDebug() << "Setting up Life Preserver UI...";
  //Initialize the Hash
  updateHash();
  //Update the display
  updateUI();
  updateMenus();
}

// =================
//    PRIVATE FUNCTIONS
// =================
void mainUI::updateHash(QString ds){
  if(HLIST.contains(ds) && !ds.isEmpty()){
    //only update the entry for the given dataset
    QStringList snaps = LPBackend::listSnapshots(ds);
    snaps.sort();
    HLIST.insert(ds, snaps); //will overwrite the current entry in the hash
  }else{
    //Clear and fill the hash
    HLIST.clear();
    QStringList dsList = LPBackend::listDatasets();
    for(int i=0; i<dsList.length(); i++){
      QStringList snaps = LPBackend::listSnapshots(dsList[i]);
      snaps.sort();
      HLIST.insert(dsList[i], snaps);
    }
  }
  RLIST = LPBackend::listReplicationTargets();
  SLIST = LPBackend::listPossibleDatasets();
}

void mainUI::updateUI(){
  ui->treeWidget->clear();
  QStringList dsList = HLIST.keys();
  for(int i=0; i<dsList.length(); i++){
    //Create the item for the treeWidget
    // [ dataset, latest snapshot, num snapshots, is Replicated ]
    QStringList cols;
	  cols << dsList[i]; //[0] - dataset
	  int num = HLIST[dsList[i]].length();
	  if(num > 0){ 
	    cols << HLIST[dsList[i]].value(0); // [1] - newest snapshot name
	    cols << QString::number(num); // [2] - total number of snapshots
	  }else{
	    cols << tr("NONE"); // [1] - newest snapshot name
	    cols << "0"; // [2] - total number of snapshots
	  }
	  //Check for replication
	  if(RLIST.contains(dsList[i])){ cols << tr("Yes"); } // [3] - is replicated
	  else{ cols << tr("No"); } // [3] - is replicated
    //Add the item to the widget
    ui->treeWidget->addTopLevelItem( new QTreeWidgetItem(cols) );
  }
  //Now adjust the columns in the widget
  for(int i=0; i<4; i++){
    ui->treeWidget->resizeColumnToContents(i);
  }
  //Now make sure that the add button menu only shows the available datasets
  addMenu->clear();
  for(int i=0; i<SLIST.length(); i++){
    if(!HLIST.contains(SLIST[i])){ addMenu->addAction( new QAction(SLIST[i],this) ); }
  }
  if(addMenu->isEmpty()){ ui->tool_add->setEnabled(false); }
  else{ ui->tool_add->setEnabled(true); }
}

void mainUI::updateMenus(){
  //Reset the button menu's to correspond to the selected dataset
  QString ds = getSelectedDS();
  //Enable/disable the remove/config buttons if nothing selected
  if(ds.isEmpty()){
    ui->tool_remove->setVisible(false);
    ui->tool_config->setVisible(false);
  }else{
    ui->tool_remove->setVisible(true);
    ui->tool_config->setVisible(true);	  
  }
  //check for a valid ds/snapshot combination
  bool ok = !ds.isEmpty();
  if(ok){ ok = HLIST.contains(ds); }
  if(ok){ ok = (HLIST[ds].length() > 0); }
  //Now set the items appropriately
  revMenu->clear();
  brMenu->clear();

  if(ok){
    //Reset the Menu Contents
    QStringList snaps = HLIST[ds];	
    for(int i=0; i<snaps.length(); i++){
      revMenu->addAction( new QAction(snaps[i], this) );
      brMenu->addAction( new QAction(snaps[i], this) );
    }	    
    //Enable the buttons if appropriate
    if(revMenu->isEmpty()){
      ui->tool_revert->setEnabled(false);
      ui->tool_browse->setEnabled(false);
    }else{
      ui->tool_revert->setEnabled(true);
      ui->tool_browse->setEnabled(true);
    }
  }else{
    //Disable the buttons
    ui->tool_revert->setEnabled(false);
    ui->tool_browse->setEnabled(false);
  }
}

QString mainUI::getSelectedDS(){
  //Get the currently selected dataset
  QString ds="";
  if(ui->treeWidget->topLevelItemCount() > 0){
    if( ui->treeWidget->currentItem() != 0){
      ds = ui->treeWidget->currentItem()->text(0); //first column
    }
  }
  return ds;
}

// =================
//        PRIVATE SLOTS
// =================
//  --- Buttons Clicked
void mainUI::on_treeWidget_itemSelectionChanged(){
  updateMenus();
}

void mainUI::on_tool_config_clicked(){
	
}

void mainUI::on_tool_remove_clicked(){
  QString ds = getSelectedDS();
  if(!ds.isEmpty()){
    //Verify the removal of the dataset
    if( QMessageBox::Yes == QMessageBox::question(this,tr("Verify Dataset Backup Removal"),tr("Are you sure that you wish to cancel automated snapshots and/or replication of the following dataset?")+"\n\n"+ds,QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){	    
      //verify the removal of all the snapshots for this dataset
      if( QMessageBox::Yes == QMessageBox::question(this,tr("Verify Snapshot Deletion"),tr("Do you wish to remove the local snapshots for this dataset?")+"\n"+tr("WARNING: This is a permanant change that cannot be reversed"),QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){
	//Remove all the snapshots
	QStringList snaps;
	if(HLIST.contains(ds)){ snaps = HLIST[ds]; }
	else{ snaps = LPBackend::listSnapshots(ds); }
	for(int i=0; i<snaps.length(); i++){
	  LPBackend::removeSnapshot(ds,snaps[i]);
	}
      }
      //Remove the dataset from life-preserver management
      if(RLIST.contains(ds)){ LPBackend::removeReplication(ds); }
      LPBackend::removeDataset(ds);
    }
  }
  updateHash();
  updateUI();
  updateMenus();
}


// --- Menu Items Clicked
void mainUI::slotRevertToSnapshot(QAction *act){
  QString snapshot = act->text();
  QString ds = getSelectedDS();
  if(!ds.isEmpty()){
    //Verify the reversion 
     if( QMessageBox::Yes == QMessageBox::question(this,tr("Verify Snapshot Reversion"),
	     QString(tr("Are you sure that you wish to revert %1 to the following snapshot?")).arg(ds)+"\n"+tr("WARNING: This will result in the loss of any data not previously backed up.")+"\n\n"+ds,
	     QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){
        //Perform the reversion
        if( !LPBackend::revertSnapshot(ds,snapshot) ){
	  //Error performing the reversion
	  QMessageBox::warning(this,tr("Reversion Error"), tr("The snapshot reversion could not be completed successfully."));
	}else{
	  //Good reversion
	  QMessageBox::information(this,tr("Reversion Success"), tr("The snapshot reversion was completed successfully."));	
	}
     }
  }
}

void mainUI::slotBrowseSnapshot(QAction *act){
  QString snapshot = act->text();	
}

void mainUI::slotAddDataset(QAction *act){
  QString dataset = act->text();
  qDebug() << "Start Wizard for new dataset:" << dataset;
  LPWizard wiz(this);
  wiz.setDataset(dataset);
  wiz.exec();
  //See if the wizard was cancelled or not
  if(!wiz.cancelled){
    //run the proper commands to get the dataset enabled
    if( LPBackend::setupDataset(dataset, wiz.localTime, wiz.totalSnapshots) ){
      if(wiz.enableReplication){
	 LPBackend::setupReplication(dataset, wiz.remoteHost, wiz.remoteUser, wiz.remotePort, wiz.remoteDataset, wiz.remoteTime);     
      }
    }
  }
  //Now update the UI/Hash
  updateHash();
  updateUI();
  updateMenus();
}

void mainUI::on_actionClose_triggered(){
  this->close();
}

// =============
//      PROTECTED
// =============
void mainUI::closeEvent(QCloseEvent *event){
  //Make sure this window only gets hidden rather than closed
  // this prevents the entire tray application from closing down as well
  event->ignore();
  this->hide();
}

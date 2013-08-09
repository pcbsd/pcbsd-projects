#include "mainUI.h"
#include "ui_mainUI.h"

mainUI::mainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainUI){
  //Initialize the graphical items
  ui->setupUi(this);  //load the mainUI.ui file
  revMenu = new QMenu();
  brMenu = new QMenu();
  //Setup the menu system
  ui->tool_revert->setMenu(revMenu);
  ui->tool_browse->setMenu(brMenu);
  connect(revMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotRevertToSnapshot(QAction*)) );
  connect(brMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotBrowseSnapshot(QAction*)) );

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
}

void mainUI::updateMenus(){
  //Reset the button menu's to correspond to the selected dataset
  QString ds = getSelectedDS();
  //check for a valid ds
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
    //Enable the buttons
    ui->tool_revert->setEnabled(true);
    ui->tool_browse->setEnabled(true);
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
    ds = ui->treeWidget->currentItem()->text(0); //first column
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

void mainUI::on_tool_add_clicked(){

}

void mainUI::on_tool_browse_clicked(){
	
}

void mainUI::on_tool_config_clicked(){
	
}

void mainUI::on_tool_copy_clicked(){
	
}

void mainUI::on_tool_remove_clicked(){
	
}

void mainUI::on_tool_revert_clicked(){
	
}

// --- Menu Items Clicked
void mainUI::slotRevertToSnapshot(QAction *act){
  QString snapshot = act->text();
}

void mainUI::slotBrowseSnapshot(QAction *act){
  QString snapshot = act->text();	
}
// =============
//      PROTECTED
// =============
void mainUI::closeEvent(QCloseEvent *event){
  event->ignore();
  this->hide();
}

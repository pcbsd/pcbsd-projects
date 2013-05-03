/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
#include "mainUI.h"
#include "ui_mainUI.h"

MainUI::MainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainUI){
  //initialization function
  ui->setupUi(this); //load the Qt-Designer file
  defaultIcon = ":/application.png";
}

void MainUI::setWardenMode(QString dir, QString ip){
  wardenDir = dir; wardenIP = ip;
}

void MainUI::ProgramInit()
{ 
   qDebug("Application starting...");
   //Initial Checks
     bool wardenMode = !wardenDir.isEmpty();
     bool ok = Extras::checkUser(wardenMode);
     if(!ok && wardenMode){  //Not root
       QMessageBox::information( this, tr("Error!"), tr("The AppCafe must be run as root when in Warden compatibility mode") );
       close();	     
     }else if( !ok && !wardenMode){
       QMessageBox::information( this, tr("Error!"), tr("The AppCafe must be started with user permissions!")+"\n"+tr("The user must also be a part of the \"operator\" group") );
       close();
     }
     //Make sure the user is in the proper group

   //Load the application settings
   
   //Initialize the backend worker class
   //qDebug() << "Initialize Backend";
   PBI = new PBIBackend();
     if(wardenMode){ PBI->setWardenMode(wardenDir, wardenIP); }
     //PBI->setDownloadDir();
     
   //Initialize the Installed tab
   //qDebug() << "Initialize Installed Tab";
   initializeInstalledTab();
   //Initialize the PBI Browser
   //qDebug() << "Initialize Browser Tab";
   initializeBrowserTab();
   
   //Now startup the backend
   //qDebug() << "Startup Backend";
     connect(PBI,SIGNAL(LocalPBIChanges()),this,SLOT(slotRefreshInstallTab()) );
     connect(PBI,SIGNAL(PBIStatusChange(QString)),this,SLOT(slotRefreshInstallTab()) );
     connect(PBI,SIGNAL(RepositoryInfoReady()),this,SLOT(slotEnableBrowser()) );
     connect(PBI,SIGNAL(SearchComplete(QStringList,QStringList)),this,SLOT(slotShowSearchResults(QStringList, QStringList)) );
     PBI->start();

   //Make sure we start on the installed tab
   ui->tabWidget->setCurrentWidget(ui->tab_installed);
}

void MainUI::slotSingleInstance(){
  this->raise();
  this->showNormal();
  this->activateWindow();
}

// =========================
// ===== INSTALLED TAB =====
// =========================
void MainUI::initializeInstalledTab(){
  //Setup the action menu for installed applications
  actionMenu = new QMenu();
    actionMenu->addAction( QIcon(":icons/appcafe.png"), tr("Install Desktop Icons"), this, SLOT(slotActionAddDesktop()) );
    actionMenu->addAction( QIcon(":icons/appcafe.png"), tr("Install Menu Icons"), this, SLOT(slotActionAddMenu()) );
    actionMenu->addAction( QIcon(":icons/appcafe.png"), tr("Install PATH Links"), this, SLOT(slotActionAddPath()) );
    actionMenu->addAction( QIcon(":icons/upgrade.png"), tr("Update Application"), this, SLOT(slotActionUpdate()) );
    actionMenu->addSeparator();
    actionMenu->addAction( QIcon(":icons/appcafe.png"), tr("Install Menu Icons (All Users)"), this, SLOT(slotActionAddMenuAll()) );
    actionMenu->addAction( QIcon(":icons/appcafe.png"), tr("Install PATH Links (All Users)"), this, SLOT(slotActionAddPathAll()) );
    actionMenu->addSeparator();
    actionMenu->addAction( QIcon(":icons/remove.png"), tr("Remove Applications"), this, SLOT(slotActionRemove()) );
    ui->tool_install_performaction->setMenu(actionMenu);
    ui->tool_install_performaction->setPopupMode(QToolButton::InstantPopup);
    ui->tool_install_gotobrowserpage->setEnabled(FALSE); //disable it until the browser is ready
    slotRefreshInstallTab();
}

void MainUI::formatInstalledItemDisplay(QTreeWidgetItem *item){
  //simplification function for filling the tree widget item with the appropriate information about the PBI
  QString ID = item->whatsThis(0);
  if(item->text(0).isEmpty()){  //new entry - get everything
    QStringList vals; vals <<"name"<<"version"<<"status"; //same as column order
    if(!vals.isEmpty()){
      vals = PBI->PBIInfo(ID, vals);
      for(int i=0; i<vals.length(); i++){ item->setText(i,vals[i]); }
      QString icon = PBI->PBIInfo(ID, QStringList("icon")).join("");
        //Load a default icon if none found
      if(icon.isEmpty()){ icon = defaultIcon; }
      item->setIcon(0,QIcon(icon) );
      item->setCheckState(0,Qt::Unchecked);
    }
  }else{ // Just update the necesary info
    QStringList vals; vals << "version" << "status";
    vals = PBI->PBIInfo(ID,vals);
    if(!vals.isEmpty()){ item->setText(1,vals[0]); item->setText(2,vals[1]); }
  }
}

// === SLOTS ===
void MainUI::slotRefreshInstallTab(){
  //Update the list of installed PBI's w/o clearing the list (loses selections)
  QStringList installList = PBI->installedList();
  //Update the currently shown list
  for(int i=0; i<ui->tree_install_apps->topLevelItemCount(); i++){
    QString itemID = ui->tree_install_apps->topLevelItem(i)->whatsThis(0);
    int index = installList.indexOf(itemID);
    if( index != -1 ){ //item is still installed
      //Update the widget item
      formatInstalledItemDisplay( ui->tree_install_apps->topLevelItem(i) );
      //Now remove the item from the installed list - already done with it
      installList.removeAt(index);
    }else{
      //Remove the item from the UI list - no longer installed
      ui->tree_install_apps->takeTopLevelItem(i);
      i--; //Make sure we don't miss an item by changing the count
    }
  }
  //Now Create new widget items as necessary (anything left on the installed list)
  for(int i=0; i<installList.length();i++){
    QTreeWidgetItem *item = new QTreeWidgetItem; //create the item
      item->setWhatsThis(0,installList[i]);
    //Now format the display
    formatInstalledItemDisplay(item);
    //Now add this item to the list
    ui->tree_install_apps->addTopLevelItem(item);
  }
  //Now re-sort the items
  if(ui->tree_install_apps->topLevelItemCount() > 0 ){
    ui->tree_install_apps->sortItems(0,Qt::AscendingOrder);
    //Make sure that there is an item selected
    if( ui->tree_install_apps->selectedItems().isEmpty() ){
      ui->tree_install_apps->setCurrentItem( ui->tree_install_apps->topLevelItem(0) );
    }
  }
  on_tree_install_apps_itemSelectionChanged(); //Update the info boxes
}

void MainUI::on_group_install_showinfo_toggled(bool show){
  if(ui->tree_install_apps->topLevelItemCount() > 0){
    ui->group_install_info->setVisible(show);
  }else{
    ui->group_install_info->setVisible(FALSE);	 
  }
}

void MainUI::on_tool_install_gotobrowserpage_clicked(){
  //When you want to open up the browser page for an application
  QString appID = ui->tree_install_apps->currentItem()->text(0).toLower();
  slotGoToApp(appID);
}

void MainUI::on_tool_install_updateall_clicked(){
  //Update all installed PBI's (if possible)
  qDebug() << "Update all PBI's not implemented yet";
}

void MainUI::on_tree_install_apps_itemSelectionChanged(){
  //When an installed PBI is clicked on
  QString appID;
  if(ui->tree_install_apps->topLevelItemCount() > 0){
    appID = ui->tree_install_apps->currentItem()->whatsThis(0);
  }
  if(appID.isEmpty()){
    ui->group_install_info->setVisible(FALSE);
    return;
  }else{
    ui->group_install_info->setVisible( ui->group_install_showinfo->isChecked() );	  
  }
  //Get the PBI info for that item
  QStringList vals; 
  vals << "name" << "icon" << "author" << "website" << "version" << "license";
  QStringList bools;
  bools << "autoupdate" << "hasdesktopicons" << "hasmenuicons";
  vals = PBI->PBIInfo(appID,vals);
  bools = PBI->PBIInfo(appID,bools);
  //Make sure the info lists are not empty
  if(vals.isEmpty() || bools.isEmpty()){
    ui->group_install_info->setVisible(FALSE);
    return; 
  }
  //Load a default icon if none found
  if(vals[1].isEmpty()){ vals[1] = defaultIcon; }
  //Now set the info on the UI
  bool desktopSC = (bools[1] == "true"); // XDG desktop entries available
  bool menuSC= (bools[2] == "true"); 	 // XDG menu entries available
  bool autoupdate = (bools[0] == "true");  
  //Create the shortcuts string
  QString shortcuts;
    if(desktopSC && menuSC){ shortcuts = tr("Desktop/Menu"); }
    else if(desktopSC){ shortcuts = tr("Desktop"); }
    else if(menuSC){ shortcuts = tr("Menu"); }
    else{ shortcuts = tr("None"); }
  //Now display that info on the UI
  ui->label_install_app->setText(vals[0]);
  ui->label_install_icon->setPixmap( QPixmap(vals[1]) );
  if(vals[3].isEmpty()){ ui->label_install_author->setText(vals[2]); }
  else{ ui->label_install_author->setText("<a href="+vals[3]+">"+vals[2]+"</a>"); }
  ui->label_install_license->setText(vals[5]);
  ui->label_install_version->setText(vals[4]);
  ui->label_install_shortcuts->setText(shortcuts);
  ui->check_install_autoupdate->setChecked(autoupdate);
}

// === SELECTED PBI ACTIONS ===
void MainUI::slotActionAddDesktop(){
  qDebug() << "Actions not implemented yet";
}

void MainUI::slotActionAddPath(){
  qDebug() << "Actions not implemented yet";	
}

void MainUI::slotActionAddPathAll(){
  qDebug() << "Actions not implemented yet";	
}

void MainUI::slotActionAddMenu(){
  qDebug() << "Actions not implemented yet";	
}

void MainUI::slotActionAddMenuAll(){
  qDebug() << "Actions not implemented yet";	
}

void MainUI::slotActionUpdate(){
  qDebug() << "Actions not implemented yet";	
}

void MainUI::slotActionRemove(){
  qDebug() << "Actions not implemented yet";	
}



// ==========================
// ====== BROWSER TAB =======
// ==========================
void MainUI::initializeBrowserTab(){
  ui->tab_browse->setEnabled(FALSE);
  //Always make sure that the browser starts off on the "home" page
  ui->stacked_browser->setCurrentWidget(ui->page_home);
  //With invisible shortcut buttons
  ui->tool_browse_cat->setVisible(FALSE);
  ui->tool_browse_app->setVisible(FALSE);
  //Clear any items left over from the designer form
  clearScrollArea(ui->scroll_br_home_newapps);
  clearScrollArea(ui->scroll_br_home_cats);
  //Search functionality
  searchTimer = new QTimer();
    searchTimer->setSingleShot(TRUE);
    searchTimer->setInterval(500); // 0.5 sec 
    connect(searchTimer,SIGNAL(timeout()),this,SLOT(slotGoToSearch()) );
  connect(ui->tool_browse_search,SIGNAL(clicked()),this,SLOT(slotGoToSearch()) );
  connect(ui->line_browse_searchbar,SIGNAL(returnPressed()),this,SLOT(slotGoToSearch()) );
}

// === SLOTS ===
void MainUI::slotEnableBrowser(){
  qDebug() << "Repo Ready: Enable the Browser";
  //Now create the browser home page
  slotUpdateBrowserHome();
  //And allow the user to go there
  ui->tool_install_gotobrowserpage->setEnabled(TRUE);
  ui->tab_browse->setEnabled(TRUE);
  //Get the number of installed/available applications and display it
  QString text = QString(tr("PBI's Installed: %1")+"\t"+tr("PBI's Available: %2")).arg(QString::number(PBI->numInstalled), QString::number(PBI->numAvailable));
  ui->statusbar->addWidget(new QLabel(text)); //make this message permanent
}

void MainUI::slotUpdateBrowserHome(){
  //Load the Categories
  QStringList cats = PBI->browserCategories();
  clearScrollArea(ui->scroll_br_home_cats);
  QVBoxLayout *catlayout = new QVBoxLayout;
  QStringList info; info << "name" << "description" << "icon";
  cats.sort(); //sort them alphabetically
  for(int i=0; i<cats.length(); i++){
    QStringList data = PBI->CatInfo(cats[i],info);
    if(!data.isEmpty()){
      LargeItemWidget *item = new LargeItemWidget(cats[i],data[0],data[1],data[2]);
      connect(item,SIGNAL(appClicked(QString)),this,SLOT(slotGoToCategory(QString)) );
      catlayout->addWidget(item);
    }
  }
  catlayout->addStretch(); //add a spacer to the end
  ui->scroll_br_home_cats->widget()->setLayout(catlayout);
  //Load the newest applications
  clearScrollArea(ui->scroll_br_home_newapps);
  QHBoxLayout *newapplayout = new QHBoxLayout;
  QStringList newapps = PBI->getRecentApps();
  for(int i=0; i<newapps.length(); i++){
    QStringList appdata = PBI->AppInfo(newapps[i],QStringList() << "name" << "icon" << "latestversion");
    if(!appdata.isEmpty()){
      SmallItemWidget *item = new SmallItemWidget(newapps[i],appdata[0],appdata[1],appdata[2]);
      connect(item,SIGNAL(appClicked(QString)),this,SLOT(slotGoToApp(QString)) );
      newapplayout->addWidget(item);
    }
  }
  newapplayout->addStretch(); //add a spacer to the end
  newapplayout->setContentsMargins(1,1,1,1);
  ui->scroll_br_home_newapps->widget()->setLayout(newapplayout);
  //Make sure that the newapps scrollarea is the proper fit vertically (no vertical scrolling)
  ui->scroll_br_home_newapps->setMinimumHeight(ui->scroll_br_home_newapps->widget()->minimumSizeHint().height()+ui->scroll_br_home_newapps->horizontalScrollBar()->height());
  
  //ui->page_home->updateGeometry();
  //Make sure the new apps area is invisible if no items available
  if(newapps.isEmpty()){ ui->group_br_home_newapps->setVisible(FALSE); }
  else{ ui->group_br_home_newapps->setVisible(TRUE); }
  
}

void MainUI::slotGoToHome(){
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_home);	
  //Make sure the shortcut buttons are disabled
  ui->tool_browse_cat->setVisible(FALSE);
  ui->tool_browse_app->setVisible(FALSE);
}

void MainUI::slotGoToCategory(QString cat){
  qDebug() << "Show Category:" << cat;
  //Get the apps in this category
  QStringList apps = PBI->browserApps(cat);
  if(apps.isEmpty()){
    qDebug() << "Invalid category:" << cat;
    return;
  }
  clearScrollArea(ui->scroll_br_cat_apps);
  apps.sort();
  QVBoxLayout *applayout = new QVBoxLayout;
  QStringList info; info << "name" << "description" << "icon";
  for(int i=0; i<apps.length(); i++){
    QStringList data = PBI->AppInfo(apps[i],info);
    if(!data.isEmpty()){
      //Trim the description if necessary
      if(data[1].size()>100){ data[1] = data[1].left(100).append("..."); }
      LargeItemWidget *item = new LargeItemWidget(apps[i],data[0],data[1],data[2]);
      connect(item,SIGNAL(appClicked(QString)),this,SLOT(slotGoToApp(QString)) );
      applayout->addWidget(item); 
    }
  }
  applayout->addStretch();
  ui->scroll_br_cat_apps->widget()->setLayout(applayout);
  //Now enable/disable the shortcut buttons
  ui->tool_browse_app->setVisible(FALSE);
  ui->tool_browse_cat->setVisible(TRUE);
    QStringList catinfo = PBI->CatInfo(cat,QStringList() << "name" << "icon");
    ui->tool_browse_cat->setText(catinfo[0]);
    if(catinfo[1].isEmpty()){ catinfo[1] = defaultIcon; }
    ui->tool_browse_cat->setIcon(QIcon(catinfo[1]));
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_cat);

}

void MainUI::slotGoToApp(QString appID){
  qDebug() << "Show App:" << appID;
  //Get the general application info
  QStringList data; 
  data << "name" << "icon" << "author" << "website" << "license" << "type" << "description" \
       << "category" << "requiresroot" << "latestversion" << "latestarch" << "backupversion" << "backuparch";
  data = PBI->AppInfo(appID,data);
  if(data.isEmpty()){
    qDebug() << "Invalid App:" << appID;
    return;
  }
  if(appID=="chntpw"){ qDebug() << "icon:" << data[1]; }
  if(data[1].isEmpty()){ data[1] = defaultIcon; }
  //Now fill the UI with the data
  ui->label_bapp_name->setText(data[0]);
  ui->label_bapp_icon->setPixmap(QPixmap(data[1]));
  if(data[3].isEmpty()){ ui->label_bapp_authorweb->setText(data[2]); }
  else{ ui->label_bapp_authorweb->setText("<a href="+data[3]+">"+data[2]+"</a>"); }
  ui->label_bapp_authorweb->setToolTip(data[3]);
  ui->label_bapp_license->setText(data[4]);
  ui->label_bapp_type->setText(data[5]);
  ui->text_bapp_description->setText(data[6]);
  if(data[8]=="true"){ ui->label_bapp_requiresroot->setText( tr("YES") ); }
  else{ ui->label_bapp_requiresroot->setText( tr("NO") ); }
  //Now determine the appropriate version info
  QString cVer = PBI->isInstalled(appID);
  bool useLatest=FALSE;
  if(cVer.isEmpty()){ useLatest=TRUE; } //not currently installed
  else if(cVer != data[9]){ useLatest=TRUE;} //not the latest version
  //Now put the proper version info on the UI
  if(useLatest){
    ui->label_bapp_version->setText(data[9]);
    ui->label_bapp_arch->setText(data[10]);
    //Need to add size info here
  }else{
    ui->label_bapp_version->setText(data[11]);
    ui->label_bapp_arch->setText(data[12]);
    //Need to add size info here  
  }
  //Now update the download button appropriately
  if(useLatest && cVer.isEmpty()){ //new installation
    ui->tool_bapp_download->setText(tr("Install Now!"));
    ui->tool_bapp_download->setIcon(QIcon(":icons/download.png"));
    ui->tool_bapp_download->setWhatsThis(appID+":::install"); //set for slot 
  }else if(useLatest){ //Upgrade available
    ui->tool_bapp_download->setText(tr("Upgrade"));
    ui->tool_bapp_download->setIcon(QIcon(":icons/upgrade.png"));
    ui->tool_bapp_download->setWhatsThis(appID+":::upgrade"); //set for slot 
  }else{  //Downgrade available
    ui->tool_bapp_download->setText(tr("Downgrade"));
    ui->tool_bapp_download->setIcon(QIcon(":icons/downgrade.png"));
    ui->tool_bapp_download->setWhatsThis(appID+":::downgrade"); //set for slot 
  }
  //Now enable/disable the shortcut buttons
  ui->tool_browse_app->setVisible(TRUE);
    ui->tool_browse_app->setText(data[0]);
    ui->tool_browse_app->setIcon(QIcon(data[1]));
  QStringList catinfo = PBI->CatInfo(Extras::nameToID(data[7]),QStringList() << "name" << "icon");
  if(!catinfo.isEmpty()){
    ui->tool_browse_cat->setVisible(TRUE);
    ui->tool_browse_cat->setText(catinfo[0]);
    if(catinfo[1].isEmpty()){ catinfo[1] = defaultIcon; }
    ui->tool_browse_cat->setIcon(QIcon(catinfo[1]));
  }
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_app);
	
}

void MainUI::slotGoToSearch(){
  searchTimer->stop(); //just in case "return" was pressed to start the search
  QString search = ui->line_browse_searchbar->text();
  if(search.isEmpty()){ return; }
  PBI->searchTerm = search;
  QTimer::singleShot(1,PBI,SLOT(startAppSearch()));
  ui->label_bsearch_info->setText( tr("Searching the application database. Please Wait....") );
    ui->label_bsearch_info->setVisible(TRUE);
    ui->group_bsearch_best->setVisible(FALSE);
    ui->group_bsearch_other->setVisible(FALSE);
  
}
	
void MainUI::slotShowSearchResults(QStringList best, QStringList rest){
  //Now display the search results
  if(best.isEmpty()){
    ui->label_bsearch_info->setText( QString(tr("No Search Results Found for the term: %1")).arg(ui->line_browse_searchbar->text()) );
    ui->label_bsearch_info->setVisible(TRUE);
    ui->group_bsearch_best->setVisible(FALSE);
    ui->group_bsearch_other->setVisible(FALSE);
  }else{
    ui->label_bsearch_info->setVisible(FALSE);
    //Now fill the "best" section
    clearScrollArea(ui->scroll_bsearch_best);
    QVBoxLayout *layout = new QVBoxLayout;
    QStringList info; info << "name" << "description" << "icon";
    for(int i=0; i<best.length(); i++){
      QStringList data = PBI->AppInfo(best[i],info);
      if(!data.isEmpty()){
        //Trim the description if necessary
        if(data[1].size()>100){ data[1] = data[1].left(100).append("..."); }
        LargeItemWidget *item = new LargeItemWidget(best[i],data[0],data[1],data[2]);
        connect(item,SIGNAL(appClicked(QString)),this,SLOT(slotGoToApp(QString)) );
        layout->addWidget(item); 
      }
    }
    layout->addStretch();
    ui->scroll_bsearch_best->widget()->setLayout(layout);
    ui->group_bsearch_best->setVisible(TRUE);
    
    //Now fill the other results
    if(rest.isEmpty()){ ui->group_bsearch_other->setVisible(FALSE); }
    else{
      ui->group_bsearch_other->setVisible(TRUE);
      clearScrollArea(ui->scroll_bsearch_other);
      QVBoxLayout *layout2 = new QVBoxLayout;
      for(int i=0; i<rest.length(); i++){
        QStringList data = PBI->AppInfo(rest[i],info);
        if(!data.isEmpty()){
          //Trim the description if necessary
          if(data[1].size()>100){ data[1] = data[1].left(100).append("..."); }
          LargeItemWidget *item = new LargeItemWidget(rest[i],data[0],data[1],data[2]);
          connect(item,SIGNAL(appClicked(QString)),this,SLOT(slotGoToApp(QString)) );
          layout2->addWidget(item); 
        }
      }
      layout2->addStretch();
      ui->scroll_bsearch_other->widget()->setLayout(layout2);
    }
  }
	
  //need to make sure the search bar still has keyboard focus (just in case)
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_search);
  
}

void MainUI::on_tool_browse_home_clicked(){
  slotGoToHome();
}

void MainUI::on_tool_browse_cat_clicked(){
  slotGoToCategory(ui->tool_browse_cat->text().toLower());
}

void MainUI::on_tool_browse_app_clicked(){
  QString appID = Extras::nameToID(ui->tool_browse_app->text());
  slotGoToApp(appID);
}

void MainUI::on_line_browse_searchbar_textChanged(){
  //Connect this to a singleshot timer, so the search functionality is only
  //  run once after a short wait rather than every time a new character is typed
  
  //Live search only after 3 characters have been typed
  if(ui->line_browse_searchbar->text().length() > 2){
    searchTimer->start();
  }else{
    searchTimer->stop();	  
  }
}

void MainUI::on_group_br_home_newapps_toggled(bool show){
  ui->scroll_br_home_newapps->setVisible(show);
}


// ================================
// ==========  OTHER ==============
// ================================
void MainUI::clearScrollArea(QScrollArea* area){
  QWidget *wgt = area->takeWidget();
  delete wgt; //delete the widget and all children
  area->setWidget( new QWidget() ); //create a new widget in the scroll area
}

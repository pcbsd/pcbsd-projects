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
}
	
void MainUI::ProgramInit()
{ 
   qDebug("Application starting...");
   //Initial Checks
     //Make sure the user is in the proper group
     //Make sure the user is not root
     
   //Startup the intalled PBI's watcher 
   
   //Startup the available PBI's watcher
   
   //Initialize the Installed tab
   // NOTE: Make sure the PBI ID is listed as the "whatsThis" for column 0 in the treewidgetitem
   initializeInstalledTab();
   //Initialize the PBI Browser
   
}

void MainUI::slotSingleInstance(){
  this->raise();
  this->show();
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
    actionMenu->addAction( QIcon(":icons/view-refresh.png"), tr("Update Application"), this, SLOT(slotActionUpdate()) );
    actionMenu->addSeparator();
    actionMenu->addAction( QIcon(":icons/appcafe.png"), tr("Install Menu Icons (All Users)"), this, SLOT(slotActionAddMenuAll()) );
    actionMenu->addAction( QIcon(":icons/appcafe.png"), tr("Install PATH Links (All Users)"), this, SLOT(slotActionAddPathAll()) );
    actionMenu->addSeparator();
    actionMenu->addAction( QIcon(":icons/remove.png"), tr("Remove Applications"), this, SLOT(slotActionRemove()) );
    ui->tool_install_performaction->setMenu(actionMenu);

}

// === SLOTS ===
void MainUI::on_group_install_showinfo_toggled(bool show){
  ui->group_install_info->setVisible(show);	
}

void MainUI::on_tool_install_gotobrowserpage(){
  //When you want to open up the browser page for an application
  QString appID = ui->tree_install_apps->currentItem()->whatsThis(0);
  qDebug() << "Go To Browser page for:"<<appID;
  qDebug() << " - Not implemented yet";
}

void MainUI::on_tool_install_updateall_clicked(){
  //Update all installed PBI's (if possible)
  qDebug() << "Update all PBI's not implemented yet";
}

void MainUI::on_tree_install_apps_itemSelectionChanged(){
  //When an installed PBI is clicked on
  QString appID = ui->tree_install_apps->currentItem()->whatsThis(0);
  //Get the PBI info for that item
  QString name = "sample";
  QString icon = ":icons/appcafe.png";
  QString author = "PC-BSD";
  QString website = "http://www.pcbsd.org";
  QString version = "??.??.?";
  QString license = "BSD";
  bool desktopSC = TRUE; // XDG desktop entries available
  bool menuSC= TRUE; 	 // XDG menu entries available
  bool autoupdate = FALSE;  
  //Create the shortcuts string
  QString shortcuts;
    if(desktopSC && menuSC){ shortcuts = tr("Desktop/Menu"); }
    else if(desktopSC){ shortcuts = tr("Desktop"); }
    else if(menuSC){ shortcuts = tr("Menu"); }
    else{ shortcuts = tr("None"); }
  //Now display that info on the UI
  ui->label_install_app->setText(name);
  ui->label_install_icon->setPixmap( QPixmap(icon) );
  if(website.isEmpty()){ ui->label_install_author->setText(author); }
  else{ ui->label_install_author->setText("<a href="+website+">"+author+"</a>"); }
  ui->label_install_license->setText(license);
  ui->label_install_version->setText(version);
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

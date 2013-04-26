#ifndef _APPCAFE_MAINUI_H
#define _APPCAFE_MAINUI_H

#define PBITRIGGER "/tmp/.pbiupdatetraytrigger"

// Set our defines for the type of mirror the user is running
#define AUTOMIRROR      0
#define SELECTMIRROR    1
#define CUSTOMMIRROR    2

#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

/* QT4 */
#include <QDebug>
#include <QDialog>
#include <QMenu>
#include <QMainWindow>

/* Local Includes */
#include "classes.h"
#include "pbi.h"

//#include "ui_mainUI.h"


/* Change this to switch the default patch tmpdir */
const QString PATCHTMPDIR_DEFAULT( PREFIX + "/tmp" );

#define PBI_UPDATED 0
#define PBI_CHECKING4UPDATES 1
#define PBI_UPDATING 2

namespace Ui {
    class MainUI;
}

class MainUI : public QMainWindow
{
        Q_OBJECT

public:
   explicit MainUI(QWidget* parent = 0);
   void ProgramInit();
   //void showInstalledTab();


public slots:
    void slotSingleInstance();
    
protected:
    //void closeEvent(QCloseEvent *event);

private slots:
  //INSTALLED TAB
  void on_group_install_showinfo_toggled(bool);
  void on_tool_install_gotobrowserpage();
  void on_tool_install_updateall_clicked();
  void on_tree_install_apps_itemSelectionChanged();
  void slotActionAddDesktop();
  void slotActionAddPath();
  void slotActionAddPathAll();
  void slotActionAddMenu();
  void slotActionAddMenuAll();
  void slotActionUpdate();
  void slotActionRemove();

private:
  Ui::MainUI *ui;
  //INSTALLED TAB
  QMenu *actionMenu;
  void initializeInstalledTab();
    
    
signals:
    
} ;

#endif


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
#include <QMessageBox>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QScrollArea>
#include <QScrollBar>

/* Local Includes */
#include "pbiBackend.h"
#include "extras.h"
#include "largeItemWidget.h"
#include "smallItemWidget.h"

namespace Ui {
    class MainUI;
}

class MainUI : public QMainWindow
{
        Q_OBJECT

public:
   explicit MainUI(QWidget* parent = 0);
   void ProgramInit();
   void setWardenMode(QString dir,QString ip); //call before ProgramInit

public slots:
    void slotSingleInstance();
    
protected:
    //void closeEvent(QCloseEvent *event);

private slots:
  //INSTALLED TAB
  void slotRefreshInstallTab();
  void slotPBIStatusUpdate(QString);
  void on_group_install_showinfo_toggled(bool);
  void on_tool_install_gotobrowserpage_clicked();
  void on_tool_install_toggleall_clicked();
  void on_tree_install_apps_itemSelectionChanged();
  void slotActionAddDesktop();
  void slotActionRemoveDesktop();
  void slotActionAddPath();
  void slotActionRemovePath();
  void slotActionAddPathAll();
  void slotActionAddMenu();
  void slotActionRemoveMenu();
  void slotActionAddMenuAll();
  void slotActionAddMime();
  void slotActionRemoveMime();
  void slotActionAddMimeAll();
  void slotActionUpdate();
  void slotActionRemove();
  //BROWSER TAB
  void slotEnableBrowser();
  void slotUpdateBrowserHome();
  void slotGoToHome();
  void slotGoToCategory(QString);
  void slotGoToApp(QString);
  void slotGoToSearch();
  void slotShowSimilarApps(QStringList);
  void slotShowSearchResults(QStringList, QStringList);
  void on_tool_browse_home_clicked();
  void on_tool_browse_cat_clicked();
  void on_tool_browse_app_clicked();
  void on_line_browse_searchbar_textChanged();
  void on_tool_bapp_download_clicked();
  void on_group_br_home_newapps_toggled(bool);
  void on_group_bapp_similar_toggled(bool);
  //OTHER
  void slotDisplayError(QString,QString);

private:
  Ui::MainUI *ui;
  QString wardenDir, wardenIP, defaultIcon;
  PBIBackend *PBI;
  //INSTALLED TAB
  QMenu *actionMenu;
  void initializeInstalledTab();
  void formatInstalledItemDisplay(QTreeWidgetItem *item);
  QStringList getCheckedItems();
  //BROWSER TAB
  QTimer *searchTimer;
  void initializeBrowserTab();
  //OTHER
  QLabel *statusLabel;
  void clearScrollArea(QScrollArea*);
  void slotDisplayStats();
    
    
signals:
    
} ;

#endif


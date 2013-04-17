#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QString>
#include <QUrl>
#include <QDir>
#include <QProcess>
#include <QTimer>
//#include "pbi.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void Reset_Form();  
    void on_actionExit_triggered();
    void on_pushFindIcon_clicked();
    void on_actionAbout_EasyPBI_triggered();
    void on_actionAppCafe_Integration_triggered();
    void on_pushNewModule_clicked();
    void on_create_button_clicked();
    void on_port_info_button_clicked();
    int Check_For_Ports(bool show_warning);
    void on_actionGet_Ports_triggered();
    void downloadFinished(QNetworkReply *reply);
    void extractPT();
    void updateProgress(qint64,qint64);
    void on_pushSavePBISettings_clicked();
    void on_pushChangeOutDir_clicked();
    void on_pushChangeSigFile_clicked();
    void on_pushSelectModule_clicked();
    void on_pushBuildPBI_clicked();
    void PBIbuildFinished(int,QProcess::ExitStatus);
    void updatePBIBuild();
    void killPBIBuild();
    void buildError(QProcess::ProcessError);
    void on_pushSaveLog_clicked();
    void on_pushSelectModuleEdit_clicked();
    void on_pushOpenPortInfo_clicked();
    void on_pushPackageModule_clicked();
    void editRefreshPbiConf();
    void editSavePbiConf();
    void on_pushAddPortBefore_clicked();
    void on_pushRemovePortBefore_clicked();
    void on_pushAddPortAfter_clicked();
    void on_pushRemovePortAfter_clicked();
    void on_pushAddMkOpt_clicked();
    void on_tabWidget_currentChanged(int);
    void on_tabModuleEditor_currentChanged(int);
    void editRefreshResources();
    void on_pushAddResource_clicked();
    void on_pushRemoveResource_clicked();
    void editRefreshEntries();
    void updateEntryLists();
    void on_listEntryPortBins_currentIndexChanged(int index);
    void on_pushRemoveDesktopEntry_clicked();
    void on_pushRemoveMenuEntry_clicked();
    void on_pushAddDesktopEntry_clicked();
    void on_pushAddMenuEntry_clicked();
    void editRefreshLinks();
    void refreshCurrentLinks(bool);
    void on_pushRefreshLinkList_clicked();
    void on_pushClearLinkChanges_clicked();
    void on_pushAddLink_clicked();
    void on_pushAddLinkType_clicked();
    void on_pushRemoveLink_clicked();
    void on_listLinkBins_currentIndexChanged(int);
    void slotSingleInstance();

private:
    Ui::MainWindow *ui;
    void SetupDefaults();
    void loadPBISettings();
    QStringList getPortBinaries(QString);
    QString PROG_DIR, PORTS_DIR, PORT_SELECTED, PBIOUT_DIR, MODOUT_DIR, PROGVERSION, PBISETTINGS_FILE, DEFAULTICON_FILE;
    QProcess *p;
    //PBI_Struct *pbi_data;
};

#endif // MAINWINDOW_H

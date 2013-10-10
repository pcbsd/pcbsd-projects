/********************************************************************************
** Form generated from reading UI file 'LPMain.ui'
**
** Created: Thu Oct 10 15:30:11 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LPMAIN_H
#define UI_LPMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LPMain
{
public:
    QAction *actionClose_WIndow;
    QAction *actionAdvanced;
    QAction *actionBasic;
    QAction *actionManage_Pool;
    QAction *actionUnmanage_Pool;
    QAction *actionRecover_File;
    QAction *actionRestore_Dataset;
    QAction *actionSave_Key_to_USB;
    QAction *actionAdd_Disk;
    QAction *actionRemove_Disk;
    QAction *actionSet_Disk_Offline;
    QAction *actionNew_Snapshot;
    QAction *action_newSnapshot;
    QAction *action_rmSnapshot;
    QAction *action_SaveKeyToUSB;
    QAction *action_startScrub;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QComboBox *combo_pools;
    QSpacerItem *horizontalSpacer;
    QToolButton *tool_configure;
    QToolButton *tool_configBackups;
    QTabWidget *tabWidget;
    QWidget *tab_status;
    QVBoxLayout *verticalLayout_3;
    QFormLayout *formLayout_2;
    QLabel *label;
    QLabel *label_status;
    QLabel *label_2;
    QLabel *label_numdisks;
    QLabel *label_3;
    QLabel *label_latestsnapshot;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QLabel *label_replication;
    QLabel *label_mirror;
    QLabel *label_errors;
    QWidget *tab_restore;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *combo_datasets;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *tool_prevsnap;
    QVBoxLayout *verticalLayout_4;
    QSlider *slider_snapshots;
    QLabel *label_snapshot;
    QToolButton *tool_nextsnap;
    QSpacerItem *verticalSpacer_2;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *tool_viewSnapshot;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *tool_restoreDir;
    QToolButton *tool_restoreFile;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuDisks;
    QMenu *menuSnapshots;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *LPMain)
    {
        if (LPMain->objectName().isEmpty())
            LPMain->setObjectName(QString::fromUtf8("LPMain"));
        LPMain->resize(377, 257);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/tray-icon-idle.png"), QSize(), QIcon::Normal, QIcon::Off);
        LPMain->setWindowIcon(icon);
        actionClose_WIndow = new QAction(LPMain);
        actionClose_WIndow->setObjectName(QString::fromUtf8("actionClose_WIndow"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose_WIndow->setIcon(icon1);
        actionAdvanced = new QAction(LPMain);
        actionAdvanced->setObjectName(QString::fromUtf8("actionAdvanced"));
        actionBasic = new QAction(LPMain);
        actionBasic->setObjectName(QString::fromUtf8("actionBasic"));
        actionManage_Pool = new QAction(LPMain);
        actionManage_Pool->setObjectName(QString::fromUtf8("actionManage_Pool"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/list-add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionManage_Pool->setIcon(icon2);
        actionUnmanage_Pool = new QAction(LPMain);
        actionUnmanage_Pool->setObjectName(QString::fromUtf8("actionUnmanage_Pool"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/list-remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUnmanage_Pool->setIcon(icon3);
        actionRecover_File = new QAction(LPMain);
        actionRecover_File->setObjectName(QString::fromUtf8("actionRecover_File"));
        actionRestore_Dataset = new QAction(LPMain);
        actionRestore_Dataset->setObjectName(QString::fromUtf8("actionRestore_Dataset"));
        actionSave_Key_to_USB = new QAction(LPMain);
        actionSave_Key_to_USB->setObjectName(QString::fromUtf8("actionSave_Key_to_USB"));
        actionAdd_Disk = new QAction(LPMain);
        actionAdd_Disk->setObjectName(QString::fromUtf8("actionAdd_Disk"));
        actionAdd_Disk->setIcon(icon2);
        actionRemove_Disk = new QAction(LPMain);
        actionRemove_Disk->setObjectName(QString::fromUtf8("actionRemove_Disk"));
        actionRemove_Disk->setIcon(icon3);
        actionSet_Disk_Offline = new QAction(LPMain);
        actionSet_Disk_Offline->setObjectName(QString::fromUtf8("actionSet_Disk_Offline"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/network-wired.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSet_Disk_Offline->setIcon(icon4);
        actionNew_Snapshot = new QAction(LPMain);
        actionNew_Snapshot->setObjectName(QString::fromUtf8("actionNew_Snapshot"));
        action_newSnapshot = new QAction(LPMain);
        action_newSnapshot->setObjectName(QString::fromUtf8("action_newSnapshot"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/camera_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_newSnapshot->setIcon(icon5);
        action_rmSnapshot = new QAction(LPMain);
        action_rmSnapshot->setObjectName(QString::fromUtf8("action_rmSnapshot"));
        action_rmSnapshot->setIcon(icon3);
        action_SaveKeyToUSB = new QAction(LPMain);
        action_SaveKeyToUSB->setObjectName(QString::fromUtf8("action_SaveKeyToUSB"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/key.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_SaveKeyToUSB->setIcon(icon6);
        action_startScrub = new QAction(LPMain);
        action_startScrub->setObjectName(QString::fromUtf8("action_startScrub"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/system-run.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_startScrub->setIcon(icon7);
        centralwidget = new QWidget(LPMain);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        combo_pools = new QComboBox(centralwidget);
        combo_pools->setObjectName(QString::fromUtf8("combo_pools"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(combo_pools->sizePolicy().hasHeightForWidth());
        combo_pools->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(combo_pools);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        tool_configure = new QToolButton(centralwidget);
        tool_configure->setObjectName(QString::fromUtf8("tool_configure"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/configure.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_configure->setIcon(icon8);

        horizontalLayout->addWidget(tool_configure);

        tool_configBackups = new QToolButton(centralwidget);
        tool_configBackups->setObjectName(QString::fromUtf8("tool_configBackups"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/drive-removable-media.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_configBackups->setIcon(icon9);

        horizontalLayout->addWidget(tool_configBackups);


        verticalLayout->addLayout(horizontalLayout);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_status = new QWidget();
        tab_status->setObjectName(QString::fromUtf8("tab_status"));
        verticalLayout_3 = new QVBoxLayout(tab_status);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label = new QLabel(tab_status);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        label_status = new QLabel(tab_status);
        label_status->setObjectName(QString::fromUtf8("label_status"));
        label_status->setText(QString::fromUtf8("GOOD"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, label_status);

        label_2 = new QLabel(tab_status);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_2);

        label_numdisks = new QLabel(tab_status);
        label_numdisks->setObjectName(QString::fromUtf8("label_numdisks"));
        label_numdisks->setText(QString::fromUtf8("1"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, label_numdisks);

        label_3 = new QLabel(tab_status);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_3);

        label_latestsnapshot = new QLabel(tab_status);
        label_latestsnapshot->setObjectName(QString::fromUtf8("label_latestsnapshot"));
        label_latestsnapshot->setText(QString::fromUtf8("snapshot"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, label_latestsnapshot);


        verticalLayout_3->addLayout(formLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        line = new QFrame(tab_status);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        label_replication = new QLabel(tab_status);
        label_replication->setObjectName(QString::fromUtf8("label_replication"));
        label_replication->setStyleSheet(QString::fromUtf8("background-color: rgba(10,200,10,100); border-radius: 5px"));
        label_replication->setText(QString::fromUtf8("Replication Status"));
        label_replication->setAlignment(Qt::AlignCenter);
        label_replication->setWordWrap(true);

        verticalLayout_3->addWidget(label_replication);

        label_mirror = new QLabel(tab_status);
        label_mirror->setObjectName(QString::fromUtf8("label_mirror"));
        label_mirror->setStyleSheet(QString::fromUtf8("background-color: rgba(10,100,200,100); border-radius: 5px"));
        label_mirror->setText(QString::fromUtf8("Mirror Status"));
        label_mirror->setAlignment(Qt::AlignCenter);
        label_mirror->setWordWrap(true);

        verticalLayout_3->addWidget(label_mirror);

        label_errors = new QLabel(tab_status);
        label_errors->setObjectName(QString::fromUtf8("label_errors"));
        label_errors->setStyleSheet(QString::fromUtf8("background-color: rgba(230,10,10,100); border-radius: 5px"));
        label_errors->setText(QString::fromUtf8("System Errors"));
        label_errors->setAlignment(Qt::AlignCenter);
        label_errors->setWordWrap(true);

        verticalLayout_3->addWidget(label_errors);

        tabWidget->addTab(tab_status, icon9, QString());
        tab_restore = new QWidget();
        tab_restore->setObjectName(QString::fromUtf8("tab_restore"));
        verticalLayout_2 = new QVBoxLayout(tab_restore);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        combo_datasets = new QComboBox(tab_restore);
        combo_datasets->setObjectName(QString::fromUtf8("combo_datasets"));

        horizontalLayout_4->addWidget(combo_datasets);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tool_prevsnap = new QToolButton(tab_restore);
        tool_prevsnap->setObjectName(QString::fromUtf8("tool_prevsnap"));
        tool_prevsnap->setStyleSheet(QString::fromUtf8(""));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_prevsnap->setIcon(icon10);
        tool_prevsnap->setIconSize(QSize(16, 16));

        horizontalLayout_2->addWidget(tool_prevsnap);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        slider_snapshots = new QSlider(tab_restore);
        slider_snapshots->setObjectName(QString::fromUtf8("slider_snapshots"));
        slider_snapshots->setOrientation(Qt::Horizontal);
        slider_snapshots->setTickPosition(QSlider::TicksAbove);

        verticalLayout_4->addWidget(slider_snapshots);

        label_snapshot = new QLabel(tab_restore);
        label_snapshot->setObjectName(QString::fromUtf8("label_snapshot"));
        label_snapshot->setStyleSheet(QString::fromUtf8("border: 1px solid grey; border-radius: 5px"));
        label_snapshot->setText(QString::fromUtf8("Selected Snapshot"));
        label_snapshot->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_snapshot);


        horizontalLayout_2->addLayout(verticalLayout_4);

        tool_nextsnap = new QToolButton(tab_restore);
        tool_nextsnap->setObjectName(QString::fromUtf8("tool_nextsnap"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_nextsnap->setIcon(icon11);

        horizontalLayout_2->addWidget(tool_nextsnap);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        line_2 = new QFrame(tab_restore);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tool_viewSnapshot = new QToolButton(tab_restore);
        tool_viewSnapshot->setObjectName(QString::fromUtf8("tool_viewSnapshot"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/view.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_viewSnapshot->setIcon(icon12);

        horizontalLayout_3->addWidget(tool_viewSnapshot);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        tool_restoreDir = new QToolButton(tab_restore);
        tool_restoreDir->setObjectName(QString::fromUtf8("tool_restoreDir"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_restoreDir->setIcon(icon13);
        tool_restoreDir->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        horizontalLayout_3->addWidget(tool_restoreDir);

        tool_restoreFile = new QToolButton(tab_restore);
        tool_restoreFile->setObjectName(QString::fromUtf8("tool_restoreFile"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_restoreFile->setIcon(icon14);
        tool_restoreFile->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        horizontalLayout_3->addWidget(tool_restoreFile);


        verticalLayout_2->addLayout(horizontalLayout_3);

        tabWidget->addTab(tab_restore, icon14, QString());
        line_2->raise();

        verticalLayout->addWidget(tabWidget);

        LPMain->setCentralWidget(centralwidget);
        menubar = new QMenuBar(LPMain);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 377, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuDisks = new QMenu(menubar);
        menuDisks->setObjectName(QString::fromUtf8("menuDisks"));
        menuSnapshots = new QMenu(menubar);
        menuSnapshots->setObjectName(QString::fromUtf8("menuSnapshots"));
        LPMain->setMenuBar(menubar);
        statusbar = new QStatusBar(LPMain);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        LPMain->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuDisks->menuAction());
        menubar->addAction(menuSnapshots->menuAction());
        menuFile->addAction(actionManage_Pool);
        menuFile->addAction(actionUnmanage_Pool);
        menuFile->addSeparator();
        menuFile->addAction(action_SaveKeyToUSB);
        menuFile->addSeparator();
        menuFile->addAction(actionClose_WIndow);
        menuDisks->addAction(actionAdd_Disk);
        menuDisks->addAction(actionRemove_Disk);
        menuDisks->addSeparator();
        menuDisks->addAction(actionSet_Disk_Offline);
        menuDisks->addSeparator();
        menuDisks->addAction(action_startScrub);
        menuSnapshots->addAction(action_newSnapshot);
        menuSnapshots->addAction(action_rmSnapshot);

        retranslateUi(LPMain);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LPMain);
    } // setupUi

    void retranslateUi(QMainWindow *LPMain)
    {
        LPMain->setWindowTitle(QApplication::translate("LPMain", "Life Preserver", 0, QApplication::UnicodeUTF8));
        actionClose_WIndow->setText(QApplication::translate("LPMain", "Close Window", 0, QApplication::UnicodeUTF8));
        actionAdvanced->setText(QApplication::translate("LPMain", "Advanced", 0, QApplication::UnicodeUTF8));
        actionBasic->setText(QApplication::translate("LPMain", "Basic", 0, QApplication::UnicodeUTF8));
        actionManage_Pool->setText(QApplication::translate("LPMain", "Manage Pool", 0, QApplication::UnicodeUTF8));
        actionUnmanage_Pool->setText(QApplication::translate("LPMain", "Unmanage Pool", 0, QApplication::UnicodeUTF8));
        actionRecover_File->setText(QApplication::translate("LPMain", "Recover File", 0, QApplication::UnicodeUTF8));
        actionRestore_Dataset->setText(QApplication::translate("LPMain", "Restore Dataset", 0, QApplication::UnicodeUTF8));
        actionSave_Key_to_USB->setText(QApplication::translate("LPMain", "Save Key to USB", 0, QApplication::UnicodeUTF8));
        actionAdd_Disk->setText(QApplication::translate("LPMain", "Add Disk", 0, QApplication::UnicodeUTF8));
        actionRemove_Disk->setText(QApplication::translate("LPMain", "Remove Disk", 0, QApplication::UnicodeUTF8));
        actionSet_Disk_Offline->setText(QApplication::translate("LPMain", "Set Disk Offline", 0, QApplication::UnicodeUTF8));
        actionNew_Snapshot->setText(QApplication::translate("LPMain", "New Snapshot", 0, QApplication::UnicodeUTF8));
        action_newSnapshot->setText(QApplication::translate("LPMain", "New Snapshot", 0, QApplication::UnicodeUTF8));
        action_rmSnapshot->setText(QApplication::translate("LPMain", "Delete Snapshot", 0, QApplication::UnicodeUTF8));
        action_SaveKeyToUSB->setText(QApplication::translate("LPMain", "Save Key to USB", 0, QApplication::UnicodeUTF8));
        action_startScrub->setText(QApplication::translate("LPMain", "Start Scrub", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_startScrub->setStatusTip(QApplication::translate("LPMain", "Check system data integrity", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        tool_configure->setStatusTip(QApplication::translate("LPMain", "Configure the local snapshot policies", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tool_configure->setText(QApplication::translate("LPMain", "Configure", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tool_configBackups->setStatusTip(QApplication::translate("LPMain", "Configure additional data safety procedures", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tool_configBackups->setText(QApplication::translate("LPMain", "...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LPMain", "System State:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("LPMain", "Number of Disks:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LPMain", "Latest Snapshot:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        label_replication->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        tabWidget->setTabText(tabWidget->indexOf(tab_status), QApplication::translate("LPMain", "Status", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tool_prevsnap->setStatusTip(QApplication::translate("LPMain", "Select the previous (older) snapshot", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tool_prevsnap->setText(QApplication::translate("LPMain", "Prev", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tool_nextsnap->setStatusTip(QApplication::translate("LPMain", "Select the next (more recent) snapshot", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tool_nextsnap->setText(QApplication::translate("LPMain", "Next", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tool_viewSnapshot->setStatusTip(QApplication::translate("LPMain", "Browse through the currently selected snapshot", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tool_viewSnapshot->setText(QApplication::translate("LPMain", "View Snapshot", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tool_restoreDir->setStatusTip(QApplication::translate("LPMain", "Revert the entire directory to the selected snapshot", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tool_restoreDir->setText(QApplication::translate("LPMain", "Restore Dir", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tool_restoreFile->setStatusTip(QApplication::translate("LPMain", "Retrieve file(s) from the selected snapshot", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tool_restoreFile->setText(QApplication::translate("LPMain", "Restore File", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_restore), QApplication::translate("LPMain", "Restore Data", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("LPMain", "File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("LPMain", "View", 0, QApplication::UnicodeUTF8));
        menuDisks->setTitle(QApplication::translate("LPMain", "Disks", 0, QApplication::UnicodeUTF8));
        menuSnapshots->setTitle(QApplication::translate("LPMain", "Snapshots", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LPMain: public Ui_LPMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LPMAIN_H

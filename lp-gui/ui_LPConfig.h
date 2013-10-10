/********************************************************************************
** Form generated from reading UI file 'LPConfig.ui'
**
** Created: Thu Oct 10 11:54:18 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LPCONFIG_H
#define UI_LPCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTimeEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LPConfig
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_dataset;
    QTabWidget *tabWidget;
    QWidget *tab_local;
    QFormLayout *formLayout_2;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *combo_local_schedule;
    QTimeEdit *time_local_daily;
    QHBoxLayout *horizontalLayout_3;
    QSpinBox *spin_local_numkeep;
    QComboBox *combo_local_keepunits;
    QLabel *label_4;
    QWidget *tab_remote;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupReplicate;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_9;
    QLineEdit *lineHostName;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_10;
    QLineEdit *lineUserName;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_2;
    QSpinBox *spinPort;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_14;
    QLineEdit *lineRemoteDataset;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QRadioButton *radioSYNC;
    QHBoxLayout *horizontalLayout_6;
    QRadioButton *radioRepTime;
    QTimeEdit *time_replicate;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *tool_apply;
    QToolButton *tool_cancel;

    void setupUi(QDialog *LPConfig)
    {
        if (LPConfig->objectName().isEmpty())
            LPConfig->setObjectName(QString::fromUtf8("LPConfig"));
        LPConfig->resize(374, 327);
        verticalLayout = new QVBoxLayout(LPConfig);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(LPConfig);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        label_dataset = new QLabel(LPConfig);
        label_dataset->setObjectName(QString::fromUtf8("label_dataset"));
        label_dataset->setText(QString::fromUtf8("sample"));

        formLayout->setWidget(0, QFormLayout::FieldRole, label_dataset);


        verticalLayout->addLayout(formLayout);

        tabWidget = new QTabWidget(LPConfig);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_local = new QWidget();
        tab_local->setObjectName(QString::fromUtf8("tab_local"));
        formLayout_2 = new QFormLayout(tab_local);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label_3 = new QLabel(tab_local);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        combo_local_schedule = new QComboBox(tab_local);
        combo_local_schedule->setObjectName(QString::fromUtf8("combo_local_schedule"));

        horizontalLayout_2->addWidget(combo_local_schedule);

        time_local_daily = new QTimeEdit(tab_local);
        time_local_daily->setObjectName(QString::fromUtf8("time_local_daily"));
        time_local_daily->setMaximumTime(QTime(23, 0, 0));
        time_local_daily->setMinimumTime(QTime(1, 0, 0));

        horizontalLayout_2->addWidget(time_local_daily);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        spin_local_numkeep = new QSpinBox(tab_local);
        spin_local_numkeep->setObjectName(QString::fromUtf8("spin_local_numkeep"));
        spin_local_numkeep->setMinimum(1);
        spin_local_numkeep->setMaximum(800);

        horizontalLayout_3->addWidget(spin_local_numkeep);

        combo_local_keepunits = new QComboBox(tab_local);
        combo_local_keepunits->setObjectName(QString::fromUtf8("combo_local_keepunits"));

        horizontalLayout_3->addWidget(combo_local_keepunits);


        formLayout_2->setLayout(1, QFormLayout::FieldRole, horizontalLayout_3);

        label_4 = new QLabel(tab_local);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_4);

        tabWidget->addTab(tab_local, QString());
        tab_remote = new QWidget();
        tab_remote->setObjectName(QString::fromUtf8("tab_remote"));
        verticalLayout_2 = new QVBoxLayout(tab_remote);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupReplicate = new QGroupBox(tab_remote);
        groupReplicate->setObjectName(QString::fromUtf8("groupReplicate"));
        groupReplicate->setCheckable(true);
        groupReplicate->setChecked(false);
        verticalLayout_3 = new QVBoxLayout(groupReplicate);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_9 = new QLabel(groupReplicate);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_8->addWidget(label_9);

        lineHostName = new QLineEdit(groupReplicate);
        lineHostName->setObjectName(QString::fromUtf8("lineHostName"));

        horizontalLayout_8->addWidget(lineHostName);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_10 = new QLabel(groupReplicate);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_7->addWidget(label_10);

        lineUserName = new QLineEdit(groupReplicate);
        lineUserName->setObjectName(QString::fromUtf8("lineUserName"));

        horizontalLayout_7->addWidget(lineUserName);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(groupReplicate);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(label_5);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        spinPort = new QSpinBox(groupReplicate);
        spinPort->setObjectName(QString::fromUtf8("spinPort"));
        spinPort->setMaximum(999999);
        spinPort->setValue(22);

        horizontalLayout_4->addWidget(spinPort);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_14 = new QLabel(groupReplicate);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_5->addWidget(label_14);

        lineRemoteDataset = new QLineEdit(groupReplicate);
        lineRemoteDataset->setObjectName(QString::fromUtf8("lineRemoteDataset"));

        horizontalLayout_5->addWidget(lineRemoteDataset);


        verticalLayout_3->addLayout(horizontalLayout_5);

        groupBox = new QGroupBox(groupReplicate);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        radioSYNC = new QRadioButton(groupBox);
        radioSYNC->setObjectName(QString::fromUtf8("radioSYNC"));
        radioSYNC->setChecked(true);

        gridLayout_3->addWidget(radioSYNC, 0, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        radioRepTime = new QRadioButton(groupBox);
        radioRepTime->setObjectName(QString::fromUtf8("radioRepTime"));

        horizontalLayout_6->addWidget(radioRepTime);

        time_replicate = new QTimeEdit(groupBox);
        time_replicate->setObjectName(QString::fromUtf8("time_replicate"));

        horizontalLayout_6->addWidget(time_replicate);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_9);


        gridLayout_3->addLayout(horizontalLayout_6, 1, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        verticalLayout_2->addWidget(groupReplicate);

        tabWidget->addTab(tab_remote, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        tool_apply = new QToolButton(LPConfig);
        tool_apply->setObjectName(QString::fromUtf8("tool_apply"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/backup-ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_apply->setIcon(icon);
        tool_apply->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        horizontalLayout->addWidget(tool_apply);

        tool_cancel = new QToolButton(LPConfig);
        tool_cancel->setObjectName(QString::fromUtf8("tool_cancel"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_cancel->setIcon(icon1);
        tool_cancel->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        horizontalLayout->addWidget(tool_cancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(LPConfig);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LPConfig);
    } // setupUi

    void retranslateUi(QDialog *LPConfig)
    {
        LPConfig->setWindowTitle(QApplication::translate("LPConfig", "Configure Dataset", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LPConfig", "Data Set:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LPConfig", "Schedule:", 0, QApplication::UnicodeUTF8));
        combo_local_schedule->clear();
        combo_local_schedule->insertItems(0, QStringList()
         << QApplication::translate("LPConfig", "Daily", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LPConfig", "Hourly", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LPConfig", "30 minutes", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LPConfig", "10 minutes", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LPConfig", "5 minutes", 0, QApplication::UnicodeUTF8)
        );
        time_local_daily->setDisplayFormat(QApplication::translate("LPConfig", "@ h AP", 0, QApplication::UnicodeUTF8));
        combo_local_keepunits->clear();
        combo_local_keepunits->insertItems(0, QStringList()
         << QApplication::translate("LPConfig", "Days", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LPConfig", "Total", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("LPConfig", "Keep:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_local), QApplication::translate("LPConfig", "Local Snapshots", 0, QApplication::UnicodeUTF8));
        groupReplicate->setTitle(QApplication::translate("LPConfig", "Replicate on a Remote System", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("LPConfig", "Host Name", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("LPConfig", "User Name", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("LPConfig", "SSH Port", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("LPConfig", "Remote Dataset", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("LPConfig", "Frequency", 0, QApplication::UnicodeUTF8));
        radioSYNC->setText(QApplication::translate("LPConfig", "With snapshot creation (Best for daily snapshots)", 0, QApplication::UnicodeUTF8));
        radioRepTime->setText(QApplication::translate("LPConfig", "Daily at:", 0, QApplication::UnicodeUTF8));
        time_replicate->setDisplayFormat(QApplication::translate("LPConfig", "h AP", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_remote), QApplication::translate("LPConfig", "Replication", 0, QApplication::UnicodeUTF8));
        tool_apply->setText(QApplication::translate("LPConfig", "Apply", 0, QApplication::UnicodeUTF8));
        tool_cancel->setText(QApplication::translate("LPConfig", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LPConfig: public Ui_LPConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LPCONFIG_H

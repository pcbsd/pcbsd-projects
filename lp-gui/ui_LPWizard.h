/********************************************************************************
** Form generated from reading UI file 'LPWizard.ui'
**
** Created: Thu Oct 10 11:54:18 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LPWIZARD_H
#define UI_LPWIZARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTimeEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWizard>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_LPWizard
{
public:
    QWizardPage *wizardPage1;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_dataset;
    QSpacerItem *verticalSpacer_5;
    QWizardPage *wizardPage;
    QGridLayout *gridLayout_8;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_6;
    QGridLayout *gridLayout_4;
    QRadioButton *radioHourly;
    QHBoxLayout *horizontalLayout_11;
    QRadioButton *radioDaily;
    QTimeEdit *timeEdit;
    QSpacerItem *horizontalSpacer_5;
    QRadioButton *radio30Min;
    QRadioButton *radio10Min;
    QRadioButton *radio5Min;
    QSpacerItem *verticalSpacer_3;
    QWizardPage *wizardPage_5;
    QGridLayout *gridLayout_11;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_11;
    QGridLayout *gridLayout_9;
    QHBoxLayout *horizontalLayout_13;
    QRadioButton *radio_keepDays;
    QSpinBox *spin_keepDays;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_14;
    QRadioButton *radioKeepTotal;
    QSpinBox *spin_keepTotal;
    QLabel *label_13;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_4;
    QWizardPage *wizardPage_3;
    QGridLayout *gridLayout_10;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_3;
    QLabel *label_2;
    QGroupBox *groupReplicate;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QRadioButton *radioSYNC;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioRepTime;
    QTimeEdit *time_replicate;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_9;
    QLineEdit *lineHostName;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_10;
    QLineEdit *lineUserName;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_2;
    QSpinBox *spinPort;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_14;
    QLineEdit *lineRemoteDataset;
    QWizardPage *wizardPage_4;
    QGridLayout *gridLayout_2;
    QLabel *labelFinished;

    void setupUi(QWizard *LPWizard)
    {
        if (LPWizard->objectName().isEmpty())
            LPWizard->setObjectName(QString::fromUtf8("LPWizard"));
        LPWizard->setWindowModality(Qt::ApplicationModal);
        LPWizard->resize(474, 381);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/lifepreserver.png"), QSize(), QIcon::Normal, QIcon::Off);
        LPWizard->setWindowIcon(icon);
        LPWizard->setOptions(QWizard::IndependentPages);
        wizardPage1 = new QWizardPage();
        wizardPage1->setObjectName(QString::fromUtf8("wizardPage1"));
        verticalLayout = new QVBoxLayout(wizardPage1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label = new QLabel(wizardPage1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        verticalSpacer_6 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_6);

        label_dataset = new QLabel(wizardPage1);
        label_dataset->setObjectName(QString::fromUtf8("label_dataset"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_dataset->setFont(font);
        label_dataset->setText(QString::fromUtf8("SAMPLE"));
        label_dataset->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_dataset);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);

        LPWizard->addPage(wizardPage1);
        wizardPage = new QWizardPage();
        wizardPage->setObjectName(QString::fromUtf8("wizardPage"));
        gridLayout_8 = new QGridLayout(wizardPage);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        groupBox_4 = new QGroupBox(wizardPage);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_2 = new QVBoxLayout(groupBox_4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setWordWrap(true);

        verticalLayout_2->addWidget(label_6);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        radioHourly = new QRadioButton(groupBox_4);
        radioHourly->setObjectName(QString::fromUtf8("radioHourly"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(radioHourly->sizePolicy().hasHeightForWidth());
        radioHourly->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(radioHourly, 1, 0, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        radioDaily = new QRadioButton(groupBox_4);
        radioDaily->setObjectName(QString::fromUtf8("radioDaily"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(radioDaily->sizePolicy().hasHeightForWidth());
        radioDaily->setSizePolicy(sizePolicy1);
        radioDaily->setChecked(true);

        horizontalLayout_11->addWidget(radioDaily);

        timeEdit = new QTimeEdit(groupBox_4);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setTime(QTime(1, 0, 0));
        timeEdit->setMaximumTime(QTime(23, 0, 0));
        timeEdit->setMinimumTime(QTime(1, 0, 0));

        horizontalLayout_11->addWidget(timeEdit);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_5);


        gridLayout_4->addLayout(horizontalLayout_11, 0, 0, 1, 1);

        radio30Min = new QRadioButton(groupBox_4);
        radio30Min->setObjectName(QString::fromUtf8("radio30Min"));

        gridLayout_4->addWidget(radio30Min, 2, 0, 1, 1);

        radio10Min = new QRadioButton(groupBox_4);
        radio10Min->setObjectName(QString::fromUtf8("radio10Min"));

        gridLayout_4->addWidget(radio10Min, 3, 0, 1, 1);

        radio5Min = new QRadioButton(groupBox_4);
        radio5Min->setObjectName(QString::fromUtf8("radio5Min"));

        gridLayout_4->addWidget(radio5Min, 4, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(48, 74, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_3, 5, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_4);


        gridLayout_8->addWidget(groupBox_4, 0, 0, 1, 1);

        LPWizard->addPage(wizardPage);
        wizardPage_5 = new QWizardPage();
        wizardPage_5->setObjectName(QString::fromUtf8("wizardPage_5"));
        gridLayout_11 = new QGridLayout(wizardPage_5);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        groupBox_5 = new QGroupBox(wizardPage_5);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_3 = new QVBoxLayout(groupBox_5);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_11 = new QLabel(groupBox_5);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setWordWrap(true);

        verticalLayout_3->addWidget(label_11);

        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        radio_keepDays = new QRadioButton(groupBox_5);
        radio_keepDays->setObjectName(QString::fromUtf8("radio_keepDays"));
        sizePolicy1.setHeightForWidth(radio_keepDays->sizePolicy().hasHeightForWidth());
        radio_keepDays->setSizePolicy(sizePolicy1);
        radio_keepDays->setChecked(true);

        horizontalLayout_13->addWidget(radio_keepDays);

        spin_keepDays = new QSpinBox(groupBox_5);
        spin_keepDays->setObjectName(QString::fromUtf8("spin_keepDays"));
        spin_keepDays->setMaximum(800);
        spin_keepDays->setValue(7);

        horizontalLayout_13->addWidget(spin_keepDays);

        label_12 = new QLabel(groupBox_5);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_13->addWidget(label_12);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_6);


        gridLayout_9->addLayout(horizontalLayout_13, 0, 0, 1, 1);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        radioKeepTotal = new QRadioButton(groupBox_5);
        radioKeepTotal->setObjectName(QString::fromUtf8("radioKeepTotal"));
        sizePolicy1.setHeightForWidth(radioKeepTotal->sizePolicy().hasHeightForWidth());
        radioKeepTotal->setSizePolicy(sizePolicy1);

        horizontalLayout_14->addWidget(radioKeepTotal);

        spin_keepTotal = new QSpinBox(groupBox_5);
        spin_keepTotal->setObjectName(QString::fromUtf8("spin_keepTotal"));
        spin_keepTotal->setMaximum(800);
        spin_keepTotal->setValue(7);

        horizontalLayout_14->addWidget(spin_keepTotal);

        label_13 = new QLabel(groupBox_5);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_14->addWidget(label_13);

        horizontalSpacer_8 = new QSpacerItem(17, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_8);


        gridLayout_9->addLayout(horizontalLayout_14, 1, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(48, 74, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_4, 2, 0, 1, 1);


        verticalLayout_3->addLayout(gridLayout_9);


        gridLayout_11->addWidget(groupBox_5, 0, 0, 1, 1);

        LPWizard->addPage(wizardPage_5);
        wizardPage_3 = new QWizardPage();
        wizardPage_3->setObjectName(QString::fromUtf8("wizardPage_3"));
        gridLayout_10 = new QGridLayout(wizardPage_3);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        groupBox_2 = new QGroupBox(wizardPage_3);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        verticalLayout_4->addWidget(label_3);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        verticalLayout_4->addWidget(label_2);

        groupReplicate = new QGroupBox(groupBox_2);
        groupReplicate->setObjectName(QString::fromUtf8("groupReplicate"));
        groupReplicate->setCheckable(true);
        groupReplicate->setChecked(false);
        gridLayout_5 = new QGridLayout(groupReplicate);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        groupBox = new QGroupBox(groupReplicate);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        radioSYNC = new QRadioButton(groupBox);
        radioSYNC->setObjectName(QString::fromUtf8("radioSYNC"));
        radioSYNC->setChecked(true);

        verticalLayout_5->addWidget(radioSYNC);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioRepTime = new QRadioButton(groupBox);
        radioRepTime->setObjectName(QString::fromUtf8("radioRepTime"));

        horizontalLayout_3->addWidget(radioRepTime);

        time_replicate = new QTimeEdit(groupBox);
        time_replicate->setObjectName(QString::fromUtf8("time_replicate"));
        time_replicate->setMaximumDateTime(QDateTime(QDate(2000, 1, 1), QTime(23, 0, 0)));
        time_replicate->setMaximumTime(QTime(23, 0, 0));
        time_replicate->setMinimumTime(QTime(1, 0, 0));

        horizontalLayout_3->addWidget(time_replicate);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_9);


        verticalLayout_5->addLayout(horizontalLayout_3);


        gridLayout_5->addWidget(groupBox, 4, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_9 = new QLabel(groupReplicate);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_8->addWidget(label_9);

        lineHostName = new QLineEdit(groupReplicate);
        lineHostName->setObjectName(QString::fromUtf8("lineHostName"));

        horizontalLayout_8->addWidget(lineHostName);


        gridLayout_5->addLayout(horizontalLayout_8, 0, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_10 = new QLabel(groupReplicate);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_7->addWidget(label_10);

        lineUserName = new QLineEdit(groupReplicate);
        lineUserName->setObjectName(QString::fromUtf8("lineUserName"));

        horizontalLayout_7->addWidget(lineUserName);


        gridLayout_5->addLayout(horizontalLayout_7, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_4 = new QLabel(groupReplicate);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(label_4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        spinPort = new QSpinBox(groupReplicate);
        spinPort->setObjectName(QString::fromUtf8("spinPort"));
        spinPort->setMaximum(999999);
        spinPort->setValue(22);

        horizontalLayout->addWidget(spinPort);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout_5->addLayout(horizontalLayout, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_14 = new QLabel(groupReplicate);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_2->addWidget(label_14);

        lineRemoteDataset = new QLineEdit(groupReplicate);
        lineRemoteDataset->setObjectName(QString::fromUtf8("lineRemoteDataset"));

        horizontalLayout_2->addWidget(lineRemoteDataset);


        gridLayout_5->addLayout(horizontalLayout_2, 3, 0, 1, 1);


        verticalLayout_4->addWidget(groupReplicate);


        gridLayout_10->addWidget(groupBox_2, 0, 0, 1, 1);

        LPWizard->addPage(wizardPage_3);
        wizardPage_4 = new QWizardPage();
        wizardPage_4->setObjectName(QString::fromUtf8("wizardPage_4"));
        gridLayout_2 = new QGridLayout(wizardPage_4);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelFinished = new QLabel(wizardPage_4);
        labelFinished->setObjectName(QString::fromUtf8("labelFinished"));
        labelFinished->setWordWrap(true);

        gridLayout_2->addWidget(labelFinished, 0, 0, 1, 1);

        LPWizard->addPage(wizardPage_4);

        retranslateUi(LPWizard);

        QMetaObject::connectSlotsByName(LPWizard);
    } // setupUi

    void retranslateUi(QWizard *LPWizard)
    {
        LPWizard->setWindowTitle(QApplication::translate("LPWizard", "New Life Preserver", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LPWizard", "This wizard will walk you through the process of setting up a new snapshot schedule for the following ZFS dataset:", 0, QApplication::UnicodeUTF8));
        wizardPage->setTitle(QString());
        wizardPage->setSubTitle(QString());
        groupBox_4->setTitle(QApplication::translate("LPWizard", "Snapshot schedule", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("LPWizard", "Snapshots can be scheduled anywhere from daily, down to every 5 minutes. Snapshots consume very little disk space, and will only grow as the current data on disk changes.  ", 0, QApplication::UnicodeUTF8));
        radioHourly->setText(QApplication::translate("LPWizard", "Hourly", 0, QApplication::UnicodeUTF8));
        radioDaily->setText(QApplication::translate("LPWizard", "Daily @", 0, QApplication::UnicodeUTF8));
        timeEdit->setDisplayFormat(QApplication::translate("LPWizard", "h AP", 0, QApplication::UnicodeUTF8));
        radio30Min->setText(QApplication::translate("LPWizard", "30 minutes", 0, QApplication::UnicodeUTF8));
        radio10Min->setText(QApplication::translate("LPWizard", "10 minutes", 0, QApplication::UnicodeUTF8));
        radio5Min->setText(QApplication::translate("LPWizard", "5 minutes", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("LPWizard", "Snapshot pruning", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("LPWizard", "<html><head/><body><p>The oldest snapshots will be auto-pruned after reaching either the number of days or the total number of snapshots that you specify. </p></body></html>", 0, QApplication::UnicodeUTF8));
        radio_keepDays->setText(QApplication::translate("LPWizard", "Keep", 0, QApplication::UnicodeUTF8));
        spin_keepDays->setSuffix(QString());
        label_12->setText(QApplication::translate("LPWizard", "days worth of snapshots", 0, QApplication::UnicodeUTF8));
        radioKeepTotal->setText(QApplication::translate("LPWizard", "Keep", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("LPWizard", "total snapshots ", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("LPWizard", "Replication Server", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LPWizard", "Replication will keep a copy of your data on a remote system in the case of a total disk failure.", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("LPWizard", "NOTE: A valid replication target system must be running a compatible version of ZFS and have SSH enabled, such as on a FreeNAS system.", 0, QApplication::UnicodeUTF8));
        groupReplicate->setTitle(QApplication::translate("LPWizard", "Replicate my data", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("LPWizard", "Frequency", 0, QApplication::UnicodeUTF8));
        radioSYNC->setText(QApplication::translate("LPWizard", "With snapshot creation (Best for daily snapshots)", 0, QApplication::UnicodeUTF8));
        radioRepTime->setText(QApplication::translate("LPWizard", "Daily at:", 0, QApplication::UnicodeUTF8));
        time_replicate->setDisplayFormat(QApplication::translate("LPWizard", "h AP", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("LPWizard", "Host Name", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("LPWizard", "User Name", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("LPWizard", "SSH Port", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("LPWizard", "Remote Dataset", 0, QApplication::UnicodeUTF8));
        labelFinished->setText(QApplication::translate("LPWizard", "<html><head/><body><p>Your new Life Preserver schedule is now set up! You may go back and change your configuration at any time from within the main Life Preserver window.</p><p><br/></p><p>Click the Finish button to apply these settings and create the new backup schedule.</p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LPWizard: public Ui_LPWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LPWIZARD_H

#include <QTranslator>
#include <qtsingleapplication.h>
#include <QtGui/QApplication>
#include <QDebug>
#include "mainwindow.h"
#define PREFIX QString("/usr/local")

int main(int argc, char ** argv)
{
    QtSingleApplication a(argc, argv);
    if( a.isRunning() )
      return !(a.sendMessage("show"));

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    
    if ( ! QFile::exists(PREFIX + "/share/pcbsd/i18n/pc-pbiapprovetool_" + langCode + ".qm" ) )  langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("pc-pbiapprovetool_") + langCode, PREFIX + "/share/pcbsd/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;


    MainWindow w;
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
    w.show();

    return a.exec();
}

#include <qtranslator.h>
#include <qlocale.h>
#include <qtsingleapplication.h>
#include <QDebug>
#include <QFile>

#include "LPTray.h"
//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

int main( int argc, char ** argv )
{
    QtSingleApplication a(argc, argv);
    if (a.isRunning())
      return !(a.sendMessage("show"));

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/lifePreserver/i18n/LifePreserver_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("LifePreserver_") + langCode, PREFIX + "/share/lifePreserver/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;

    LPTray *w = new LPTray(); 
    w->show();

    QObject::connect( &a, SIGNAL( messageReceived(const QString &) ), w, SLOT( slotSingleInstance() ) );
    return a.exec();
}

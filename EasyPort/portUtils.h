#ifndef _EASYPORT_PORTUTILS_H
#define _EASYPORT_PORTUTILS_H

#include <QStringList>
#include <QString>

class PortUtils{
public:
	static QStringList generateNewMakefile(QString name, QString version, QString category, QString mastersite, QString maintainer, QString comment);
	static QStringList findPortCategories(QString portdir);

};

#endif

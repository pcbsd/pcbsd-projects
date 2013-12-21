#ifndef _EASYPORT_PORTUTILS_H
#define _EASYPORT_PORTUTILS_H

#include <QStringList>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QObject>
#include <QCoreApplication>
#include <QDebug>

class PortUtils{
public:
	static QStringList generateNewMakefile(QString name, QString version, QString category, QString mastersite, QString maintainer, QString comment);
	static QStringList findPortCategories(QString portdir);
	static QStringList getMakefileConfigOpts();
	
	//General Utilities
	static bool runCmd(QString indir, QString cmd, QStringList args = QStringList());
	static void compressDir(QString dirPath);
	static bool createFile(QString fileName, QStringList contents);
	static bool removeFile(QString fileName);
	static QStringList readFile(QString filePath);
};

#endif

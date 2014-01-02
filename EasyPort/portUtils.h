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
	//Port Information utilities
	static QStringList findPortCategories(QString portdir);
	static QStringList getMakefileConfigOpts();
	
	//Port configuration simplifications
        static QStringList generateNewMakefile(QString name, QStringList vars, QStringList vals);
	static QStringList generatePkgDescr(QString description, QString website="", QString author="");
	static QStringList insertMakeFileConfig(QStringList current, QString var, QString val, bool replace=true);

	//General Utilities
	static bool runCmd(QString indir, QString cmd, QStringList args = QStringList());
	static void compressDir(QString dirPath);
	static bool createFile(QString fileName, QStringList contents);
	static bool removeFile(QString fileName);
	static QStringList readFile(QString filePath);
};

#endif

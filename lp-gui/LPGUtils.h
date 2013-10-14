#ifndef _LP_GUTILS_H
#define _LP_GUTILS_H

#include <QMessageBox>
#include <QDialog>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QDir>

#include "LPBackend.h"
#include "LPContainers.h"

class LPGUtils{
public:
	static LPDataset loadPoolData(QString zpool); //Load backend data into container
	static QString generateReversionFileName(QString filename, QString destDir);
	static bool revertFile(QString oldPath, QString newPath); //copy a file out of a snapshot
	static QStringList revertDir(QString oldPath, QString newPath); //copy a dir out of a snapshot
};

#endif

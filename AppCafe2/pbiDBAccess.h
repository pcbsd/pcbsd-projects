#ifndef _APPCAFE_PBI_DATABASE_ACCESS_H
#define _APPCAFE_PBI_DATABASE_ACCESS_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

class PBIDBAccess{
public:

	PBIDBAccess(){
	  DBDir = new QDir();
	}
	~PBIDBAccess(){}
	bool setDBPath(QString); //must be set before anything else!!
	bool setRepo(QString);
	QStringList availableRepos();
	//Main access functions
	QStringList installed(); // return list of ID strings for installed PBI's
	QStringList installedPbiInfo(QString pbiID);
	bool installedPbiNeedsRoot(QString pbiID);
	bool installedPbiAutoUpdate(QString pbiID);
	bool installedPbiHasXdgDesktop(QString installPath);
	bool installedPbiHasXdgMenu(QString installPath);
	QString indexFilePath();
	QString metaFilePath();
	QStringList parseIndexLine(QString line);
	QStringList parseAppMetaLine(QString line);
	QStringList parseCatMetaLine(QString line);
	QString remoteToLocalIcon(QString name, QString remoteIconPath);
	
private:
	QString currentRepoNumber, currentRepoID, DBPath;
	QDir *DBDir;
	QString readOneLineFile(QString);
	
};

#endif

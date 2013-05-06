#ifndef _APPCAFE_PBI_BACKEND_H
#define _APPCAFE_PBI_BACKEND_H

/*  TERMINOLOGY NOTE:
	"PBI" refers to locally installed applications
	"APP" refers to available applications in the repo database
	"CAT" refers to available categories in the repo database
*/
#include <QFileSystemWatcher>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDate>

#include "containers.h"
#include "extras.h"
#include "pbiDBAccess.h"

class PBIBackend : public QObject{
	Q_OBJECT

public:
	//Initializations
	PBIBackend();
	~PBIBackend(){}
	// Clean Start/Stop functions
	void setWardenMode(QString dir, QString ip);
	void setDownloadDir(QString);
	bool start();
	int numInstalled, numAvailable;
	//Search variables foro public slot inputs
	QString searchTerm;
	QString searchSimilar;
	// Main Listing functions
	QStringList installedList(); //return unique ID's of all installed PBI's
	QStringList browserCategories(); //return list of available browser categories
	QStringList browserApps( QString catID ); //list all apps in the given category
	QStringList getRecentApps(); //list all recent applications/update
	// Local/Repo Interaction
	QString isInstalled(QString appID); //returns version that is installed (if possible);
	QString upgradeAvailable(QString pbiID); //returns ID for PBI upgrade (if available)
	QString downgradeAvailable(QString pbiID); //returns ID for PBI downgrade (if available)
	void upgradePBI(QStringList pbiID); //start upgrade process for list of PBI's
	void downgradePBI(QStringList pbiID); //start downgrade process
	void removePBI(QStringList pbiID); //start the removal process
	void stopUpdate(QStringList pbiID); //stop upgrade/downgrade/removal process
	void installApp(QStringList appID); //install application from the repo
	// Information Retrieval
	QStringList PBIInfo( QString pbiID, QStringList infoList);
	QStringList CatInfo( QString catID, QStringList infoList);
	QStringList AppInfo( QString appID, QStringList infoList);

public slots:
	void startAppSearch(); //get list of apps containing the search string (SIGNAL OUTPUT ONLY)
	void startSimilarSearch(); //get list of apps that are similar to the input app
	
private:
	//variables - warden mode
	QString wardenDir, wardenIP;
	bool wardenMode;
	//variables - filesystem watcher;
	QFileSystemWatcher *watcher;
	QString baseDBDir; //  This is always /var/db/pbi/
	QString repoNumber, DBDir;
	//variables - database
	PBIDBAccess *sysDB;
	QHash<QString, InstalledPBI> PBIHASH;
	QHash<QString, MetaCategory> CATHASH;
	QHash<QString, MetaPBI> APPHASH;
	bool noRepo;
	//variables - processes
	QString baseDlDir, dlDir; // download/install directories
	QString sysArch; //system architecture
	QStringList PENDINGDL, PENDINGINSTALL, PENDINGREMOVAL, PENDINGUPDATE;
	WorkProcess *dlProc, *instProc, *remProc, *updProc;
	//functions
	
private slots:
	//Process functions
	void checkProcesses();
	void startDownloadProcess(QString);
	void startInstallProcess(QString);
	void startRemovalProcess(QString);
	void startUpdateProcess(QString);
	// Database watcher
	void slotSyncToDatabase();
	void syncPBI(QString);
	void syncCurrentRepo();
	
signals:
	void RepositoryInfoReady();
	void ErrorNoRepo();
	void LocalPBIChanges();
	void PBIStatusChange(QString pbiID);
	//Process Signals
	void DownloadError(QString errormsg);
	void InstallationError(QString errormsg);
	void RemovalError(QString errormsg);
	void UpdateError(QString errormsg);
	void ProcessFinished(); //mainly used internally
	//Search results
	void SearchComplete(QStringList, QStringList);// "best" and "rest" results lists
	void SimilarFound(QStringList);
};

#endif

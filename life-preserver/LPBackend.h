#ifndef _LP_BACKEND_H
#define _LP_BACKEND_H

#include <QProcess>
#include <QString>
#include <QStringList>

//Class of static functions for using the "lpreserver" backend
class LPBackend{

public:	
	//Informational
	static QStringList listPossibleDatasets(); //list all possible datasets on the system
	static QStringList listDatasets(); //list all current lifepreserver datasets
	static QStringList listSnapshots(QString dataset); //list all snapshots for a particular dataset
	static QStringList listReplicationTargets(); //list all datasets with replication enabled
	//Snapshop Management
	static void newSnapshot(QString dataset);
	static void removeSnapshot(QString dataset, QString snapshot);
	static void revertSnapshot(QString dataset, QString snapshot); //revert to given snapshot
	static void browseSnapshot(QString dataset, QString snapshot);
	//Replication Management
	static void setupReplication(QString dataset, QString remotehost, QString user, int port, QString remotedataset, int time);
	static void removeReplication(QString dataset);

};
#endif

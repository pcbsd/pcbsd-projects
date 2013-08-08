#ifndef _LP_BACKEND_H
#define _LP_BACKEND_H

#include <QProcess>

//Class of static functions for using the "lpreserver" backend
class LPBackend{

public:	
	//Informational
	QStringList listDatasets(); //list all lifepreserver datasets
	QStringList listSnapshots(QString dataset); //list all snapshots for a particular dataset
	QStringList listReplicationTargets(); //list all datasets with replication enabled
	//Snapshop Management
	void newSnapshot(QString dataset);
	void removeSnapshot(QString dataset, QString snapshot);
	void revertSnapshot(QString dataset, QString snapshot); //revert to given snapshot
	void browseSnapshot(QString dataset, QString snapshot);
	//Replication Management
	void setupReplication(QString dataset, QString remotehost, QString user, int port, QString remotedataset, int time);
	void removeReplication(QString dataset);

};
#endif
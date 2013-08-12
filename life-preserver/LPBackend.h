#ifndef _LP_BACKEND_H
#define _LP_BACKEND_H

#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDebug>

//Class of static functions for using the "lpreserver" backend
class LPBackend{

public:	
	//Informational
	static QStringList listPossibleDatasets(); //list all possible datasets on the system
	static QStringList listDatasets(); //list all current lifepreserver datasets
	static QStringList listSnapshots(QString dataset); //list all snapshots for a particular dataset
	static QStringList listReplicationTargets(); //list all datasets with replication enabled
	//Dataset Management
	static bool setupDataset(QString dataset, int time, int numToKeep); //add or configure dataset
	static bool removeDataset(QString dataset);
	static bool datasetInfo(QString dataset, int& time, int& numToKeep); //get current settings for a dataset
	//Snapshop Management
	static bool newSnapshot(QString dataset);
	static bool removeSnapshot(QString dataset, QString snapshot);
	static bool revertSnapshot(QString dataset, QString snapshot); //revert to given snapshot
	static bool browseSnapshot(QString dataset, QString snapshot);
	//Replication Management
	static bool setupReplication(QString dataset, QString remotehost, QString user, int port, QString remotedataset, int time);
	static bool removeReplication(QString dataset);
	static bool replicationInfo(QString dataset, QString& remotehost, QString& user, int& port, QString& remotedataset, int& time);

};
#endif

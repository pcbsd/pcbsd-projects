#ifndef _PC_DEVMANAGER_BACKEND_H
#define _PC_DEVMANAGER_BACKEND_H

class Backend{
public:
	//Listing Groups of devices
	static QStringList listDevices(QString filter, bool topLevelOnly = true);
	static QStringList listDeviceInfo(QStringList devs);
	//Single Device Options

private:
	//Utility functions for running QProcesses
	static QStringList cmdOutput(QString cmd);
	static int runCmd(QString cmd);

};
#endif

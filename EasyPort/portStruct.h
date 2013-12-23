#ifndef _EASY_PORT_STRUCT_H
#define _EASY_PORT_STRUCT_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QSettings>

class PortStruct{
public:
	PortStruct();
	~PortStruct();

	//Port information
	bool loadPort(QString);
	QString portPath();
	QStringList mainFiles(); //list the main files EasyPort recognizes
	QStringList extraFiles(); //list of all other files in the port

	//General Settings
	QString portsDir(); //which port directory to use on the system
	void setPortsDir(QString);
	QString newPortDir(); //where to place new ports
	void setNewPortDir(QString);

private:
	QSettings *SETTINGS; //for global settings and such
	QString basepath;

};

#endif

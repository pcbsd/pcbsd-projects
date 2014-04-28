/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
#ifndef _APPCAFE_CONTAINERS_H
#define _APPCAFE_CONTAINERS_H

#include <QString>
#include <QProcess>
#include <QObject>
#include <QDebug>

// Info Container for PbiNg categories
class NGCat{
  public:
	QString name, icon, description, portcat;
  
	NGCat(){}
	~NGCat(){}
};

// Info Container for PbiNg applications
class NGApp{
  public:
	QString origin, pbiorigin, name, version, icon, description, shortdescription, portcat, maintainer, website, license, type, category, size;
	QString installedversion, installedsize, installedwhen;
	bool isInstalled, isLocked, isOrphan;
	QStringList similarApps, needsPkgs, tags, screenshots, possiblePlugins, installedPlugins, buildOptions;
	bool hasDE, hasME, hasMT; //desktop/menu entries, mimetypes, path links
  
	NGApp(){
	  isInstalled=false; isLocked=false; isOrphan=false;
	  hasDE=false; hasME=false; hasMT=false;
	}
	~NGApp(){}
};

#endif

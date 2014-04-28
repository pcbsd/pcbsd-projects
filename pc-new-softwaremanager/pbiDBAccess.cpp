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
 #include "pbiDBAccess.h"

PBIDBAccess::PBIDBAccess(){
  proc = new QProcess;
  proc->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
  proc->setProcessChannelMode(QProcess::MergedChannels);
  //PKGAVAILABLE.clear(); PKGINSTALLED.clear();
  //PBIAVAIL.clear(); CATAVAIL.clear(); PKGAVAIL.clear();
}

PBIDBAccess::~PBIDBAccess(){
  delete proc;	
}

// ========================================
// =======  PUBLIC ACCESS FUNCTIONS =======
// ========================================
void PBIDBAccess::setCurrentJail(QString jailID, bool localreload, bool allreload){
  //The PBI/Cat lists are the same for all jails
  syncPkgRepoList(allreload); //load the available PKG repo list
  syncPkgInstallList(jailID, localreload || allreload); //load the installed PKG list
  syncPbiRepoLists(allreload); //load the PBI index lists
}

QHash<QString, NGApp> PBIDBAccess::getRawAppList(){ //PBI-apps that can be installed
  return PBIAVAIL;
}

QHash<QString, NGCat> PBIDBAccess::Categories(){  //All categories for ports/pbi's (unified)
  return CATAVAIL;
}

QHash<QString, NGApp> PBIDBAccess::DetailedAppList(){
  QHash<QString, NGApp> apps = PBIAVAIL; //start with the current raw info
  //Now update the raw info to reflect the current install status
  QStringList keys = apps.keys();
  for(int i=0; i<keys.length(); i++){
    NGApp app = updateAppStatus( apps[keys[i]] );
    apps.insert(keys[i], app); //replace the current info with the updated stuff
  }
  return apps;
}

QHash<QString, NGApp> PBIDBAccess::DetailedPkgList(){
   syncLargePkgRepoList(); //Now fill out the details for all available package (can take a while)
  QHash<QString, NGApp> hash;
  for(int i=0; i<PKGINSTALLED.length(); i++){
    NGApp app;
      app.origin = PKGINSTALLED[i];
      app = getRemotePkgDetails(app); //fill it with remote info first
      app = getLocalPkgDetails(app); //now fill it with local info
    hash.insert(app.origin, app);
  }
  return hash;
}

NGApp PBIDBAccess::getLocalPkgDetails(NGApp app){
  //Simply set the proper bits in the container for locally installed apps
  // NOTE: This is dependant upon which jail is being probed
  QStringList args;
  if( !jailLoaded.isEmpty() ){
    args << "-j" << jailLoaded;
  }
  args << "query" << "%v::::%sh::::%k::::%t::::%a" << app.origin;
  QString out = runCMD("pkg", args);
  if(out.isEmpty()){
    app.isInstalled=false;
  }else{
    app.isInstalled=true;
    app.installedversion = out.section("::::",0,0);
    app.installedsize = out.section("::::",1,1);
    app.isLocked = (out.section("::::",2,2) == "1");
    QString timestamp = out.section("::::",3,3);
      app.installedwhen = QDateTime::fromMSecsSinceEpoch( timestamp.toLongLong() ).toString(Qt::DefaultLocaleShortDate);
    app.isOrphan = (out.section("::::",4,4) == "1");
  }
  return app;
}

NGApp PBIDBAccess::getRemotePkgDetails(NGApp app){
  //This is jail-independant
  QStringList args; args << "rquery" << "%n::::%v::::%m::::%w::::%c::::%e::::%sh::::%L" << app.origin;
  QString out = runCMD("pkg", args);
  if(out.isEmpty()){ return app; } //no changes since no info
  app.name = out.section("::::",0,0);
  app.version = out.section("::::",1,1);
  app.maintainer = out.section("::::",2,2);
  app.website = out.section("::::",3,3);
  app.shortdescription = cleanupDescription( out.section("::::",4,4).split("\n") );
  app.description = cleanupDescription( out.section("::::",5,5).split("\n") );
  app.size = out.section("::::",6,6);
  app.license = out.section("::::",7,7);
    app.license.replace("\n",", ");
  //Now get the build options
  args.clear(); args << "rquery" << "%Ok=%Ov::::(%OD)" << app.origin;
  app.buildOptions = cmdOutput("pkg", args);
    app.buildOptions.replaceInStrings("::::()",""); //for empty descriptions
    app.buildOptions.replaceInStrings("::::"," ");  //for existing descriptions
  //Now the hard-codes values
  app.type = "pkg";
  app.portcat = app.origin.section("/",0,0);
  
  return app;
}


QStringList PBIDBAccess::getRawPkgList(){ //All packages that can be installed
  return PKGAVAILABLE; 
}

QStringList PBIDBAccess::getRawInstalledPackages(){ //Installed Packages on the system
  return PKGINSTALLED;
}
	
NGApp PBIDBAccess::updateAppStatus(NGApp app){
  //This function assumes that the internal lists are currently up-to-date
  bool ok = true;
  if( !app.needsPkgs.isEmpty() ){
    //Check for all these package, not just the current pkg
    for(int i=0; i<app.needsPkgs.length(); i++){
      if( !PKGINSTALLED.contains(app.needsPkgs[i]) ){
	ok = false;
	break; //don't bother checking further
      }
    }
  }
  if(ok){
    app = getLocalPkgDetails(app); //this will check the actual app package
  }else{
    app.isInstalled = false;
  }
  return app;
}

QStringList PBIDBAccess::AppMenuEntries(NGApp app){
  if(app.pbiorigin.isEmpty() || !app.hasME ){ return QStringList(); }
  QDir mdir(PBI_DBDIR+app.pbiorigin+"/xdg-menu");
  QStringList files = mdir.entryList(QStringList() << "*.desktop", QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
  for(int i=0; i<files.length(); i++){
    files[i] = mdir.absoluteFilePath(files[i]);
  }
  return files;
}

// ========================================
// =======  PRIVATE ACCESS FUNCTIONS ======
// ========================================

//------------------
//  SYNCERS
//------------------
void PBIDBAccess::syncPkgRepoList(bool reload){
  //All packages available on the repo (jail independant)
  if(PKGAVAILABLE.isEmpty() || reload ){
    PKGAVAILABLE.clear();
    PKGAVAILABLE = cmdOutput("pkg rquery -a %o");
    PKGAVAILABLE.sort();
  }
  qDebug() << " - end sync";
}

void PBIDBAccess::syncPkgInstallList(QString jailID, bool reload){
  if(PKGINSTALLED.isEmpty() || reload || (jailLoaded!=jailID) ){
    if(jailID.isEmpty()){
      PKGINSTALLED = cmdOutput("pkg query -a %o");
    }else{
      PKGINSTALLED = cmdOutput("pkg", QStringList() << "-j"<<jailID<<"query"<<"-a"<<"%o" );
    }
    PKGINSTALLED.sort(); //category/name alphabetically
    jailLoaded = jailID; //keep track of which jail this list is for
  }
}

void PBIDBAccess::syncLargePkgRepoList(bool reload){
  //Detailed list of packages available on the repo (can take a while)
  if(PKGAVAIL.isEmpty() || reload){
    PKGAVAIL.clear();
    for(int i=0; i<PKGAVAILABLE.length(); i++){
      NGApp app;
	    app.origin = PKGAVAILABLE[i];
	    app = getRemotePkgDetails(app);
      PKGAVAIL.insert(PKGAVAILABLE[i], app);
    }
  }
}

void PBIDBAccess::syncPbiRepoLists(bool reload){
  // NOTE: Uses the PKGAVAILABLE list  - check your sync order!!
	
  //All PBIs/Categories available in the index (jail independant)
  if(PBIAVAIL.isEmpty() || CATAVAIL.isEmpty() || reload){
    PBIAVAIL.clear(); CATAVAIL.clear();
    QStringList index = readIndexFile();
    index.sort(); //make sure that categories are first (Cat=, PBI=)
    for(int i=0; i<index.length(); i++){
      if(index[i].startsWith("Cat=")){
	NGCat cat = parseNgCatLine( index[i].section("=",1,50) );
	if(!cat.portcat.isEmpty()){
	  CATAVAIL.insert(cat.portcat, cat);
	}
      }else if(index[i].startsWith("PBI=")){
	NGApp app = parseNgIndexLine( index[i].section("=",1,50) );
	//Prune the PBI app based upon package availability
	if( !app.origin.isEmpty() && PKGAVAIL.contains(app.origin) ){
	  //Also check for additional required packages
	  bool ok = true;
	  for(int i=0; i<app.needsPkgs.length(); i++){
	    if( !PKGAVAIL.contains(app.needsPkgs[i]) ){ ok = false; break; }
	  }
	  if(ok){
	    PBIAVAIL.insert(app.origin, app);
	  }
	}
      }
    }
  } //end sync if necessary
}

//-------------------
//   PARSERS
//-------------------
NGApp PBIDBAccess::parseNgIndexLine(QString line){
  //PBI= index line: [port, name, +ports, author, website, license, app type, category, tags, maintainer, shortdesc, fulldesc, screenshots, related, plugins, conf dir]
  // screenshots = list of URL's for screenshots (empty space delimiter? Note "%20"->" " conversion within a single URL)
  // related = list of ports that are similar to this one
  QStringList lineInfo = line.split("::::");
  NGApp app;
	if(lineInfo.length() < 16){ return app; } //invalid entry - skip it
	app.origin = lineInfo[0];
	app.name = lineInfo[1];
	app.needsPkgs = lineInfo[2].split(" ");
	app.author = lineInfo[3];
	app.website = lineInfo[4];
	app.license = lineInfo[5];
	app.type = lineInfo[6];
	app.category = app.origin.section("/",0,0);
	app.tags = lineInfo[8].split(",");
	app.maintainer = lineInfo[9];
	app.shortdescription = cleanupDescription( lineInfo[10].split("<br>") );
	app.description = cleanupDescription( lineInfo[11].split("<br>") );
	app.screenshots = lineInfo[12].split(" ");
	    app.screenshots = app.screenshots.replaceInStrings("%20"," ");
	app.similarApps = lineInfo[13].split(" ");
	app.possiblePlugins = lineInfo[14].split(" ");
	app.pbiorigin = lineInfo[15];
	//Now check for different types of shortcuts for this app
	app.hasDE = QFile::exists( PBI_DBDIR+app.pbiorigin+"/xdg-desktop" );
	app.hasME = QFile::exists( PBI_DBDIR+app.pbiorigin+"/xdg-menu" );
	app.hasMT = QFile::exists( PBI_DBDIR+app.pbiorigin+"/xdg-mime" );
	//Now create the path to the icon in the index
	app.icon = PBI_DBDIR+app.pbiorigin+"/icon.png";
	
  return app;
}

NGCat PBIDBAccess::parseNgCatLine(QString line){
  //Cat= index line: [name, icon, description, freebsd category]
  QStringList lineInfo = line.split("::::");
  NGCat cat;
	if(lineInfo.length() < 4){ return cat; } //invalid entry - skip it
	cat.name = lineInfo[0];
	cat.icon = PBI_DBDIR+"PBI-cat-icons/"+lineInfo[1];
	cat.description = cleanupDescription( lineInfo[2].split("<br>") );
	cat.portcat = lineInfo[3];
  
  return cat;
}


//----------------------
//   UTILITIES
//-----------------------
QString PBIDBAccess::readOneLineFile(QString path){
  QFile file(path);
  if(!file.exists()){ return ""; } //Return nothing for missing file
  //Now read the file
  QString output;
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while(!in.atEnd()){
      if(!output.isEmpty()){ output.append("\n"); }
      output.append( in.readLine() ); 
    }
    file.close();
  }
  return output;
}

QString PBIDBAccess::runCMD(QString cmd, QStringList args){
  //Small function to run quick database modification commands or queries
  QString output;
  if(args.isEmpty()){
    proc->start(cmd);
  }else{
    proc->start(cmd, args);
  }
  if(proc->waitForFinished(30000)){
    output = proc->readAll();	   
  }else{
    proc->terminate();
    output = "Process timed out (30 sec)";
  }
  if(output.endsWith("\n")){ output.chop(1); }
  output = output.simplified();
  return output;
}

QStringList PBIDBAccess::cmdOutput(QString cmd, QStringList args){
  //Small function to run quick database modification commands
  QString output;
  if(args.isEmpty()){
    proc->start(cmd);
  }else{
    proc->start(cmd, args);
  }
  if(proc->waitForFinished(60000)){
    output = proc->readAll();	   
  }else{
    proc->terminate();
    output = "Process timed out (60 sec)";
  }
  if(output.endsWith("\n")){ output.chop(1); }
  output = output.simplified();
  return output.split("\n");
}

QString PBIDBAccess::cleanupDescription(QStringList tmp){
  for(int i=1; i<tmp.length(); i++){
    //tmp[i-1] = tmp[i-1].simplified();
    tmp[i] = tmp[i].simplified();
    if( tmp[i].startsWith("WWW: ") ){
      //Remove the website URL from the end, it is already accounted for elsewhere
      tmp.removeAt(i);
      i--;
    }else if(tmp[i-1].isEmpty() || tmp[i].isEmpty() ){}
    else if(tmp[i-1].endsWith(".") || tmp[i-1].endsWith(":") || tmp[i-1].endsWith(";") || tmp[i-1].endsWith("?") || tmp[i-1].endsWith("!") ){}
    else if( tmp[i].startsWith("*") || tmp[i].startsWith("0") || tmp[i].startsWith("-") || tmp[i].startsWith("o ") ){}
    else{
      //if(DEBUG){ qDebug() << " - Bad Line Break:\n" << tmp[i-1] << "<br>" << tmp[i]; }
      //Bad line break, combine it with the previous line
      tmp[i-1].append(" "+tmp[i]);
      tmp.removeAt(i);
      i--;
    }
  }
  QString desc = tmp.join("\n");
  desc.remove("\\\\"); //Remove any double backslashes
  return desc;
}

QStringList PBIDBAccess::readIndexFile(){
  QFile file(PBI_DBDIR+"PBI-INDEX");
  if(!file.exists()){ return QStringList(); } //Return nothing for missing file
  //Now read the file
  QStringList output;
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while(!in.atEnd()){
      output << in.readLine(); 
    }
    file.close();
  }
  return output;
}

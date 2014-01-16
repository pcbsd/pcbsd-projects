#include "portUtils.h"

// ===============
//    INFORMATION
// ===============
QStringList PortUtils::findPortCategories(QString portdir){
  //search through the available ports tree and find the valid categories
  QStringList cats;
  QDir dir(portdir);
  if(dir.exists()){
    QStringList raw = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    for(int i=0; i<raw.length(); i++){
      //check each raw directory to see if it is a valid ports collection
      if( (raw[i]!="distfiles") && raw[i].at(0).isLower() ){
      	cats << raw[i];      
      }
    }
  }
  return cats;
}

QStringList PortUtils::getMakefileConfigOpts(){
  //Output format: <variable>:::<format>:::<description>
  /*   <format>: 
	text - single line/word of text
	textlist - list of distinct text
	port - FreeBSD port category/name
	portfile - any file from a FreeBSD port
	portbin - binary from a FreeBSD port
	portlib - library from a FreeBSD port
	cats - FreeBSD category list
	url - Some kind of valid URL(s)
	bool - option is only set if it is listed
	int - some counting number (greater than 0)
  */
	
  QStringList opt;
  //These should be listed in their order of appearance in the Makefile
  opt << "PORTNAME:::text:::"+QObject::tr("Name of software (required)");
  opt << "PORTVERSION:::text:::"+QObject::tr("Current version of the application");
  opt << "PORTREVISION:::text:::"+QObject::tr("Additional port version number (appended to app version)");
  opt << "PORTEPOCH:::text:::"+QObject::tr("Increase this if the new app version is lower numerically");
  opt << "PKGNAMEPREFIX:::text:::"+QObject::tr("Prefix to specify that port is language-specific");
  opt << "PKGNAMESUFFIX:::text:::"+QObject::tr("Suffix to specify compilation options or a version designator");
  opt << "DISTVERSION:::text:::"+QObject::tr("Vendor version of the distribution (if different than PORTVERSION)");
  opt << "DISTNAME:::text:::"+QString(QObject::tr("Name of  port or distribution used in generating DISTFILES (default: %1)")).arg("<PORTNAME>-<DISTVERSIONPREFIX><DISTVERSION><DISTVERSIONSUFFIX>");
  opt << "CATEGORIES:::cats:::"+QObject::tr("FreeBSD categories to put the port in");
  opt << "DISTFILES:::textlist:::"+QObject::tr("Name(s) of the distribution source file(s) to use");
  opt << "EXTRACT_SUFX:::text:::"+QString(QObject::tr("Alternate source suffix if not the standard %1")).arg("\"*.tar.gz\"");
  opt << "MASTER_SITES:::url:::"+QObject::tr("URL for the master location(s) to look for the application source");
  opt << "MASTER_SITE_SUBDIR:::text:::"+QObject::tr("Master Site subdirectory to look for application source");
  opt << "PATCHFILES:::textlist:::"+QObject::tr("Names of additional patch files to use (within PATCH_SITES)");
  opt << "PATCH_SITES:::url:::"+QObject::tr("URL(s) to search for patch files");
  opt << "DIST_SUBDIR:::text:::"+QObject::tr("Subdirectory within DISTDIR to put distribution files and patches");
  opt << "ALLFILES:::textlist:::"+QObject::tr("List all distribution and patch files (optional)");
  opt << "IGNOREFILES:::textlist:::"+QObject::tr("Don't perform checksum checks on specific files");
  opt << "NOFETCHFILES:::textlist:::"+QObject::tr("Only download these files from MASTER_SITE_OVERRIDE");
  opt << "EXTRACT_ONLY:::textlist:::"+QObject::tr("Subset of DISTFILES to actually extract (optional)");
  opt << "ALWAYS_KEEP_DISTFILES:::textlist:::"+QObject::tr("Particular DISTFILES to save with the packages (optional)");
  opt << "MAINTAINER:::text:::"+QObject::tr("Port maintainer email address (usually yours if you are creating it)");
  opt << "COMMENT:::text:::"+QObject::tr("Short description of the application");
  opt << "LICENSE:::text:::"+QObject::tr("Application release license");
  //Restriction flags (give the reason why)
  opt << "RESTRICTED:::text:::"+QObject::tr("Prevent distribution on FTP or CDROM");
  opt << "NO_CDROM:::text:::"+QObject::tr("Prevent distribution on CDROM");
  opt << "NO_PACKAGE:::text:::"+QObject::tr("Prevent distribution as package");
  opt << "FORBIDDEN:::text:::"+QObject::tr("Package should not be built due to security vulnerabilities");
  opt << "LEGAL_TEXT:::text:::"+QObject::tr("List any legal issues with the port");
  opt << "LEGAL_PACKAGE:::text:::"+QObject::tr("No legal issues except it cannot be distributed as a package");
  opt << "IGNORE:::text:::"+QObject::tr("Package build should be skipped entirely");
  opt << "BROKEN:::text:::"+QObject::tr("Port is thought to be broken");
  opt << "DEPRECIATED:::text:::"+QObject::tr("Port is scheduled for removal");
  opt << "EXPIRATION_DATE:::text:::"+QObject::tr("Date when port will be removed (YYYY-MM-DD)");
  opt << "DISABLE_VULNERABILITIES:::text:::"+QObject::tr("Don't check if the port is listed in the vulnerabilities database (optional)");
  opt << "RESTRICTED_FILES:::textlist:::"+QObject::tr("List of files that cannot be redistributed");
  //Boolian values
  opt << "IS_INTERACTIVE:::bool:::"+QObject::tr("Set this if the port needs to interact with the user during a package build");
  opt << "USE_SUBMAKE:::bool:::"+QObject::tr("Set this if each of the make targets needs to be in a separate process");
  opt << "NO_ARCH:::bool:::"+QObject::tr("Set this if the port is architecture neutral");
  //Architecture restrictions
  opt << "ONLY_FOR_ARCHS:::textlist:::"+QObject::tr("Only build port if current architecture is listed here");
  opt << "NOT_FOR_ARCHS:::textlist:::"+QObject::tr("Only build port if current architecture is not listed here");
  opt << "ONLY_FOR_ARCHS_REASON:::text:::"+QObject::tr("Reason why it is only for particular architectures");
  opt << "NOT_FOR_ARCHS_REASON:::text:::"+QObject::tr("Reason why it is not for particular architectures");
  opt << "IA32_BINARY_PORT:::bool:::"+QObject::tr("Set this if the port fetches/installs compiled i386 binaries");
  //Dependecies
  opt << "EXTRACT_DEPENDS:::portfile:::"+QObject::tr("List of dependencies for the extract stage of the port");
  opt << "PATCH_DEPENDS:::portfile:::"+QObject::tr("List of dependencies for the patching stage of the port");
  opt << "FETCH_DEPENDS:::portfile:::"+QObject::tr("List of dependencies for the download stage of the port");
  opt << "BUILD_DEPENDS:::portfile:::"+QObject::tr("List of dependencies for the build stage of the port");
  opt << "RUN_DEPENDS:::portbin:::"+QObject::tr("List of dependencies to actually run the application");
  opt << "LIB_DEPENDS:::portlib:::"+QObject::tr("List of libraries the application requires to run");
  //Port flags
  opt << "USE_BZIP2:::bool:::"+QObject::tr("The distfiles use bzip2 compression");
  opt << "USE_LHA:::bool:::"+QObject::tr("The distfiles use lha for compression");
  opt << "USE_XZ:::bool:::"+QObject::tr("The distfiles use xz or lzma for compression");
  opt << "USE_ZIP:::bool:::"+QObject::tr("The distfiles use zip for compression");
  opt << "USE_MAKESELF:::bool:::"+QObject::tr("The distfiles use makeself for compression");
  opt << "USE_GCC:::bool:::"+QObject::tr("Set the port to require gcc");
  opt << "USE_CSTD:::text:::"+QObject::tr("Override the default C language standard");
  opt << "USE_CXXSTD:::text:::"+QObject::tr("Override the default C++ language standard");
  opt << "USE_GHOSTSCRIPT:::text:::"+QObject::tr("Require ghostscript to both build and run this port");
  opt << "USE_GHOSTSCRIPT_BUILD:::text:::"+QObject::tr("Require ghostscript to build this port");
  opt << "USE_GHOSTSCRIPT_RUN:::text:::"+QObject::tr("Require ghostscript to run this port");
  opt << "USE_GL:::textlist:::"+QObject::tr("List of Mesa or GL dependencies for the port");
  opt << "USE_SDL:::bool:::"+QObject::tr("Set that the port uses sdl libraries");
  opt << "USE_OPENSSL:::bool:::"+QObject::tr("Set that the port requires OpenSSL");
  opt << "USE_OPENLDAP:::bool:::"+QObject::tr("Set that the port uses OpenLDAP libraries");
  opt << "USE_AUTOTOOLS:::bool:::"+QObject::tr("Set that the port uses various GNU autotools");
  opt << "USE_SCONS:::bool:::"+QObject::tr("Set that the port uses the Python-based SCons build system");
  opt << "USE_EFL:::bool:::"+QObject::tr("Set that the port uses EFL libraries");
  opt << "USE_FPC:::bool:::"+QObject::tr("Set that the port uses the Free Pascal language");
  opt << "USE_JAVA:::bool:::"+QObject::tr("Set that the port uses the Java language");
  opt << "USE_OCAML:::bool:::"+QObject::tr("Set that the port uses the OCaml language");
  opt << "USE_PYTHON:::bool:::"+QObject::tr("Set that the port uses the Python language");
  opt << "USE_RUBY:::bool:::"+QObject::tr("Set that the port uses the Ruby language");
  opt << "USE_GNUSTEP:::bool:::"+QObject::tr("Set that the port uses the GNUstep system");
  opt << "USE_GECKO:::bool:::"+QObject::tr("Set that the port uses the Gecko/Mozilla product");
  opt << "USE_GNOME:::textlist:::"+QObject::tr("List of the Gnome dependencies this port needs");
  opt << "USE_MATE:::textlist:::"+QObject::tr("List of the MATE dependencies this port needs");
  opt << "USE_LUA:::bool:::"+QObject::tr("Set that the port uses the Lua libraries and components");
  opt << "USE_WX:::bool:::"+QObject::tr("Set that the port uses the WxWidgets library and components");
  opt << "USE_KDE4:::textlist:::"+QObject::tr("List of the KDE4 dependencies this port needs");
  opt << "USE_QT4:::textlist:::"+QObject::tr("List of the Qt4 dependencies this port needs");
  opt << "USE_LINUX:::text:::"+QObject::tr("This port uses either the default or a specific linux base");
  opt << "USE_LINUX_RPM:::bool:::"+QObject::tr("Set that the port pulls in variables/targets useful to Linux RPM ports");
  opt << "AUTOMATIC_PLIST:::bool:::"+QObject::tr("Automatically generate the packing list if USE_LINUX_RPM is set");
  opt << "USE_XORG:::textlist:::"+QObject::tr("List of X.org module dependencies this port needs");
  opt << "USE_TEX:::textlist:::"+QObject::tr("List of TeX dependencies this port needs");
  opt << "USE_APACHE:::bool:::"+QObject::tr("Set that the port needs the apache webserver");
  //Conflict checking
  opt << "CONFLICTS:::textlist:::"+QObject::tr("List of package name patterns that the port conflicts with");
  opt << "CONFLICTS_BUILD:::bool:::"+QObject::tr("Check for conflicts before starting the build");
  opt << "CONFLICTS_INSTALL:::bool:::"+QObject::tr("Check for conflicts before starting the installation");
  //Working directories
  opt << "WRKDIRPREFIX:::text:::"+QObject::tr("Where to root the temporary working directory hierarchy");
  opt << "WRKDIR:::text:::"+QObject::tr("Temporary working directory override");
  opt << "WRKSRC:::text:::"+QObject::tr("Subdirectory of WRKDIR where the distribution actually unpacks to");
  opt << "WRKSRC_SUBDIR:::text:::"+QObject::tr("Subdirectory of WRKSRC where the distribution actually builds");
  opt << "NO_WRKSUBDIR:::bool:::"+QObject::tr("Assume port unpacks directly into WRKDIR");

  return opt;
}

QStringList PortUtils::getMakefileOptHints(QString opt){
  //Output format: <user text shown>:::<config text added>
	
  //NOTE: Keep these in the order they need to be shown!! (alphabetical usually)
  QStringList hints;
  if(opt == "MASTER_SITES"){
    hints << "Apache Jakarta:::APACHE_JAKARTA";
    hints << "BerliOS:::BERLIOS";
    hints << "CheeseShop:::CHEESESHOP";
    hints << "Debian:::DEBIAN";
    hints << "GCC Project:::GCC";
    hints << "GNOME Project:::GNOME";
    hints << "GNU Project:::GNU";
    hints << "Mozilla Project:::MOZDEV";
    hints << "Perl (CPAN):::PERL_CPAN";
    hints << "Python Project:::PYTHON";
    hints << "RubyForge:::RUBYFORGE";
    hints << "Savannah:::SAVANNAH";
    hints << "SourceForge:::SF";
  }else if(opt == "DISTFILES"){
    hints << "PortName:::${PORTNAME}";
    hints << "PortVersion:::${PORTVERSION}";
  }else if(opt.startsWith("USE_GHOSTSCRIPT")){
    hints << "Default Version:::yes"
    hints << "Version 7.x:::7";
    hints << "Version 8.x:::8";
    hints << "Version 9.x:::9";
  }else if(opt == "USE_GL"){
    hints << "egl:::egl";
    hints << "glesv2:::glesv2";
    hints << "glut:::glut";
    hints << "glu:::glu";
    hints << "glw:::glw";
    hints << "gl:::gl";
  }else if(opt == "USE_GNOME"){
    hints << "ESound::esound"
    hints << "Common Makefile Substitutions:::gnomehack";
    hints << "Hierarchy:::gnomehier";
    hints << "MIME Data:::gnomemimedata";
    hints << "Prefix:::gnomeprefix";
    hints << "International Fixes:::intlhack";
    hints << "International Tool:::intltool";
    hints << "Introspection:::introspection";
    hints << "Reference Fixes:::referencehack";
    hints << "PKG Config:::pkgconfig";
    hints << "Desktop:::gnomedesktop";
    hints << "Panel:::gnomepanel";
    hints << "Speech:::gnomespeech";
    hints << "Menus:::gnomemenus";
  }else if(opt == "USE_MATE"){
    hints << "Auto Generate:::autogen";
    hints << "International Fixes:::intlhack";
    hints << "International Tools:::intltool";  
    hints << "Caja:::caja";
    hints << "Common:::common";
    hints << "Control Center:::controlcenter";
    hints << "Desktop:::desktop";
    hints << "Dialogs:::dialogs";
    hints << "Documentation Utils:::docutils";
    hints << "Icons:::icontheme";
    hints << "Keyring:::keyring";
    hints << "KBD Library:::libmatekbd";
    hints << "Keyring Library:::libmatekeyring";
    hints << "Weather Library:::libmateweather";
    hints << "WNCK Library:::libmatewnck";
    hints << "Marco:::marco";
    hints << "Menus:::menus";
    hints << "Mu Character Map:::mucharmap";
    hints << "Notification Daemon:::notificationdaemon";
    hints << "Panel:::panel";
    hints << "PolicyKit:::polkit";
    hints << "Pluma:::pluma";
    hints << "Settings Daemon:::settingsdaemon";
  }else if(opt == "USE_KDE4"){
    hints << "All:::all";
    hints << "KDE Hierarchy:::kdehier";
    hints << "KDE Libraries:::kdelibs";
    hints << "KDE Prefix:::kdeprefix";
    hints << "Base Apps:::baseapps";
    hints << "Activities Library:::kactivities";
    hints << "Kate:::kate";
    hints << "Ruby Bindings:::korundum";
    hints << "CDDB Library:::libkcddb";
    hints << "Audio CD Library:::libkcompactdisk";
    hints << "LibRaw Library:::libkdcraw";
    hints << "Educational Libraries:::libkdeedu";
    hints << "Games Libraries:::libkdegames";
    hints << "Exiv2 Library:::libkexiv2";
    hints << "Image Plugin Interface:::libkipi";
    hints << "Konqueror Libraries:::libkonq";
    hints << "SANE Library:::libksane";
    hints << "Marble:::marble";
    hints << "Okular:::okular";
    hints << "Nepomuk Core Libraries:::nepomuk-core";
    hints << "Nepomuk Widgets Libraries:::nepomuk-widgets";
    hints << "Oxygen Icon Theme:::oxygen";
    hints << "Perl Bindings:::perlkde";
    hints << "PIM Libraries:::pimlibs";
    hints << "Python Bindings:::pykde4";
    hints << "Python User Interface Compiler:::pykdeuic4";
    hints << "Ruby Bindings:::qtruby";
    hints << "Runtime Environment:::runtime";
    hints << "MIME Types Database:::sharedmime";
    hints << "SMOKE Base Libraries:::smokegen";
    hints << "SMOKE Libraries:::smokekde";
    hints << "SMOKE Libraries (Qt-only):::smokeqt";
    hints << "Workspace:::workspace";
    hints << "Akonadi:::akonadi";
    hints << "Attica:::attica";
    hints << "Automatic moc:::automoc4";
    hints << "Ontologies:::ontologies";
    hints << "Graphical Effects/Filters Library:::qimageblitz";
    hints << "Soprano:::soprano";
    hints << "Desktop Search Daemon:::strigi";
  }else if(opt == "USE_QT4"){
    hints << "Core Library:::corelib";
    hints << "GUI Library:::gui";
    hints << "Network Library:::network";
    hints << "OpenGL Library:::opengl";
    hints << "Qt3 Compatibility:::qt3support";
    hints << "Unit Testing Library:::qtestlib";
    hints << "Script Library:::script";
    hints << "SQL Library:::sql";
    hints << "XML Library:::xml";
    hints << "QMake:::qmake";
    hints << "Meta Object Compiler:::moc";
    hints << "Resource Compiler:::rcc";
    hints << "User Interface Compiler:::uic";
    hints << "SVG Icon Engine:::iconengines";
    hints << "Image File Formats:::imageformats";
  }else if(opt == "USE_XORG"){
    hints << "bigreqsproto:::bigreqsproto";
    hints << "compositeproto:::compositeproto";
    hints << "damageproto:::damageproto";
    hints << "dmx:::dmx";
    hints << "dmxproto:::dmxproto";
    hints << "dri2proto:::dri2proto";
    hints << "dri3proto:::dri3proto";
    hints << "evieproto:::evieproto";
    hints << "fixesproto:::fixesproto";
    hints << "fontcacheproto:::fontcacheproto";
    hints << "fontenc:::fontenc";
    hints << "fontsproto:::fontsproto";
    hints << "fontutil:::fontutil";
    hints << "glproto:::glproto";
    hints << "ice:::ice";
    hints << "inputproto:::inputproto";
    hints << "kbproto:::kbproto";
    hints << "libfs:::libfs";
    hints << "oldx:::oldx";
    hints << "pciaccess:::pciaccess";
    hints << "pixman:::pixman";
    hints << "presentproto:::presentproto";
    hints << "printproto:::printproto";
    hints << "randrproto:::randrproto";
    hints << "recordproto:::recordproto";
    hints << "renderproto:::renderproto";
    hints << "resourceproto:::resourceproto";
    hints << "scrnsaverproto:::scrnsaverproto";
    hints << "sm:::sm";
    hints << "trapproto:::trapproto";
    hints << "videoproto:::videoproto";
    hints << "x11:::x11";
    hints << "xau:::xau";
    hints << "xaw:::xaw";
    hints << "xaw6:::xaw6";
    hints << "xaw7:::xaw7";
    hints << "xbitmaps:::xbitmaps";
    hints << "xcb:::xcb";
    hints << "xcmiscproto:::xcmiscproto";
    hints << "xcomposite:::xcomposite";
    hints << "xcursor:::xcursor";
    hints << "xdamage:::xdamage";
    hints << "xdmcp:::xdmcp";
    hints << "xevie:::xevie";
    hints << "xext:::xext";
    hints << "xextproto:::xextproto";
    hints << "xf86bigfontproto:::xf86bigfontproto";
    hints << "xf86dgaproto:::xf86dgaproto";
    hints << "xf86driproto:::xf86driproto";
    hints << "xf86miscproto:::xf86miscproto";
    hints << "xf86rushproto:::xf86rushproto";
    hints << "xf86vidmodeproto:::xf86vidmodeproto";
    hints << "xfixes:::xfixes";
    hints << "xfont:::xfont";
    hints << "xfontcache:::xfontcache";
    hints << "xft:::xft";
    hints << "xi:::xi";
    hints << "xinerama:::xinerama";
    hints << "xineramaproto:::xineramaproto";
    hints << "xkbfile:::xkbfile";
    hints << "xkbui:::xkbui";
    hints << "xmu:::xmu";
    hints << "xmuu:::xmuu";
    hints << "xorg-macros:::xorg-macros";
    hints << "xorg-server:::xorg-server";
    hints << "xp:::xp";
    hints << "xpm:::xpm";
    hints << "xprintapputil:::xprintapputil";
    hints << "xprintutil:::xprintutil";
    hints << "xproto:::xproto";
    hints << "xproxymngproto:::xproxymngproto";
    hints << "xrandr:::xrandr";
    hints << "xrender:::xrender";
    hints << "xres:::xres";
    hints << "xscrnsaver:::xscrnsaver";
    hints << "xshmfence:::xshmfence";
    hints << "xt:::xt";
    hints << "xtrans:::xtrans";
    hints << "xtrap:::xtrap";
    hints << "xtst:::xtst";
    hints << "xv:::xv";
    hints << "xvmc:::xvmc";
    hints << "xxf86dga:::xxf86dga";
    hints << "xxf86misc:::xxf86misc";
    hints << "xxf86vm:::xxf86vm";
  }else if(opt=="USE_TEX"){
    hints << "Full (teTeX or TeXLive):::full";
    hints << "Use teTeX:::tetex";
    hints << "Use TeXLive:::texlive";
    hints << "Base:::base";
    hints << "TeXmf Tree:::texmf";
    hints << "Documentation:::docs";
    hints << "WEB2C (TeXLive Only):::web2c";
    hints << "kpathsea Library (TexLive Only):::kpathsea";
    hints << "pTeX Conversion Library (TexLive Only):::ptexenc";
    hints << "tlmgr Dependency (TexLive Only):::infra";
    hints << "dvipsk:::dvipsk";
    hints << "DVIPDFMx:::dvipdfmx";
    hints << "XDvi:::xdvik";
    hints << "All Formats (TexLive Only):::formats";
    hints << "TeX Format:::tex";
    hints << "LaTeX Format:::latex";
    hints << "PDFTeX Format:::pdftex";
    hints << "Aleph Format (TexLive Only):::aleph";
    hints << "JadeTeX Format:::jadetex";
    hints << "LuaTeX Format (TexLive Only):::luatex";
    hints << "pTeX Format:::ptex";
    hints << "XeTeX Format (TexLive Only):::xetex";
    hints << "XMLTeX Format:::xmltex";
    hints << "Directory Search Hash Regeneration:::texhash";
    hints << "Font Map Regeneration (TeXLive Only):::updmap";
    hints << "Format Regeneration (TeXLive Only):::fmtutil";
  }
  
  return hints;
}

// ================
//    CONFIGURATIONS
// ================
QStringList PortUtils::generateNewMakefile(QString name, QStringList vars, QStringList vals){
  //QuickPorting requires: PORTNAME (as "name" input), PORTVERSION, CATEGORIES, MASTER_SITES, MAINTAINER, COMMENT
  if(name.isEmpty() || (vars.length() != vals.length()) ){ return QStringList(); } //invalid inputs
  //Get the ordering of the config options
  QStringList opts = PortUtils::getMakefileConfigOpts();
  //Now generate the template
  QStringList out;
  out << "# Port template generated by EasyPort version 1.0";
  out << "# $FreeBSD$";
  out << "";
  out << "# General Port information #";
  out << "PORTNAME=\t"+name;
  //Place the input options in the proper order
  for(int i=0; i<opts.length(); i++){
    int index = vars.indexOf(opts[i].section(":::",0,0));
    if( index >= 0 ){
	out << vars[index]+"=\t"+vals[index];
    }
  }
  out << "";
  out << ".include <bsd.port.mk>";

  return out;
}

QStringList PortUtils::generatePkgDescr(QString description, QString website, QString author){
  QStringList contents;
  contents << description;
  contents << "";
  if(!website.isEmpty()){ contents << "WWW: "+website; }
  if(!author.isEmpty()){ contents << "AUTHOR: "+author; }
  return contents;
}

QStringList PortUtils::insertMakeFileConfig(QStringList current, QString var, QString val, bool replace){
  //Get the valid order of the options in the Makefile
  QStringList opts = PortUtils::getMakefileConfigOpts();
  int CI= -1;
  for(int i=0; i<opts.length(); i++){
    opts[i] = opts[i].section(":::",0,0); //trim off the extra info
    if(opts[i] == var ){ CI = i; }
  }
  if(CI < 0){ return current; } //Invalid makefile option - return the current file contents
  //Now go through the file contents and insert the option in the appropriate spot
  for(int i=0; i<current.length(); i++){
    if(current[i].isEmpty() || current[i].startsWith("#") ){ continue; } //skip this line
    else if(current[i].contains("=")){
      QString cvar = current[i].section("=",0,0).remove("?").simplified();
      int TI = opts.indexOf(cvar); //this index
      if(TI < CI){ 
	continue; //not far enough in the file yet
      }else if(TI == CI){ //This config already exists
	if(replace){
	  //Overwrite the current line and remove any "additional" lines for the same variable
	  int dI = i;
	  while(current[dI].endsWith("\\")){ dI++; }
	  while(dI > i){
	    current.removeAt(dI);
	    dI--;
	  }
	  current[i] = var+"=\t"+val;
	}else{
	  //Just append the value to the current item
	  int II = i;
	  while(current[II].endsWith("\\")){ II++; } //move to the appropriate line
	  if( II > i || var.contains("_DEPENDS") || current[II].length() > 60){ //put it on the next line
	    current.insert(II+1, val);
	    current[II].append(" \\");
	  }else{  //just append it to the current line
	    current[II].append(" "+val);
	  }
	}
	break; //done
      }else{ //TI > CI
	//insert the new config option right before this line
	current.insert(i, var+"=\t"+val);
	break; //done
      }
    }else if( ( current[i].contains(":") && (current[i].startsWith("pre-") || current[i].startsWith("post-") || current[i].startsWith("do-")) ) || current[i].contains(".include <") ){
      //Ran out of config section in the file - just add it to the end of the config section
      current.insert(i, var+"=\t"+val);
      current.insert(i+1,"");
    }
  }
  return current;
}

// ======================
//   GENERAL UTILITIES
// ======================
bool PortUtils::runCmd(QString indir, QString cmd, QStringList args){
  QProcess proc;
  if(!indir.isEmpty() && QFile::exists(indir) ){
    proc.setWorkingDirectory(indir);
  }
  if(args.isEmpty()){ proc.start(cmd); }
  else{ proc.start(cmd,args); }
  while( !proc.waitForFinished(300) ){
    QCoreApplication::processEvents(); //make sure we don't hang the GUI	  
  }
  return (proc.exitCode() == 0);
}

void PortUtils::compressDir(QString dirPath){
  //Compress the directory
  QString localDir = dirPath.section("/",-1);
  //QString cmd = "cd "+dirPath+"/..; tar czf "+localDir+".tar.gz "+localDir;
  QStringList args; args << "czf" << localDir+".tar.gz" << localDir;
  qDebug() << "Compressing dir:" << dirPath+".tar.gz";
  dirPath.chop(localDir.length()); //go up one directory
  PortUtils::runCmd(dirPath, "tar", args);
  return;
}

bool PortUtils::createFile(QString fileName, QStringList contents){
//fileName = full path to file (I.E. /home/pcbsd/junk/junk.txt)
//contents = list of lines to be written (one line per entry in the list - no newline needed at the end of an entry)
	
  //Check if this is actually a file removal
  if(contents.isEmpty() && QFile::exists(fileName)){
    bool good = QFile::remove(fileName);
    if(!good){
      qDebug() << fileName+": Could not be deleted";
    }
    return good;
  }else if(contents.isEmpty()){
    return true; //File to delete already does not exist 
  }
  
  //Check that the parent directory exists, and create it if necessary
  QString pDir = fileName;
  pDir.chop( fileName.section("/",-1).length() );
  QDir dir(pDir);
    if(!dir.exists()){ 
      qDebug() << "Create parent Directory:" << dir.absolutePath();
      if( !dir.mkpath(dir.absolutePath()) ){
        qDebug() << fileName+": Could not create containing directory";
	return false;
      }
    }

  //Open the file with .tmp extension
  QFile file(fileName+".tmp");
  if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ){
    qDebug() << fileName+".tmp: Failure -- Could not open file";
    return false;
  }
  //Write the file
  QTextStream ofile(&file); //start the output stream
  for(int i=0; i<contents.length(); i++){
    ofile << contents[i];
    ofile << "\n";
  }
  //Close the File
  file.close();
  //Remove any existing file with the final name/location
  if( QFile::exists(fileName) ){
    if( !QFile::remove(fileName) ){
      qDebug() << fileName+": Error -- Could not overwrite existing file";
      QFile::remove(fileName+".tmp");
      return false;
    }
  }
  //Move the temporary file into its final location
  if( !file.rename(fileName) ){
    qDebug() << fileName+": Error: Could not rename "+fileName+".tmp as "+fileName;
    return false;
  }
  //Return success
  QString extra = QDir::homePath(); //remove this from the filename display
  qDebug() << "Saved:" << fileName.replace(extra,"~");
  return true;;
}

bool PortUtils::removeFile(QString fileName){
  return PortUtils::createFile(fileName, QStringList() );
}

QStringList PortUtils::readFile(QString filePath){
  QStringList contents;
  //Check that the file exists first
  if(!QFile::exists(filePath)){ 
    qDebug() << "Warning: file to read does not exist:" << filePath;
    return contents; 
  }
  //Open the file for reading
  QFile file(filePath);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ 
    qDebug() << "Error: file could not be opened for reading:"<< filePath;
    return contents; 
  }
  QTextStream in(&file);
  //Save the contents as a QStringList
  while(!in.atEnd()){
    contents << in.readLine();
  }
  //Return the contents (one entry per line)
  return contents;
}


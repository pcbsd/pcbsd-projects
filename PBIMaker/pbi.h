#ifndef PBI_HEADER_FILE__
#define PBI_HEADER_FILE__

typedef struct
{
   char *path;
   void *next;
} BIN_FILE;

typedef struct
{
   char *author_name;
   char *web_site;
   char *port_name, *port_name9, *port_name8, *port_dir;
   char **new_files;
   char *icon_name, *main_exe;
   BIN_FILE *more_exe;
   char *backup_icon;
   char *category;
   char *exclude;
   char *dir_owner;
   char *port_prefix;
   FILE *log;
   int logging;
   int interactive;
   int console_app;
   int build_module_eight;
   int build_module_nine;
   int build_type;
   int skip_build;
} PBI_Struct;

#define MAKEAPBI_VERSION "1.3"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define MAX_FILES 1024

#define LOGGING_SILENT 0
#define LOGGING_STDOUT 1
#define LOGGING_FILE 2

#define BUILD_NONE 1
#define BUILD_PKG 2
#define BUILD_MAKE 3

#ifndef PREFIX
#define PREFIX "/usr/local"
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// set up the data structure
PBI_Struct *Init_Data();

// clean up the data structure
void Clean_Up(PBI_Struct *data, int quietly);

// Check for and handle command-line options
int Get_Options(PBI_Struct *data, int args, char *params[]);


// checks to see if a port exists
int Port_Exists(PBI_Struct *data);

// see if we are the root user
int Check_Root(PBI_Struct *data);

// Figure out the name of the module based on its path
int Get_Name(PBI_Struct *data);

// create the folder layout for a module -- version 8
int Create_Module_Template(PBI_Struct *data);

// create the folder layout for a module -- version 9
int Create_Module_Template9(PBI_Struct *data);

// make sure we are building the port in batch mode
int Confirm_Batch_Mode(PBI_Struct *data);

// create a file so we can keep track of the current time
int Create_Time_File(PBI_Struct *data);

// actually build the port. Make sure we do not get an error
int Make_Port(PBI_Struct *data);

// attempt to install port
int Install_Port(PBI_Struct *data);

// find all finds which may have been installed by the port
int Find_Port_Files(PBI_Struct *data);

// try to figure out the port's website
int Get_Website(PBI_Struct *data);

// try to figure out the port's author
int Get_Author(PBI_Struct *data);

// try to guess the port's icon
int Get_Icon(PBI_Struct *data);

// try to guess what category this port belongs to
int Get_Category(PBI_Struct *data);

// write the pbi.conf file -- version 8
int Write_Conf(PBI_Struct *data);

// write the pbi.conf file -- version 9
int Write_Conf9(PBI_Struct *data);


// write the copyfiles file to the module
int Write_Copyfiles(PBI_Struct *data);

// create the kmenu entry file
int Write_Kmenu(PBI_Struct *data);

// write menu/icon files for version 9
int Write_Menu(PBI_Struct *data, char *path);

// Remove batch mode setting
int Restore_Make_Config(PBI_Struct *data, int quietly);

// copy icon from system to overlay directory -- version 8
int Copy_Icon(PBI_Struct *data);

// copy icon from system to overlay directory -- version 9
int Copy_Icon9(PBI_Struct *data);

// get rid of trailing newline character
void Trim_Newline(char *line);

// try to guess which file is the executable
int Get_Main_Exe(PBI_Struct *data);

// try to guess which file is the executable the smart way,
// using the port's information
int Get_Main_Exe_Smart(PBI_Struct *data);

// search for additional bin/* files using pkg-plist file
int Get_More_Exe(PBI_Struct *data);

// copy the program's executable to the overlay-dir -- version 8
int Copy_Main_Exe(PBI_Struct *data);

// perform a chown to assign the module to a user
void Chown_Module(PBI_Struct *data);

// perform a chown to assign the module to a user -- version 9
void Chown_Module9(PBI_Struct *data);

// Tries to guess whether the selected port is
// a GUI app or a console app. Returns TRUE
// for GUI and FALSE for console.
int Is_GUI(PBI_Struct *data);

#endif


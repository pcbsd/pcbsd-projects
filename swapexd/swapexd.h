#ifndef SWAPEXD_HEADER_FILE__
#define SWAPEXD_HEADER_FILE__

#ifndef VERSION
#define VERSION "0.3"
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


#define GROW_BUFFER_DEFAULT 256
#define REDUCE_BUFFER_DEFAULT 1024
#define DELTA_SIZE_DEFAULT 256
#define SWAP_FILE_DEFAULT "/usr/swap/swap0"
#define CONFIG_FILE_DEFAULT "/etc/swapexd.conf"
#define WAIT_TIME_DEFAULT 5

#define ACTION_GROW 1
#define ACTION_SHRINK 2


typedef struct
{
  int daemon_mode;  // true or false
  int show_help;    // true or false
  int show_version;  // true or false
  char *config_filename;
  int grow_buffer;  // grow swap when we have this much left (in MB)
  int reduce_buffer; // shrink swap when we have this much free (in MB)
  int delta_size;   // size by which to grow or shrink swap
  int current_swap_file_size;   // in MB
  char *path_to_swap_file;  // filename of swap file
  int swap_filesize; // in megabytes
  int last_md_number;  // last device number we used
  int wait_time;    // time between polling in seconds
  int create_pid_file;    // should we make a pid file in /var/run/ ?
} SWAP_DATA;



// Returns pointer to structure on success or NULL on failure.
SWAP_DATA *Init_Swap_Data();

// This file cleans up the swap files and memory devices
void Clean_Up_Swap_File(SWAP_DATA *data);


// This function removes our internal data structures
void Clean_Up_Swap_Data(SWAP_DATA *data);


int Get_Command_Line_Arguments(SWAP_DATA *data, int num_args, char *args[]);

void Print_Usage();

void Print_Version();

int Read_Config_File(SWAP_DATA *data);

int Create_Swap_Directory(SWAP_DATA *data);

int Get_Total_Swap_Size();

int Get_Swap_Usage();

int Create_Swap_File(SWAP_DATA *data);


// Create memory device for a given swap file. Return
// the number of the new memory device.
// Returns -1 on error
int Link_Swap_File(SWAP_DATA *data);


// This function does the heavy work of growing or shrinking
// the swap file. Action should be either ACTION_GROW or ACTION_SHRINK
int Resize_Swap_File(SWAP_DATA *data, int action);

int Create_PID_File();

#endif


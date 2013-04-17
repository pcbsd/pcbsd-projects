/*
Description:

To compile: Please run the accompanying makefile.

License: This program is made available under the BSD licnese. Please
	 see the accompanying LICENSE file for details.

Author: Jesse Smith <jessefrgsmith@yahoo.ca>

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "swapexd.h"

int keep_running = TRUE;


void handler_sigterm(int signum)
{
   signal(SIGTERM, handler_sigterm);
   signal(SIGINT, handler_sigterm);
   keep_running = FALSE;
   #ifdef DEBUG
   printf("Caught termination signal.\n");
   #endif
}


int main(int argc, char *argv[])
{
  SWAP_DATA *swap_data;
  int status;
  int total_swap_size, swap_usage, swap_available;
  pid_t process_id;

  // create data structure
  swap_data = Init_Swap_Data();
  if (! swap_data)
  {
     printf("Unable to allocate memory. Exiting.\n");
     return 1;
  }

  // check command line arguments
  Get_Command_Line_Arguments(swap_data, argc, argv);

  // see if we should display version
  if (swap_data->show_version)
     Print_Version();
  // see if we should display help
  if (swap_data->show_help)
     Print_Usage();

  if ( (swap_data->show_version) || (swap_data->show_help) )
  {
      Clean_Up_Swap_Data(swap_data);
      exit(0);
  }
 
  // give us random numbers
  srand( time(NULL) ); 

  // try to load config file
  Read_Config_File(swap_data);

  // see if we should run as a daemon
  if (swap_data->daemon_mode)
  {
      #ifdef DEBUG
      printf("Forking\n");
      #endif
      process_id = fork();
      // parent
      if (! process_id)
      {
         Clean_Up_Swap_Data(swap_data);
         exit(0);
      }
      else  // child process
      {
         setsid();
         process_id = fork();
         if (process_id)
         {
           Clean_Up_Swap_Data(swap_data);
           exit(0);
         }
      }
  }     // end of using daemon mode

  // check to see if we need to create a PID file
  if (swap_data->create_pid_file)
     Create_PID_File();

  Create_Swap_Directory(swap_data);
  // check to see if old swap file exists and, if so, remove it

  // register signal handler
  signal(SIGTERM, handler_sigterm);
  signal(SIGINT, handler_sigterm);
  while (keep_running)
  {
  // get current amount of swap space
     total_swap_size = Get_Total_Swap_Size();
  // get current swap usage
     swap_usage = Get_Swap_Usage();
  // cacluate available swap space remaining
     swap_available = total_swap_size - swap_usage;
     #ifdef DEBUG
     printf("Total swap: %d\n", total_swap_size);
     printf("Swap used:  %d\n", swap_usage);
     printf("Swap free:  %d\n", swap_available);
     #endif
  // if we are close to needing more swap, create/enlarge the swap file
     if (swap_data->grow_buffer > swap_available)
     {
        #ifdef DEBUG
        printf("Trying to grow swap file.\n");
        #endif
        Resize_Swap_File(swap_data, ACTION_GROW);
     } 
  // if we have plenty of swap space available, reduce the swap file 
     else if ( (swap_data->current_swap_file_size > 0) &&
               (swap_data->reduce_buffer < swap_available) )
     {
         #ifdef DEBUG
         printf("Tring to shrink swap file.\n");
         #endif
         Resize_Swap_File(swap_data, ACTION_SHRINK);
     }
  // wait before running another test
    sleep(5);
  }   // end of endless loop

  #ifdef DEBUG
  printf("Left main loop, cleaning up.\n");
  #endif

  // clean up
  Clean_Up_Swap_File(swap_data);
  Clean_Up_Swap_Data(swap_data);
  return 0;
}

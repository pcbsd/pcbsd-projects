#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "swapexd.h"


SWAP_DATA *Init_Swap_Data()
{
   SWAP_DATA *my_data;

   my_data = (SWAP_DATA *) calloc(1, sizeof(SWAP_DATA));
   if (! my_data)
     return NULL;

   my_data->config_filename = strdup(CONFIG_FILE_DEFAULT);
   my_data->grow_buffer = GROW_BUFFER_DEFAULT;
   my_data->reduce_buffer = REDUCE_BUFFER_DEFAULT;
   my_data->delta_size = DELTA_SIZE_DEFAULT;
   my_data->current_swap_file_size = 0;
   my_data->path_to_swap_file = strdup(SWAP_FILE_DEFAULT);
   my_data->last_md_number = -1;
   my_data->wait_time = WAIT_TIME_DEFAULT;
   my_data->create_pid_file = FALSE;
   return my_data;
}



void Clean_Up_Swap_File(SWAP_DATA *data)
{
   char cmd_line[1024];
   // unlink swap file
   if (data->path_to_swap_file)
      unlink(data->path_to_swap_file);
   // turn off memory device
   if (data->last_md_number >= 0)
   {
     snprintf(cmd_line, 1024, "swapoff /dev/md%d", data->last_md_number);
     system(cmd_line);
     // remove memory device
     snprintf(cmd_line, 1024, "mdconfig -d -u %d", data->last_md_number);
     system(cmd_line);
   }
}


void Clean_Up_Swap_Data(SWAP_DATA *data)
{
   if (data->config_filename)
     free(data->config_filename);
   if (data->path_to_swap_file)
     free(data->path_to_swap_file);
   free(data);
}



int Get_Command_Line_Arguments(SWAP_DATA *data, int num_args, char *args[])
{
   int index = 1;

   while (index < num_args)
   {
       // check each argument for something ew recognize
       if (! strcmp(args[index], "-h") )
          data->show_help = TRUE;
       else if (! strcmp(args[index], "-v") )
          data->show_version = TRUE;
       else if ( (! strcmp(args[index], "-c") ) && (index < num_args - 1) )
       {
          if (data->config_filename)
             free(data->config_filename);   // clear out the default
          data->config_filename = strdup(args[index+1]);
          index++;
          #ifdef DEBUG
          printf("Got new config file: %s\n", data->config_filename);
          #endif
       }
       else if ( (! strcmp(args[index], "-d") ) || 
                 (! strcmp(args[index], "-daemon") ) )
          data->daemon_mode = TRUE;
       else if (! strcmp(args[index], "-p") )
          data->create_pid_file = TRUE; 
       index++;
   }

   return TRUE;
}



void Print_Usage()
{
   printf("SwapExd\n");
   printf("-v\t\t\tprint version info and exit\n");
   printf("-h\t\t\tprint help message and exit\n");
   printf("-d\t\t\trun in background (daemon mode)\n");
   printf("-daemon\t\t\trun in background(daemon mode)\n");
   printf("-c <config_file> \tuse alternative config file\n");
   printf("\n");
}


void Print_Version()
{
  printf("SwapExd -- version %s\n\n", VERSION);
}




int Read_Config_File(SWAP_DATA *data)
{
   FILE *config_file;
   char line[256];
   char *status;

   if (! data->config_filename)
      return FALSE;

   #ifdef DEBUG
   printf("Trying to open config file %s\n", data->config_filename);
   #endif
   config_file = fopen(data->config_filename, "r");
   if (! config_file)
       return FALSE;

   // opened config file, read each line
   status = fgets(line, 256, config_file);
   while (status)
   {
       status = strchr(line, '\n');
       if (status)
          status[0] = '\0';   // clear newline
       #ifdef DEBUG
       printf("Read %s from config file.\n", line);
       #endif
         
       if (! strncmp(line, "SWAP_FILE", 9) )
       {
          // remove old swap file path
          if (data->path_to_swap_file)
              free(data->path_to_swap_file);
          data->path_to_swap_file = strdup( &(line[10]) );
       }
       else if (! strncmp(line, "GROW_BUFFER", 11) )
          sscanf( &(line[12]), "%d", &(data->grow_buffer) );
       else if (! strncmp(line, "REDUCE_BUFFER", 13) )
          sscanf( &(line[14]), "%d", &(data->reduce_buffer) );
       else if (! strncmp(line, "DELTA_SIZE", 10) )
          sscanf( &(line[11]), "%d", &(data->delta_size) );
       else if (! strncmp(line, "WAIT_TIME", 9) )
          sscanf( &(line[10]), "%d", &(data->wait_time) );

       status = fgets(line, 256, config_file);
   }  // end of reading from file
   fclose(config_file);
   return (data->path_to_swap_file) ? TRUE : FALSE;
}


int Create_Swap_Directory(SWAP_DATA *data)
{
    char *dir_name;
    int index;
    int status;

    if (! data->path_to_swap_file)
       return FALSE;
    dir_name = strdup(data->path_to_swap_file); 
    if (! dir_name)
       return FALSE;

    // remove everything after trailing slash
    index = strlen(dir_name);
    index--;
    while (index >= 0)
    { 
       if (dir_name[index] == '/')
       {
          dir_name[index] = '\0';
          index = -1;
       }
       index--;
    }
    #ifdef DEBUG
    printf("Trying to create swap directory: %s\n", dir_name);
    #endif
    status = mkdir(dir_name, 0755);   
    if (!status)
       return TRUE;
    else
       return FALSE;
}




int Get_Total_Swap_Size()
{
   char output[512];
   FILE *my_process;
   int swap_size = 0;

   #ifdef DEBUG
   printf("Getting total swap size.\n");
   #endif
   my_process = popen("swapctl -s -m | awk '{ print $2 }'", "r"); 
   if (! my_process)
      return -1;

   fgets(output, 512, my_process);
   sscanf(output, "%d", &swap_size);
   
   pclose(my_process);
   return swap_size;  
}



int Get_Swap_Usage()
{
   FILE *my_process;
   char output[512];
   int usage_amount = 0;

   #ifdef DEBUG
   printf("Running usage data grab.\n");
   #endif
   my_process = popen("swapctl -s -m | awk '{ print $3 }'", "r"); 
   if (! my_process)
      return -1;

   fgets(output, 512, my_process);
   sscanf(output, "%d", &usage_amount);
   pclose(my_process);
   return usage_amount;
}



int Create_Swap_File(SWAP_DATA *data)
{
   char buffer[1024];   // 1k
   int wrote_kb, wrote_mb;
   FILE *new_swap_file;

   if (data->current_swap_file_size > 0)
   {
       new_swap_file = fopen(data->path_to_swap_file, "w");
       if (! new_swap_file)
          return FALSE;
       memset(buffer, 0, 1024); 
       for (wrote_mb = 0; wrote_mb < data->current_swap_file_size; wrote_mb++)
       {
         for (wrote_kb = 0; wrote_kb < 1024; wrote_kb++)
         {
            fwrite(buffer, sizeof(char), 1024, new_swap_file);
         }
       }   // end of writing MB
       fclose(new_swap_file);
       // set permissions
       chmod(data->path_to_swap_file, 0600);
   }     // end of we should create a swap file
   else
     return FALSE;
   return TRUE;
}



int Link_Swap_File(SWAP_DATA *data)
{
   int status = -1;
   char command_line[1024];
   int random_number;
   int tries = 10;

   while ( (status != 0) && (tries > 0) )
   {
     random_number = rand();
     snprintf(command_line, 1024, "mdconfig -a -t vnode -f %s -u %d",
              data->path_to_swap_file, random_number);
     status = system(command_line);
     tries--;
   }
   if (! status)
     return random_number;
   else
     return -1;
}




int Resize_Swap_File(SWAP_DATA *data, int action)
{
   char command_line[1024];
   int status = TRUE, device_number = -1;
   int old_swap_file_size = 0;
 
   // delete old swap file
   #ifdef DEBUG
   printf("Removing old swap file.\n");
   #endif
   if (data->path_to_swap_file)
      unlink(data->path_to_swap_file);

   // create new file of proper size
   #ifdef DEBUG
   printf("Old swap file was: %dMB\n", data->current_swap_file_size);
   #endif
   old_swap_file_size = data->current_swap_file_size;
   if (action == ACTION_GROW)
      data->current_swap_file_size += data->delta_size;
   else if (action == ACTION_SHRINK)
      data->current_swap_file_size -= data->delta_size;
   else
      return FALSE;     // guard against illegal operation
   if (data->current_swap_file_size < 0)
      data->current_swap_file_size = 0;
   #ifdef DEBUG
   printf("New swap file will be: %dMB\n", data->current_swap_file_size); 
   #endif
   // link in new swap file
   if (data->current_swap_file_size > 0)
   {
     #ifdef DEBUG
     printf("Creating new swap file.\n");
     #endif
     status = Create_Swap_File(data);
   }
   if (! status)
   {
      #ifdef DEBUG
      printf("Failed to create swap file. Aborting resize.\n");
      #endif
      // file creation failed, so swap size has not changed
      data->current_swap_file_size = old_swap_file_size;
      return FALSE;
   }
   // link in new swap file
   if (data->current_swap_file_size > 0)
   {
       #ifdef DEBUG
       printf("Linking in swap file.\n");
       #endif
       device_number = Link_Swap_File(data);
   }

     // activate new swap file
     if ( (data->current_swap_file_size > 0) && (device_number >= 0) )
     {
       #ifdef DEBUG
       printf("Activating new swap file.\n");
       #endif
       snprintf(command_line, 1024, "swapon /dev/md%d", device_number);
       system(command_line);
     }

     // Turn off old swap file if it exists
     if (data->last_md_number >= 0)
     {
       #ifdef DEBUG
       printf("Turning off old swap file: /dev/md%d\n", data->last_md_number);
       #endif
       // deactiveate old swap file
       snprintf(command_line, 1024, "swapoff /dev/md%d", data->last_md_number);
       system(command_line);
       // remove old swap file links
       #ifdef DEBUG
       printf("Removing old memory device %d\n", data->last_md_number);
       #endif
       snprintf(command_line, 1024, "mdconfig -d -u %d", data->last_md_number);
       system(command_line);
     }
     // record the number of the new memory device
     data->last_md_number = device_number;
   
   return TRUE;
}




int Create_PID_File()
{
  FILE *my_file;
  pid_t my_pid;

  my_file = fopen("/var/run/swapexd.pid", "w");
  if (my_file)
  {
     my_pid = getpid();
     fprintf(my_file, "%d", my_pid);
     fclose(my_file);
     return TRUE;
  }

  return FALSE;
}


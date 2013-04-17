#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include "pbi.h"



PBI_Struct *Init_Data()
{
   PBI_Struct *my_data;

   my_data = (PBI_Struct *) calloc(1, sizeof(PBI_Struct));
   if (! my_data)
       return NULL;

   my_data->logging = LOGGING_STDOUT;
   my_data->log = stdout;
   my_data->console_app = FALSE;
   my_data->new_files = (char **) calloc(MAX_FILES, sizeof(char*) );
   my_data->build_type = BUILD_MAKE;
   return my_data;
}



void Clean_Up(PBI_Struct *data, int quietly)
{
    int index;
    pid_t my_pid;
    char filename[256];
    BIN_FILE *current_exe, *next_exe;

    if (! data)
       return;

    if ( (data->logging) && (! quietly) )
       fprintf(data->log, "Cleaning up PBI data.\n");

    Restore_Make_Config(data, quietly);
    my_pid = getpid();
    sprintf(filename, "/tmp/%d", my_pid);
    unlink(filename);

    if (data->author_name) free(data->author_name);
    if (data->web_site) free(data->web_site);
    if (data->port_name) free(data->port_name);
    if (data->port_name8) free(data->port_name8);
    if (data->port_name9) free(data->port_name9);
    if (data->port_dir) free(data->port_dir);
    if (data->backup_icon) free(data->backup_icon);

    if (data->new_files)
    {  
       for (index = 0; index < MAX_FILES; index++)
       {
          if (data->new_files[index])
             free(data->new_files[index]);
       }
       free(data->new_files);
    }
    if (data->icon_name) free(data->icon_name);
    if (data->main_exe) free(data->main_exe);
    if (data->category) free(data->category);
    if (data->exclude) free(data->exclude);
    if (data->dir_owner) free(data->dir_owner);
    if (data->port_prefix) free(data->port_prefix);
    if ( (data->log) && (data->logging == LOGGING_FILE) )
         fclose(data->log);

    // remove all bin files
    if (data->more_exe)
    {
       current_exe = data->more_exe;
       while (current_exe)
       {
           next_exe = (BIN_FILE *) current_exe->next;
           if (current_exe->path)
              free(current_exe->path);
           free(current_exe);
           current_exe = next_exe;
       }
    }
    free(data);
}



int Get_Options(PBI_Struct *data, int args, char *params[])
{
     int index = 0;
     int module_version = FALSE;
     int slashes = 0;

     if (args < 2)
     {
         printf("Version: %s\n", MAKEAPBI_VERSION);
         printf("usage: pbimaker <-8 or -9> [options] <path_to_port>\n\n");
         printf("Options:\n");
         printf("  -q         -- Run in quiet mode, no logging or output.\n");
         printf("  -l logfile -- Save output to a logfile.\n");
         printf("  -i         -- Run in interactiv mode.\n");
         printf("  -e dir     -- Exclude a directory from searched.\n");
         printf("  -o user    -- chown the module to user\n");
         printf("  -c         -- target port is a console app, not a GUI\n");
         printf("  -p         -- install via pkg_add, not make\n");
         printf("  -b icon    -- if no icon is found, use this one\n");
         printf("  -s         -- skip building/installing port\n");
         printf("  -8         -- build module for PBI version 8\n");
         printf("  -9         -- build module for PBI version 9\n");
         printf("\n");
         return FALSE;
     }

     while (index < args)
     {
         if (! strcmp(params[index], "-q") )
            data->logging = LOGGING_SILENT;
         else if ( (! strcmp(params[index], "-l") ) && (index < args - 1) )
         {
            data->logging = LOGGING_FILE;
            data->log = fopen(params[index + 1], "w");
            if (! data->log)
               return FALSE;
            else
               index++;
         }
         else if ( (! strcmp(params[index], "-b") ) && (index < args - 1) )
         {
            data->backup_icon = strdup(params[index + 1]);
         }
         else if ( (! strcmp(params[index], "-e") ) && (index < args - 1) )
         {
             data->exclude = strdup(params[index + 1]);
         }
         else if (! strcmp(params[index], "-i") )
            data->interactive = TRUE;
         else if (! strcmp(params[index], "-c") )
            data->console_app = TRUE;
         else if (! strcmp(params[index], "-p") )
            data->build_type = BUILD_PKG;
         else if (! strcmp(params[index], "-s") )
            data->skip_build = TRUE;
         else if ( (! strcmp(params[index], "-o") ) && (index < args - 1) )
         {
             data->dir_owner = strdup(params[index + 1]);
         }
        else if (! strcmp(params[index], "-8") )
        {
            module_version = TRUE;
            data->build_module_eight = TRUE;
        }
        else if (! strcmp(params[index], "-9") )
        {
            module_version = TRUE;
            data->build_module_nine = TRUE;
        }
         index++;
     }

     // last item should be our port
     data->port_dir = strdup(params[args - 1]);
     if (! data->port_dir)
         return FALSE;

     // we have the full path, now find the prefix
     // to the port name
     // remove trailing '/'
     if (data->port_dir[strlen(data->port_dir) - 1] == '/')
         data->port_dir[strlen(data->port_dir) - 1] = '\0';
     data->port_prefix = strdup(data->port_dir);
     // the prefix should be everything up to and including the
     // second '/' from the right
     if (data->port_prefix)
     {
        index = strlen(data->port_prefix) - 1;
        while ( (slashes < 2) && (index >= 0) )
        {
            if (data->port_prefix[index] == '/')
            {
               slashes++;
               if (slashes < 2)
                  index--;
            }
            else
               index--;
        }
        // cut off the tail of the prefix string
        if (index > 0)
           data->port_prefix[index + 1] = '\0';
     }

     if (! module_version)
         return FALSE;

     return TRUE;
}


int Port_Exists(PBI_Struct *data)
{
    DIR *my_dir;

    my_dir = opendir(data->port_dir);
    if (my_dir)
    {
        closedir(my_dir);
        if (data->logging)
          fprintf(data->log, "Found port %s\n", data->port_dir);
        return TRUE;
    }
    else
    {
        if (data->logging)
           fprintf(data->log, "Unable to find port %s\n", data->port_dir);
        return FALSE;
    }
}


int Check_Root(PBI_Struct *data)
{
    uid_t my_id;

    my_id = getuid();
    if (my_id == 0)
    {
       if (data->logging)
         fprintf(data->log, "Root access confirmed.\n");
       return TRUE;
    }
    else
    {
       if (data->logging)
         fprintf(data->log, "We do not have root access.\n");
       return FALSE;
    }
}



int Get_Name(PBI_Struct *data)
{
   char *last_slash;
   int name_length;

   /*
   This is done in the Get_Options function
   if ( data->port_dir[ strlen(data->port_dir) - 1 ] == '/' )
      data->port_dir[ strlen(data->port_dir) - 1] = '\0';
   */
   last_slash = strrchr(data->port_dir, '/');
   if (!last_slash)
   {
      if (data->logging)
        fprintf(data->log, "Was unable to find port name from %s\n",
                data->port_dir);
      return FALSE;
   }

   last_slash++;
   data->port_name = strdup(last_slash);
   if ( (data->port_name) && (data->logging) )
   {
      fprintf(data->log, "Got port name: %s\n", data->port_name);
      name_length = strlen(data->port_name);
      data->port_name8 = (char *) calloc( name_length + 3, sizeof(char) );
      if (data->port_name8)
      {
          strcpy(data->port_name8, data->port_name);
          strcat(data->port_name8, "-8");
      }
      data->port_name9 = strdup(data->port_name);
   }
   else if (data->logging)
      fprintf(data->log, "Unable to get port name.\n");

   return TRUE;
}


int Create_Module_Template(PBI_Struct *data)
{
    int result;

    if (data->logging)
       fprintf(data->log, "Creating empty module template, version 8.\n");

    mkdir(data->port_name8, 0755);
    result = chdir(data->port_name8);
    if (result == -1)
    {
       if (data->logging)
         fprintf(data->log, "Unable to complete version 8 template.\n");
       return FALSE;
    }
    mkdir("kmenu-dir", 0755);
    mkdir("mime-dir", 0755);
    mkdir("overlay-dir", 0755);
    // mkdir("overlay-dir/bin", 0755);
    chdir("..");

    if (data->logging)
       fprintf(data->log, "Done creating module 8 template.\n");
    return TRUE; 
}



int Create_Module_Template9(PBI_Struct *data)
{
   int result;

   if (data->logging)
      fprintf(data->log, "Creating empty module template, version 9.\n");

   mkdir(data->port_name9, 0755);
   result = chdir(data->port_name9);
   if (result == -1)
   {
       if (data->logging)
          fprintf(data->log, "Unable to complete module 9 template.\n");
       return FALSE;
   }
   mkdir("resources", 0755);
   mkdir("scripts", 0755);
   mkdir("xdg-desktop", 0755);
   mkdir("xdg-menu", 0755);
   mkdir("xdg-mime", 0755);
   chdir("..");

   if (data->logging)
      fprintf(data->log, "Done creating module 9 template.\n");
   return TRUE;
}




int Confirm_Batch_Mode(PBI_Struct *data)
{
    FILE *my_conf;

    if (data->logging)
       fprintf(data->log, "Switching make to batch mode.\n");

    system("cp /etc/make.conf /etc/make.conf.pbi");
    my_conf = fopen("/etc/make.conf", "w");
    if (my_conf)
    {
       fprintf(my_conf, "BATCH=yes\n");
       fclose(my_conf);
       if (data->logging)
         fprintf(data->log, "Switch to batch mode complete.\n");
       return TRUE;
    }
    else
    {
       if (data->logging)
          fprintf(data->log, "Was unable to create batch mode.\n");
       system("mv /etc/make.conf.pbi /etc/make.conf");
       return FALSE;
    }

}


int Create_Time_File(PBI_Struct *data)
{
    FILE *my_file;
    pid_t my_pid;
    char filename[256];

    my_pid = getpid();
    sprintf(filename, "/tmp/%d", my_pid);
    if (data->logging)
      fprintf(data->log, "Creating temporary file %s\n", filename);
    my_file = fopen(filename, "w");
    if (my_file)
    {
       fclose(my_file);
       return TRUE;
    }
    return FALSE;    
}




int Make_Port(PBI_Struct *data)
{
    char current_dir[1024];
    char *result;
    int status;

    // get cwd
    result = getcwd(current_dir, 1024);
    if (! result)
    {
       if (data->logging)
         fprintf(data->log, "Unable to get current working directory.\n");
       return FALSE;
    }

    if (data->logging)
       fprintf(data->log, "Moving to port directory %s\n", data->port_dir);
    // move to port dir
    status = chdir(data->port_dir);
    if (status)
    {
       if (data->logging)
          fprintf(data->log, "Something went wrong changing dir.\n");
       return FALSE;
    }

    // make the port
    if (data->logging)
       fprintf(data->log, "Attempting to make the port.\n");

    status = system("make install");
    // check return status
    if (status)
    {
       if (data->logging)
         fprintf(data->log, "Something went wrong during the make.\n");
       chdir(current_dir);   // return home
       return FALSE;
    }

    // return to previous cwd
    if (data->logging)
    {
      fprintf(data->log, "Completed make. Everything appears OK.\n");
      fprintf(data->log, "Returning to directory %s\n", current_dir);
    }
    chdir(current_dir);
    return TRUE;
}



int Install_Port(PBI_Struct *data)
{
    char *command_line;
    int status;

    if (data->logging)
        fprintf(data->log, "Attempting to install port.\n");

    command_line = (char *) calloc( strlen(data->port_name) + 32, sizeof(char));
    if (!command_line)
    {
       if (data->logging)
          fprintf(data->log, "Unable to allocate memory.\n");
       return FALSE;
    }

    sprintf(command_line, "pkg_add -r -v %s", data->port_name);
    status = system(command_line);
    free(command_line);
    if (status)
    {
        if (data->logging)
          fprintf(data->log, "Error trying to install port.\n");
        return FALSE;
    }
    if (data->logging)
       fprintf(data->log, "Finished installing port.\n");
    return TRUE;
}





int Find_Port_Files(PBI_Struct *data)
{
   char command[512];
   char buffer[1024], *result;
   pid_t my_pid;
   FILE *my_pipe;
   int index = 0;

   my_pid = getpid();
   sprintf(command, "find /usr/local -type f -newer /tmp/%d", my_pid);
   if (data->logging)
      fprintf(data->log, "Finding files using %s\n", command);
   my_pipe = popen(command, "r");
   if (!my_pipe)
      return FALSE;

   result = fgets(buffer, 1024, my_pipe);
   while ( (result) && (index < MAX_FILES) )
   {
       Trim_Newline(buffer);
       if (data->logging)
          fprintf(data->log, "Found new file: %s\n", buffer);
       if ( (data->exclude) && ( strstr(buffer, data->exclude) ) )
       {
          if (data->logging)
            fprintf(data->log, "Skipping due to exclude.\n");
       }
       else 
          data->new_files[index] = strdup(buffer);
       index++;
       result = fgets(buffer, 1024, my_pipe); 
   }
   pclose(my_pipe);

   sprintf(command, "find /usr/local/ -type l -newer /tmp/%d", my_pid);
   if (data->logging)
      fprintf(data->log, "Finding links using %s\n", command);
   my_pipe = popen(command, "r");
   if (!my_pipe)
      return FALSE;

   result = fgets(buffer, 1024, my_pipe);
   while ( (result) && (index < MAX_FILES) )
   {
      Trim_Newline(buffer);
      if (data->logging)
         fprintf(data->log, "Found new link: %s\n", buffer);
      if ( (data->exclude) && ( strstr(buffer, data->exclude) ) )
      {
          if (data->logging)
            fprintf(data->log, "Skipping due to exclude.\n");
      }
      else
          data->new_files[index] = strdup(buffer);

      index++;
      result = fgets(buffer, 1024, my_pipe);
    }
    pclose(my_pipe);

   return TRUE;
}


int Get_Website(PBI_Struct *data)
{
   char full_filename[1024];
   FILE *phile;
   char buffer[256];   // lines in file
   char *result, *my_http = NULL;

   if (data->logging)
      fprintf(data->log, "Trying to guess website.\n");
   sprintf(full_filename, "%s/pkg-descr", data->port_dir);
   phile = fopen(full_filename, "r");
   if (phile)
   {
      result = fgets(buffer, 256, phile);
      while ( (result) && (! my_http) )
      {
          // find http string
          result = strstr(buffer, "http");
          if (result)
             my_http = result;
          else
             result = fgets(buffer, 256, phile);
      }
      if (my_http)
         data->web_site = strdup(my_http);
      fclose(phile);
   }

   if (data->web_site)
   {
      Trim_Newline(data->web_site);
      if (data->logging)
        fprintf(data->log, "Found website %s\n", data->web_site);
      return TRUE;
   }
   // check for interactive mode so user can supply site
   else if (data->interactive)
   {
      printf("Unable to find website.\nPlease supply %s's website> ",
             data->port_name);
      fgets(buffer, 256, stdin);
      data->web_site = strdup(buffer);
      Trim_Newline(data->web_site);
      return TRUE;
   }
   else  // no site and no interaction
   {
      if (data->logging)
        fprintf(data->log, "Unable to get port's website.\n");
      return FALSE;
   }
   
}


int Get_Author(PBI_Struct *data)
{
    char full_filename[1024];
    char buffer[256], *result;
    int found = FALSE;
    FILE *phile;

    sprintf(full_filename, "%s/Makefile", data->port_dir);
    phile = fopen(full_filename, "r");
    if (phile)
    {
       // search for maintainer field
       result = fgets(buffer, 256, phile);
       while ( (result) && (! found) )
       {
          result = strstr(buffer, "MAINTAINER=");
          if (result)
          {
             data->author_name = strdup( & (result[12]) );
             found = TRUE;
          }
          result = fgets(buffer, 256, phile);
       }
       fclose(phile);
    }

    // did not find name, check the description file
    if (! data->author_name)
    {
        sprintf(full_filename, "%s/pkg-descr", data->port_dir);
        phile = fopen(full_filename, "r");
        if (phile)
        {
             // search for e-mail address
             result = fgets(buffer, 256, phile);
             while ( (result) && (!found) )
             {
                 result = strchr(buffer, '@');
                 if (result)
                 {
                    // avoid leading tab character
                    data->author_name = strdup(& (buffer[1]) );
                    found = TRUE;
                 }
                 else
                    result = fgets(buffer, 256, phile);
             }
        }
    }



    if (data->author_name)
    {
        if (data->logging)
          fprintf(data->log, "Found author %s\n", data->author_name);
        Trim_Newline(data->author_name);
        return TRUE;
    }
    else if (data->interactive)
    {
       printf("Could not guess the author of %s\n", data->port_name);
       printf("Please provide author> ");
       fgets(buffer, 256, stdin);
       data->author_name = strdup(buffer);
       Trim_Newline(data->author_name);
       return TRUE; 
    }
    else
    {
        if (data->logging)
           fprintf(data->log, "Was unable to find port's author.\n");
        return FALSE;
    }
   
}


int Get_Icon(PBI_Struct *data)
{
    int index = 0;
    int found = FALSE;
    char *result, *result2;
    char full_path[PATH_MAX];
    FILE *icon_file;

    while ( (index < MAX_FILES) && (! found) )
    {
         result = NULL;
         result2 = NULL;
         if (data->new_files[index])
         {
            result = strstr(data->new_files[index], "icon");
            result2 = strstr(data->new_files[index], ".png");
         }
         if ( (result) && (result2) )
            found = TRUE;
         else
           index++;
    }

    // look for port files
    if (found)
    {
       data->icon_name = strdup(data->new_files[index]);
       if (data->logging)
          fprintf(data->log, "Found icon file %s\n", data->icon_name);
       // this is cutting off the filename and keeping the path, not what we want
       // if (strrchr(data->icon_name, '/') )
       //    strrchr(data->icon_name, '/')[0] = '\0';
       return TRUE;
    }

    // cannot find icon the hard way, try to find the default
    // icon in the following places...
    // local directory, PREFIX/share/pbimaker
    icon_file = fopen("defaulticon.png", "r");
    if (icon_file)
    {
       found = TRUE;
       getcwd(full_path, PATH_MAX);
       data->icon_name = (char *) calloc( strlen(full_path) + 32, sizeof(char) );
       sprintf(data->icon_name, "%s/defaulticon.png", full_path);
       fclose(icon_file);
    }
    else  // didn't find file here
    {
        sprintf(full_path, "%s/share/pbimaker/defaulticon.png", PREFIX);
        icon_file = fopen(full_path, "r");
        if (icon_file)
        {
           found = TRUE;
           data->icon_name = strdup(full_path);
           fclose(icon_file);
        }
    }

    if (found)
    {
         if (data->logging)
            fprintf(data->log, "Using default icon %s\n", data->icon_name);
         return TRUE;
    }
    else
    {
       if (data->logging)
         fprintf(data->log, "Could not find icon file.\n");
       if (data->backup_icon)
       {
            data->icon_name = strdup(data->backup_icon);
            if ( (data->icon_name) && (data->logging) )
               fprintf(data->log, "Using backup icon %s.\n", data->icon_name);
            if (data->icon_name)
               return TRUE;
       }
       return FALSE;
    }
}



int Get_Category(PBI_Struct *data)
{
   char *cat;

   cat = (char *) calloc(1024, sizeof(char));
   if (! cat)
      return FALSE;

   if ( strstr(data->port_dir, "games") )
       strcpy(cat, "Games");
   else if ( strstr(data->port_dir, "devel") )
       strcpy(cat, "Development");
   else if ( strstr(data->port_dir, "multimedia"))
       strcpy(cat, "Multimedia");
   else if ( strstr(data->port_dir, "audio") )
       strcpy(cat, "Multimedia");
   else if ( strstr(data->port_dir, "graphics"))
       strcpy(cat, "Graphics");
   else if ( strstr(data->port_dir, "www") )
       strcpy(cat, "Internet");
   else if ( strstr(data->port_dir, "net") )
       strcpy(cat, "Internet");
   else if ( strstr(data->port_dir, "ftp") )
       strcpy(cat, "Internet");
   else if ( strstr(data->port_dir, "mail") )
       strcpy(cat, "Internet");
   else if ( strstr(data->port_dir, "cad") )
       strcpy(cat, "Graphics");
   else
       strcpy(cat, "Utilities");

   if (cat[0])
   {
      if (data->logging)
         fprintf(data->log, "Assign port to %s category.\n", cat);
      data->category = cat;
      return TRUE; 
   }
   else if (data->interactive)
   {
      printf("Was unable to guess cateogry.\n");
      printf("What is %s's menu cateogry? ", data->port_name);
      fgets(cat, 1024, stdin);
      data->category = cat;
      return TRUE;
   }
   else
   {
     // free(cat);
     data->category = cat;
     data->category[0] = ' ';
     return FALSE;
   }
}


int Write_Conf(PBI_Struct *data)
{
   FILE *my_file;
   char filename[256];
   char *icon_filename = NULL;
 
   if (data->logging)
     fprintf(data->log, "Creating PBI conf file.\n");

   // figure out icon's filename
   if (data->icon_name)
   {
      icon_filename = strrchr(data->icon_name, '/');
      if (icon_filename)
         icon_filename++;
   }

   sprintf(filename, "%s/pbi.conf", data->port_name8);
   my_file = fopen(filename, "w");
   if (my_file)
   {
      fprintf(my_file, "#!/bin/sh\n");
      fprintf(my_file, "PROGNAME=\"%s\"\n", data->port_name);
      if (data->web_site)
         fprintf(my_file, "PROGWEB=\"%s\"\n", data->web_site);
      else
         fprintf(my_file, "PROGWEB=\"\"\n");
      if (data->author_name)
         fprintf(my_file, "PROGAUTHOR=\"%s\"\n", data->author_name);
      else
         fprintf(my_file, "PROGAUTHOR=\"\"\n");
      if (icon_filename)
         fprintf(my_file, "PROGICON=\"%s\"\n", icon_filename);
      else if (data->icon_name)
         fprintf(my_file, "PROGICON=\"%s\"\n", data->icon_name);
      else
         fprintf(my_file, "PROGICON=\"\"\n");
      fprintf(my_file, "PBIPORT=\"%s\"\n", data->port_dir);
      fprintf(my_file, "PROGLIBS=\"Auto\"\n");
      fprintf(my_file, "PBIUPDATE=\"http://update.pbidir.com\"\n");
      fprintf(my_file, "OTHERPORT=\"\"\n");
      fprintf(my_file, "MAKEOPTS=\"\"\n");
      fprintf(my_file, "BUILDKEY=\"01\"\n");
      fprintf(my_file, "export PBIPORT MAKEOPTS PROGNAME PROGWEB PROGAUTHOR OTHERPORT PROGICON PROGLIBS PBIUPDATE BUILDKEY\n");
      fclose(my_file);
      chmod(filename, 0755);   // make config script executable
      return TRUE;
   }

   if (data->logging)
     fprintf(data->log, "Unable to create pbi.conf file.\n");
   return FALSE;
}



int Write_Conf9(PBI_Struct *data)
{
   FILE *my_file;
   char filename[256];
   char *icon_filename = NULL;
   char port_category[256];
   int index, dest_index = 0;

   if (data->logging)
     fprintf(data->log, "Creating PBI 9 conf file.\n");

   // figure out icon's filename
   if (data->icon_name)
   {
      icon_filename = strrchr(data->icon_name, '/');
      if (icon_filename)
         icon_filename++;
   }

   // get the port's category
   // as in /usr/ports/category/portname
   index = strlen(data->port_prefix);
   while ( (data->port_dir[index]) && (data->port_dir[index] != '/') )
   {
      port_category[dest_index] = data->port_dir[index];
      dest_index++;
      index++;
   }
   port_category[dest_index] = '\0';

   sprintf(filename, "%s/pbi.conf", data->port_name9);
   my_file = fopen(filename, "w");
   if (my_file)
   {
      fprintf(my_file, "#!/bin/sh\n");
      fprintf(my_file, "PBI_PROGNAME=\"%s\"\n", data->port_name);
      if (data->web_site)
         fprintf(my_file, "PBI_PROGWEB=\"%s\"\n", data->web_site);
      else
         fprintf(my_file, "PBI_PROGWEB=\"\"\n");
      if (data->author_name)
         fprintf(my_file, "PBI_PROGAUTHOR=\"%s\"\n", data->author_name);
      else
         fprintf(my_file, "PBI_PROGAUTHOR=\"\"\n");
      if (icon_filename)
         fprintf(my_file, "PBI_PROGICON=\"%s\"\n", icon_filename);
      else if (data->icon_name)
         fprintf(my_file, "PBI_PROGICON=\"%s\"\n", data->icon_name);
      else
         fprintf(my_file, "PBI_PROGICON=\"\"\n");
      fprintf(my_file, "PBI_MAKEPORT=\"%s/%s\"\n", port_category, data->port_name);
      fprintf(my_file, "PBI_MKPORTBEFORE=\"\"\n");
      fprintf(my_file, "PBI_MKPORTAFTER=\"\"\n");
      fprintf(my_file, "PBI_MAKEOPTS=\"\"\n");
      fprintf(my_file, "export PBI_MAKEOPTS PBI_PROGNAME PBI_PROGWEB PBI_PROGAUTHOR PBI_MKPORTBEFORE PBI_MKPORTAFTER PBI_PROGICON PBI_MAKEPORT\n");
      fclose(my_file);
      chmod(filename, 0755);   // make config script executable
      return TRUE;
   }
   if (data->logging)
     fprintf(data->log, "Unable to create pbi.conf file.\n");
   return FALSE;
}


int Write_Copyfiles(PBI_Struct *data)
{
    FILE *cfile;
    char filename[256];
    int count = 0;
    char *last_slash, *start_pos;
    
    if (data->logging)
        fprintf(data->log, "Creating copy-files\n");
    sprintf(filename, "%s/copy-files", data->port_name8);
    cfile = fopen(filename, "w");
    if (cfile)
    {
       while (count < MAX_FILES)
       {
          if (data->new_files[count])
          {
            fprintf(cfile, "%s ", data->new_files[count]);
            last_slash = strrchr(data->new_files[count], '/');
            if (last_slash)
            {
               start_pos = data->new_files[count] + 11;
               while (start_pos <= last_slash)
               {
                 fputc((int) start_pos[0], cfile);
                 start_pos++;
               }
            }
            fprintf(cfile, "\n");
          }
            count++;
       }
       fclose(cfile);
       if (data->logging)
         fprintf(data->log, "Finished writing copy-files\n");
       return TRUE;
    }
    
    if (data->logging)
      fprintf(data->log, "Unable to create copy-files\n");
    return FALSE;
}



int Write_Kmenu(PBI_Struct *data)
{
   FILE *kfile;
   char filename[256];
   char *my_exe = NULL;
   char *icon_description = NULL;

   if (data->logging)
     fprintf(data->log, "Creating Kmenu file.\n");

   sprintf(filename, "%s/kmenu-dir/%s", data->port_name8, data->port_name);
   if (data->main_exe)
   {
      my_exe = strrchr(data->main_exe, '/');
      if (my_exe)
          my_exe++;
   }
   kfile = fopen(filename, "w");
   if (kfile)
   {
      if (my_exe)
         fprintf(kfile, "ExePath: bin/%s\n", my_exe);
      else
         fprintf(kfile, "ExePath: bin/\n");
      if (data->icon_name)
      {
          char *temp_name = strrchr(data->icon_name, '/');
          if (temp_name)
          {
             temp_name++;
             fprintf(kfile, "ExeIcon: %s\n", temp_name);
          }
          else if (data->icon_name)
            fprintf(kfile, "ExeIcon: %s\n", data->icon_name);
          else
             fprintf(kfile, "ExeIcon: \n");
             
      }
      else
          fprintf(kfile, "ExeIcon: \n");

      icon_description = strdup(data->port_name);
      if (icon_description)
      {
         icon_description[0] = toupper(icon_description[0]);
         fprintf(kfile, "ExeDescr: %s\n", icon_description);
         free(icon_description);
      }
      else
         fprintf(kfile, "ExeDescr: %s\n", data->port_name);

      if (data->console_app)
      {
        fprintf(kfile, "ExeNoDesktop: 1\n");
        fprintf(kfile, "ExeNoMenu: 1\n");
        fprintf(kfile, "ExeNoCrashHandler: 1\n");
        fprintf(kfile, "ExeRunRoot: 0\n");
        fprintf(kfile, "ExeRunShell: 1\n");
        fprintf(kfile, "ExeNotify: 1\n");
      }
      else     // GUI app
      {
        fprintf(kfile, "ExeNoDesktop: 0\n");
        fprintf(kfile, "ExeNoMenu: 0\n");
        fprintf(kfile, "ExeNoCrashHandler: 0\n");
        fprintf(kfile, "ExeRunRoot: 0\n");
        fprintf(kfile, "ExeRunShell: 0\n");
        fprintf(kfile, "ExeNotify: 1\n");
      }

      fprintf(kfile, "ExeLink: 0\n");
      fprintf(kfile, "ExeWebLink: 0\n");
      fprintf(kfile, "ExeTaskbar: 0\n");
      fprintf(kfile, "ExeOwndir: 2\n");
      fprintf(kfile, "ExeKdeCat: %s\n", data->category);

      fclose(kfile);
      if (data->logging)
        fprintf(data->log, "Finished writing to Kmenu file.\n");
      return TRUE;
   }
   
   if (data->logging)
     fprintf(data->log, "Unable to create Kmenu file %s.\n", filename);
   return FALSE;
}



int Write_Menu(PBI_Struct *data, char *bin_file)
{
   FILE *my_file;
   char filename[256];
   char *my_exe = NULL;
   char *proper_name = NULL;

   if (data->console_app)
   {
      if (data->logging)
        fprintf(data->log, "Skipping desktop entry.\n");
      return TRUE;
   }

   if (data->logging)
     fprintf(data->log, "Creating desktop entry.\n");

   if ( (data->main_exe) && (! bin_file) )
   {
      my_exe = strrchr(data->main_exe, '/');
      if (my_exe)
          my_exe++;
   }
   else
     my_exe = bin_file;

   if (! my_exe)
   {
      if (data->logging)
        fprintf(data->log, "Couldn't find exe to write to menu/desktop.\n");
      return FALSE;
   }

   proper_name = strdup(my_exe);
   if (proper_name)
       proper_name[0] = toupper(proper_name[0]);
  

   sprintf(filename, "%s/xdg-desktop/%s.desktop", data->port_name9, my_exe);
   my_file = fopen(filename, "w");
   if (my_file)
   {
      fprintf(my_file, "[Desktop Entry]\n");
      fprintf(my_file, "Value=1.0\n");
      fprintf(my_file, "Type=Application\n");
      if (proper_name)
         fprintf(my_file, "Name=%s\n", proper_name);
      else
         fprintf(my_file, "Name=%s\n", my_exe);
      fprintf(my_file, "GenericName=%s\n", my_exe);
      fprintf(my_file, "Exec=%%%%PBI_EXEDIR%%%%/%s\n", my_exe);
      fprintf(my_file, "Path=%%%%PBI_APPDIR%%%%\n");
      if (data->icon_name)
      {
          char *temp_name = strrchr(data->icon_name, '/');
          if (temp_name)
          {
             temp_name++;
             fprintf(my_file, "Icon=%%%%PBI_APPDIR%%%%/%s\n", temp_name);
          }
          else if (data->icon_name[0])
             fprintf(my_file, "Icon=%%%%PBI_APPDIR%%%%/%s\n", data->icon_name);
          else
             fprintf(my_file, "Icon=\n");
      }
      fprintf(my_file, "StartupNotify=true\n");
      fclose(my_file);
   }

   if (data->logging)
     fprintf(data->log, "Writing menu file.\n");

   sprintf(filename, "%s/xdg-menu/%s.desktop", data->port_name9, my_exe);
   my_file = fopen(filename, "w");
   if (my_file)
   {
      fprintf(my_file, "[Desktop Entry]\n");
      fprintf(my_file, "Value=1.0\n");
      fprintf(my_file, "Type=Application\n");
      if (proper_name)
         fprintf(my_file, "Name=%s\n", proper_name);
      else
         fprintf(my_file, "Name=%s\n", my_exe);
      fprintf(my_file, "GenericName=%s\n", my_exe);
      fprintf(my_file, "Exec=%%%%PBI_EXEDIR%%%%/%s\n", my_exe);
      fprintf(my_file, "Path=%%%%PBI_APPDIR%%%%\n");
      if (data->icon_name)
      {
          char *temp_name = strrchr(data->icon_name, '/');
          if (temp_name)
          {
             temp_name++;
             fprintf(my_file, "Icon=%%%%PBI_APPDIR%%%%/%s\n", temp_name);
          }
          else if (data->icon_name[0])
             fprintf(my_file, "Icon=%%%%PBI_APPDIR%%%%/%s\n", data->icon_name);
          else
             fprintf(my_file, "Icon=\n");
      }
      // fprintf(my_file, "MimeType=application/x-%s\n", data->port_name);
      fprintf(my_file, "StartupNotify=true\n");
      fprintf(my_file, "Categories=%s", data->category);

      fclose(my_file);
   }

   if (proper_name)
       free(proper_name);
   return TRUE;
}




int Restore_Make_Config(PBI_Struct *data, int quietly)
{
     FILE *old_file = NULL;

     if ( (data->logging) && (! quietly) )
        fprintf(data->log, "Reverting back to old make configuration.\n");
     // make sure file exists before we try to move it
     old_file = fopen("/etc/make.conf.pbi", "r");
     if (old_file)
     {
        fclose(old_file);
        system("mv /etc/make.conf.pbi /etc/make.conf");
     }
     return TRUE;
}


int Copy_Icon(PBI_Struct *data)
{
      char command[2048];

      if (data->icon_name)
      {
         sprintf(command, "cp %s %s/overlay-dir/", data->icon_name,
                 data->port_name8);
         if (data->logging)
           fprintf(data->log, "Copying icon file %s\n", data->icon_name);
         system(command);
         return TRUE;
      }
      return FALSE;
}



int Copy_Icon9(PBI_Struct *data)
{
   char command[2048];

   if (data->icon_name)
   {
     sprintf(command, "cp %s %s/resources/", data->icon_name, 
             data->port_name9);
    
     if (data->logging)
       fprintf(data->log, "Copying icon file, %s, to resource directory.\n",
               data->icon_name);
     system(command);
     return TRUE;
   }
   return FALSE;
}



void Trim_Newline(char *line)
{
    int index = 0;
    int my_length = strlen(line);
    int found = FALSE;
    
    while ( (index < my_length) && (! found) )
    {
        if (line[index] == '\n')
        {
           line[index] = '\0';
           found = TRUE;
        }
        else
           index++;
    }
}



int Get_Main_Exe(PBI_Struct *data)
{
    int index = 0;
    char *result;
    FILE *the_exe;
    char filename[1024];

    if (data->logging)
       fprintf(data->log, "Trying to guess the port's executable.\n");

    // first we look for /usr/local/bin/portname as that is most likely
    sprintf(filename, "/usr/local/bin/%s", data->port_name);
    the_exe = fopen(filename, "r");
    if (the_exe)
    {
        fclose(the_exe);
        if (data->logging)
          fprintf(data->log, "Found exe %s\n", filename);
        data->main_exe = strdup(filename);
        return TRUE;
    }

    // next step is to try for the first item located in the bin dir
    while (index < MAX_FILES) 
    {
        if ( data->new_files[index] )
        {
            result = strstr(data->new_files[index], "/usr/local/bin");
            if (result)
            {
                if (data->logging)
                  fprintf(data->log, "Found exe option.\n");
                data->main_exe = strdup(data->new_files[index]);
                return TRUE;
            }
        }
        index++; 
    }


    if (data->logging)
       fprintf(data->log, "Was unable to guess main executable.\n");
    return FALSE;
}



int Get_Main_Exe_Smart(PBI_Struct *data)
{
   FILE *my_file;
   int found = FALSE;
   char line[1024];
   char file_name[1024];
   char *status;

   // check the port's package list first
   snprintf(file_name, 1024, "%s/pkg-plist", data->port_dir);
   my_file = fopen(file_name, "r");
   if (my_file)
   {
      status = fgets(line, 1024, my_file);
      while ( (status) && (! found) )
      {
         if ( ( strstr(line, "bin/") ) ||
              ( strstr(line, "sbin/") ) )
         {
           if ( strstr(line, data->port_name) )
           {
               Trim_Newline(line);
               data->main_exe = strdup(line);
               found = TRUE;
           }
         }
         if (! found)
           status = fgets(line, 1024, my_file);
      }
      fclose(my_file);
   }

   if (data->logging)
   {
     if (found)
        fprintf(data->log, "Found main exe file %s\n", data->main_exe);
     else
        fprintf(data->log, "Could not find exe the smart way.\n"); 
   }
   return found;
}



// This function will search through the pkg-plist file
// looking for lines starting with bin/ or sbin/
// These items will be added to the more_exe linked list.
// The function returns TRUE if things go normally or
// FALSE if an error occurs.
int Get_More_Exe(PBI_Struct *data)
{
   BIN_FILE *new_exe, *last_exe = NULL;
   FILE *my_plist;
   char filename[1024], line[1024];
   char *status;

   if (data->logging)
     fprintf(data->log, "Looking for more bin files.\n");

   snprintf(filename, 1024, "%s/pkg-plist", data->port_dir);
   my_plist = fopen(filename, "r");
   if (! my_plist)
      return FALSE;

   status = fgets(line, 1024, my_plist);
   while (status)
   {
       // check if line starts with what we want
       // and isn't already listed in main_exe
       
       if ( (! strncmp(line, "bin/", 4) ) &&
            ( ( ! data->main_exe) ||
              (! strstr(data->main_exe, line) ) ) )
       {
          // trim line
          Trim_Newline(line);
          // try to create new node
          new_exe = (BIN_FILE *) calloc(1, sizeof(BIN_FILE));
          if (new_exe)
          {
             new_exe->path = strdup( &(line[4]) );
             if (data->more_exe)
                 last_exe->next = new_exe;
             else
                data->more_exe = new_exe;
             last_exe = new_exe;
             if (data->logging)
             {
                fprintf(data->log, "Found extra exe: %s\n", new_exe->path);
             }
          }   // end of created new node
       }   // end of line with bin entry

       status = fgets(line, 1024, my_plist);
   }   // end of while reading from file
   fclose(my_plist); 

   return TRUE;
}



int Copy_Main_Exe(PBI_Struct *data)
{
    char command[1024];
    int count = 0, copies = 0;
    char *result;

    if (data->logging)
       fprintf(data->log, "Copying port's executables to overlay dir.\n");

    while (count < MAX_FILES)
    {
       if (data->new_files[count])
       {
           result = strstr(data->new_files[count], "/usr/local/bin/");
           if (result)
           {
              // found a file to copy
              sprintf(command, "cp %s %s/overlay-dir/bin/", data->new_files[count],
                      data->port_name8);
              system(command);
              copies++;
           }
       }
       count++;
    }

    if (data->logging)
      fprintf(data->log, "Copied %d files to overlay-dir.\n", copies);
    return TRUE;
}



void Chown_Module(PBI_Struct *data)
{
    char buffer[1024];

    if (! data->dir_owner)
        return;

    snprintf(buffer, 1024, "chown -R %s %s", data->dir_owner, data->port_name8);
    if (data->logging)
      fprintf(data->log, "Performing %s\n", buffer);
    system(buffer);
}


void Chown_Module9(PBI_Struct *data)
{
    char buffer[1024];
    
    if (! data->dir_owner)
        return;
     
    snprintf(buffer, 1024, "chown -R %s %s", data->dir_owner, data->port_name9);
    if (data->logging)
      fprintf(data->log, "Performing %s\n", buffer);
    system(buffer);
}



int Is_GUI(PBI_Struct *data)
{
    int found_gui = FALSE;
    FILE *package_file;
    char line[512], *status;

    snprintf(line, 512, "%s/pkg-plist", data->port_dir);
    package_file = fopen(line, "r");
    if (! package_file)
      return FALSE;

    // found package file
    status = fgets(line, 512, package_file);
    while ( (status) && (! found_gui) )
    {
       status = strstr(line, ".png");
       if (status)
           found_gui = TRUE;
       status = strstr(line, ".desktop");
       if (status)
          found_gui = TRUE;
       status = strstr(line, ".pixmap");
       if (status)
          found_gui = TRUE;
       status = strstr(line, ".bmp");
       if (status)
           found_gui = TRUE;

       // check next line
       if (! found_gui)
          status = fgets(line, 512, package_file);
    }
    fclose(package_file);
    return found_gui;
}


#include <stdio.h>
#include <string.h>
#include "pbi.h"

int main(int argc, char *argv[])
{
   PBI_Struct *pbi_data;
   BIN_FILE *current_exe = NULL;
   int status;

   // init everything
   pbi_data = Init_Data();

   // get command line arguments
   status = Get_Options(pbi_data, argc, argv);
   if (! status)
   {
      Clean_Up(pbi_data, TRUE);
      return 1;
   }

   // check to make sure port exists
   status = Port_Exists(pbi_data);
   if (! status)
   {
       Clean_Up(pbi_data, TRUE);
       return 1;
   }

   // only require root when building
   if (! pbi_data->skip_build)
   {
     status = Check_Root(pbi_data);
     if (! status)
     {
        Clean_Up(pbi_data, TRUE);
        return 1;
     }
   }

   status = Get_Name(pbi_data);
   if (! status)
   {
      Clean_Up(pbi_data, TRUE);
      return 1;
   }

   // then create module template
   if (pbi_data->build_module_eight)
   {
     status = Create_Module_Template(pbi_data);
     if (! status)
     {
        Clean_Up(pbi_data, TRUE);
        return 1;
     }
   }
   if (pbi_data->build_module_nine)
   {
     status = Create_Module_Template9(pbi_data);
     if (! status)
     {
        Clean_Up(pbi_data, TRUE);
        return 1;
     }
   }

   // find as much data as we can on the port
   Get_Website(pbi_data);
   Get_Author(pbi_data);
   Get_Category(pbi_data);

   if (! pbi_data->skip_build)
   {
   // make sure batch mode is turned on for make
   status = Confirm_Batch_Mode(pbi_data);
   if (! status)
   {
      Clean_Up(pbi_data, TRUE);
      return 1;
   }

   // find the current time
   status = Create_Time_File(pbi_data);
   if (! status)
   {
      Clean_Up(pbi_data, TRUE);
      return 1;
   }

   // run make install on port or pkg_add the port
   if (pbi_data->build_type == BUILD_MAKE)
     status = Make_Port(pbi_data);
   else if (pbi_data->build_type == BUILD_PKG)
     status = Install_Port(pbi_data);
   else
     status = TRUE;
   if (! status)
   {
      Clean_Up(pbi_data, FALSE);
      return 2;
   }

   // find all files that have been installed on the system from the port
   status = Find_Port_Files(pbi_data);
   if (! status)
   {
      Clean_Up(pbi_data, FALSE);
      return 3;
   }

   }  /// end of making/installing port



   // write out pbi.conf, copy-files, kmenu entry
   status = Get_Main_Exe_Smart(pbi_data);
   if (! status)
      Get_Main_Exe(pbi_data);

   // get additional bin file entries
   Get_More_Exe(pbi_data);

   // We do not need to copy binary files into the bin directory
   // if (pbi_data->build_module_eight)
   //     Copy_Main_Exe(pbi_data);
   Get_Icon(pbi_data);
   if (pbi_data->build_module_eight)
      Copy_Icon(pbi_data);
   if (pbi_data->build_module_nine)
      Copy_Icon9(pbi_data);

   if (pbi_data->build_module_eight)
      Write_Conf(pbi_data);
   if (pbi_data->build_module_nine)
      Write_Conf9(pbi_data);

   // Write_Copyfiles(pbi_data);
   if (pbi_data->build_module_eight)
      Write_Kmenu(pbi_data);
   if (pbi_data->build_module_nine)
   {
      Write_Menu(pbi_data, NULL);
      current_exe = pbi_data->more_exe;
      while (current_exe)
      {
         Write_Menu(pbi_data, current_exe->path);
         current_exe = (BIN_FILE *) current_exe->next;
      }
   }

   if (pbi_data->build_module_eight)
      Chown_Module(pbi_data);
   if (pbi_data->build_module_nine)
      Chown_Module9(pbi_data);

   Clean_Up(pbi_data, FALSE);
   return 0;
}


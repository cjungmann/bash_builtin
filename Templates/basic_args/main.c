#include "local_builtin.h"
#include "argeater_setters.h"
#include <stdio.h>

#include <unistd.h>  // for read()

/**** BEGIN ARGUMENTS MANAGEMENT SECTION ****/

#include <argeater.h>

bool help_flag = false;
FILE *file = NULL;
int file_handle = -1;
long longval = 0;

AE_ITEM actions[] = {
   {(const char **)&help_flag, "help", 'h', AET_FLAG_OPTION,
    "Show usage information", NULL, TEMPLATE_argeater_bool_setter },

   {(const char **)&file, "file", 'f', AET_VALUE_OPTION,
    "File to stream", "FILE", TEMPLATE_argeater_stream_setter },

   {(const char **)&file_handle, "file2", 'F', AET_VALUE_OPTION,
    "File to open", NULL, TEMPLATE_argeater_file_setter },

   {(const char **)&longval, "long", 'l', AET_VALUE_OPTION,
    "Long value", NULL, TEMPLATE_argeater_long_setter}
};

AE_MAP action_map = INIT_MAP(actions);

void show_help(AE_MAP *map, const char *cmdname)
{
   argeater_show_usage(map, cmdname);
   argeater_show_options(map, 3);
   argeater_show_arguments(map, 3);
}

/**** END ARGUMENTS MANAGEMENT SECTION ****/

static int TEMPLATE_builtin(WORD_LIST *list)
{
   int  retval = EXECUTION_SUCCESS;

   if (list)
   {
      ACLONE *clones = CLONE_WORD_LIST(list);

      if (argeater_process(clones, &action_map))
      {
         if (help_flag)
         {
            show_help(&action_map, "TEMPLATE");
            goto early_exit;
         }

         if (file)
         {
            char buff[80];
            size_t bytes_read = fread(buff, 1, sizeof(buff)-1, file);
            buff[bytes_read] = '\0';
            printf("The stream you requested starts with:\n\x1b[33;1m%s\x1b[m\n", buff);
            fclose(file);
         }

         if (file_handle != -1)
         {
            char buff[80];
            ssize_t bytes_read = read(file_handle, buff, sizeof(buff)-1);
            buff[bytes_read] = '\0';
            printf("The file handle you requested starts with:\n\x1b[33;1m%s\x1b[m\n", buff);
            close(file_handle);
         }

         if (longval != 0)
            printf("The long value you submitted is %ld\n", longval);
      }
   }
   else
   {
      // Delete or change this section of an empty argument list is allowed.
      show_help(&action_map, "TEMPLATE");
   }

        early_exit:

   return retval;
}

static char *desc_TEMPLATE[] = {
   "TEMPLATE - My Bash builtin",
   "",
   "Use Bash builtin 'TEMPLATE' to do cool stuff.",
   (char *)NULL
};

struct builtin TEMPLATE_struct = {
   .name      = "TEMPLATE",
   .function  = TEMPLATE_builtin,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_TEMPLATE,
   .short_doc = "TEMPLATE [arguments]",
   .handle    = 0
};

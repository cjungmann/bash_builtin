#include "dispatcher.h"
#include "handle_actions.h"
#include "argeater_setters.h"
#include <stdio.h>
#include <alloca.h>

void show_verb_help(AE_MAP *map, const char *verb)
{
   const char *tname = "TEMPLATE:";
   int pre_len = strlen(tname);
   int verb_len = strlen(verb);
   char *buff = alloca(pre_len + verb_len + 1);
   char *ptr = buff;
   memcpy(ptr, tname, pre_len);
   ptr += pre_len;
   memcpy(ptr, verb, verb_len);
   ptr += verb_len;
   *ptr = '\0';

   argeater_show_usage(map, buff);

   // Indent info 3 spaces:
   argeater_show_options(map, 3);
   argeater_show_arguments(map, 3);
}

int TEMPLATE_display_help(H_TEMPLATE* handle, ACLONE *args)
{
   int retval = EXECUTION_SUCCESS;
   printf("You're in the help action.\n");
   return retval;
}

int TEMPLATE_declare(H_TEMPLATE* handle, ACLONE *args)
{
   int exit_code;

   bool help_flag = false;
   FILE *file = NULL;

   AE_ITEM declare_actions[] = {
      {(const char **)&help_flag, "help", 'h', AET_FLAG_OPTION,
       "Show TEMPLATE declare usage", NULL, TEMPLATE_argeater_bool_setter },

      {(const char **)&file, "file", 'f', AET_VALUE_OPTION,
       "File stream to read", NULL, TEMPLATE_argeater_stream_setter }
   };

   AE_MAP action_map = INIT_MAP(declare_actions);

   if (argeater_process(args, &action_map))
   {
      // Ensure retval is set
      exit_code = EXECUTION_SUCCESS;

      if (help_flag)
      {
         show_verb_help(&action_map, "declare");
         goto early_exit;
      }

      if (file)
      {
         // Consume the file (replace this section):
         int bufflen = 128;
         char *buff = alloca(bufflen+1);
         size_t bytes_read = fread(buff, 1, bufflen, file);
         buff[bytes_read] = '\0';
         printf(buff);

         // close the stream:
         fclose(file);
         file = NULL;
      }
   }
   else
      exit_code = EXECUTION_FAILURE;

  early_exit:
   return exit_code;
}

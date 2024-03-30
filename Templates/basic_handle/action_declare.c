#include "dispatcher.h"
#include "argeater_setters.h"
#include "error_handling.h"
#include <stdio.h>

static bool help_flag;
static FILE *file;
static const char *handle_name;

AE_ITEM declare_actions[] = {
   {(const char **)&help_flag, "help", 'h', AET_FLAG_OPTION,
    "Show TEMPLATE declare usage", NULL, TEMPLATE_argeater_bool_setter },

   {&handle_name, "name", '\0',  AET_ARGUMENT,
    "Name of new handle", NULL, argeater_string_setter},

   {(const char **)&file, "file", '\0', AET_ARGUMENT,
    "File stream to read", NULL, TEMPLATE_argeater_stream_setter }
};

AE_MAP declare_map = INIT_MAP(declare_actions);

int TEMPLATE_declare(H_TEMPLATE *handle, ACLONE *args)
{
   int exit_code = EXECUTION_FAILURE;

   help_flag = false;
   file = NULL;
   handle_name = NULL;

   if (argeater_process(args, &declare_map))
   {
      if (handle_name == NULL)
         (*ERROR_SINK)("TEMPLATE:declare requires a handle name");
      else if (file==NULL)
         (*ERROR_SINK)("TEMPLATE:declare requires a file name");
      else
      {
         SHELL_VAR *var = find_variable(handle_name);
         if (var)
         {
            if (TEMPLATE_p(var))
               (*ERROR_SINK)("TEMPLATE:declare variable '%s' "
                             "is already a TEMPLATE handle",
                             handle_name);
            else
            {
               printf("We would be creating the new handle here.");
               exit_code = EXECUTION_SUCCESS;
            }
         }
      }
   }
   else
      exit_code = EXECUTION_FAILURE;

   return exit_code;
}

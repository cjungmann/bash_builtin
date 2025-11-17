/* -*- mode:c -*- */
/**
 * @file action_declare.def
 * @brief Declare provisions for implementing a help display function.
 */

#include "handle.h"            // for TEMPLATEH (handle) prototype
#include "dispatcher.h"        // for AVERB definition
#include "argeater_setters.h"  // argeater_setters sending errors to TEMPLATE_ERROR
#include "shell_vars.h"        // for install_payload_to_shell_var
#include "error_handling.h"    // for ERROR_SINK function pointer
#include <stdio.h>             // printf
#include <stdbool.h>           // for bool data type

/***** CONSTRUCT THE ARGUMENT MAP *****/

// File-scope status variable(s) that will contain the
// command-line argument settings after the argument map
// to which they are attached is used by argeater's
// `argeater_process` function.
static FILE *file;
static const char *handle_name;

// Array of items that associate status variables to the means
// to access them and text to be used in a help display:
AE_ITEM declare_actions[] = {
   {&handle_name, "handle_name", '\0', AET_ARGUMENT,
    "Name of new handle", NULL, argeater_string_setter },

   {(const char **)&file, "file", '\0', AET_ARGUMENT,
    "File stream to read", NULL, TEMPLATE_argeater_stream_setter }
};

// File-scope instantiation of argument map to be used
// by the `argeater_process` function.
AE_MAP TEMPLATE_declare_arg_map = INIT_MAP(declare_actions);

/***** END CONSTRUCTION OF THE ARGUMENT MAP *****/

/***** Action Implementation *****/

int TEMPLATE_declare(SHELL_VAR *sv_handle, ACLONE *args)
{
   int exit_code = EXECUTION_FAILURE;

   static const char *handle_name = NULL;
   static FILE *file = NULL;

   static const char *name_arg = NULL;
   static const char *value_arg = NULL;
   static const char *callback_arg = NULL;

   static AE_ITEM items[] = {
      {&handle_name, "handle_name", '\0', AET_ARGUMENT,
       "Name of new handle", NULL, argeater_string_setter },

      {(const char **)&file, "file", '\0', AET_ARGUMENT,
       "File stream to read", NULL, TEMPLATE_argeater_stream_setter },

      {&name_arg, "generic_name", 'n', AET_VALUE_OPTION,
       "some name", NULL, argeater_string_setter },

      {&value_arg, "generic_value", 'v', AET_VALUE_OPTION,
       "some value", NULL, argeater_string_setter },

      {&callback_arg, "callback_function", 'f', AET_VALUE_OPTION,
       "script function", NULL, TEMPLATE_argeater_function_setter}
   };

   AE_MAP map = INIT_MAP(items);

   if (!argeater_process(args, &map))
   {
      (*ERROR_SINK)("Error process arguments.");
      exit_code = EXECUTION_FAILURE;
   }
   else
   {
      // Handle allocated resources (open stream) first
      // to ensure they are deallocated upon exit:
      if (file==NULL)
         (*ERROR_SINK)("TEMPLATE:declare failed to open required file");
      else
      {
         if (handle_name)
         {
            int hlen = TEMPLATE_calc_handle_size(name_arg,
                                                 value_arg,
                                                 callback_arg);
            char *buff = xmalloc(hlen);
            if (buff)
            {
               TEMPLATEH* th = TEMPLATE_initialize_handle(buff, hlen,
                                                          name_arg,
                                                          value_arg,
                                                          callback_arg);

               if (th)
               {
                  SHELL_VAR *newsv = NULL;
                  exit_code = install_payload_to_shell_var(&newsv, handle_name, buff);
               }
               else
               {
                  (*ERROR_SINK)("TEMPLATE:handle initialization failed");
                  free(buff);
                  buff = NULL;
               }
            }
            else
               (*ERROR_SINK)("TEMPLATE:out-of-memory while allocating handle");
         }
         else
            (*ERROR_SINK)("TEMPLATE:declare requires a handle name");

         // Use the open stream, 'file', then close it before
         // exiting 'declare' to prevent leaving open handles
         // upon an unexpected termination:

         // collect_info_from_file(file);
         if (file != stdin)
            fclose(file);
      }
   }

   return exit_code;
}

/***** AVERB for action table *****/

AVERB TEMPLATE_declare_verb = {
   "declare",                            // verb name to match
   "Create a new TEMPLATE handle",       // description to use for 'help' action
   "TEMPLATE declare [new_handle_name]", // invocation of verb
   false,                                // true if handle must already be declared
   TEMPLATE_declare,                     // reference to action function
   &TEMPLATE_declare_arg_map             // reference to argument map (for help display)
};

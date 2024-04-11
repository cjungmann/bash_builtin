/* -*- mode:c -*- */
/**
 * @file action_declare.def
 * @brief Declare provisions for implementing a help display function.
 */

#include "handle.h"            // for H_TEMPLATE (handle) prototype
#include "dispatcher.h"        // for AVERB definition
#include "argeater_setters.h"  // argeater_setters sending errors to TEMPLATE_ERROR
#include "shell_vars.h"        // for install_payload_to_new_shell_var
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

int TEMPLATE_declare(H_TEMPLATE *handle, ACLONE *args)
{
   int exit_code = EXECUTION_FAILURE;

   file = NULL;
   handle_name = NULL;

   if (argeater_process(args, &TEMPLATE_declare_arg_map))
   {
      // Handle allocated resources (open stream) first
      // to ensure they are deallocated upon exit:
      if (file==NULL)
         (*ERROR_SINK)("TEMPLATE:declare failed to open required file");
      else
      {
         if (handle_name)
         {
            int lenval = strlen(TEMPLATE_HANDLE_ID);
            char *buff = xmalloc(lenval+1);
            memcpy(buff, TEMPLATE_HANDLE_ID, lenval);
            buff[lenval] = '\0';

            SHELL_VAR *newsv = NULL;
            exit_code = install_payload_to_new_shell_var(&newsv, handle_name, buff);
         }
         else
            (*ERROR_SINK)("TEMPLATE:declare requires new handle name");

         // Close the file after using it:
         if (file != stdin)
            fclose(file);
      }
   }
   else
      exit_code = EXECUTION_FAILURE;

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

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
static const char *handle_name;
static const char *string_arg;

// Array of items that associate status variables to the means
// to access them and text to be used in a help display:
AE_ITEM declare_actions[] = {
   {&handle_name, "handle_name", '\0', AET_ARGUMENT,
    "Name of new handle", NULL, argeater_string_setter },

   {&string_arg, "string", '\0', AET_ARGUMENT,
    "Random string value", NULL, argeater_string_setter }
};

// File-scope instantiation of argument map to be used
// by the `argeater_process` function.
AE_MAP TEMPLATE_declare_arg_map = INIT_MAP(declare_actions);

/***** END CONSTRUCTION OF THE ARGUMENT MAP *****/

/***** Action Implementation *****/

int TEMPLATE_declare(SHELL_VAR *sv_handle, ACLONE *args)
{
   int exit_code = EXECUTION_FAILURE;

   // Initialize global static variables used in argeater map:
   handle_name = NULL;
   string_arg = NULL;

   if (!argeater_process(args, &TEMPLATE_declare_arg_map))
   {
      (*ERROR_SINK)("Error process arguments.");
      goto early_exit;
   }

   if (handle_name == NULL)
   {
      (*ERROR_SINK)("TEMPLATE:declare requires a handle name");
      goto early_exit;
   }

   int hlen = TEMPLATE_calc_handle_size(string_arg);

   char *buff = xmalloc(hlen);
   if (buff)
   {
      TEMPLATEH* th = TEMPLATE_initialize_handle(buff, hlen, string_arg);

      if (th)
      {
         SHELL_VAR *newsv = NULL;
         exit_code = install_payload_to_shell_var(&newsv, handle_name, buff);
      }
      else
      {
         // TEMPLATE_initialize_handle will have already sent the message
         free(buff);
         buff = NULL;
      }
   }
   else
      (*ERROR_SINK)("TEMPLATE:out-of-memory while allocating handle");

  early_exit:
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

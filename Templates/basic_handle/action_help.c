/**
 * @file action_help.c
 * @brief Boilerplate provisions for implementing a help display function.
 */

#include "dispatcher.h"
#include "argeater_setters.h"
#include "error_handling.h"
#include <alloca.h>
#include <stdio.h>

// The following includes provide access to maps for help information
#include "table_of_actions.h"

/**
 * @brief Generic help display function
 * @param "verb"   action table entry for which help is being displayed.
 */
void show_verb_help(const AVERB *verb)
{
   const char *tname = "TEMPLATE ";
   int pre_len = strlen(tname);
   int verb_len = strlen(verb->name);
   char *buff = alloca(pre_len + verb_len + 1);
   char *ptr = buff;
   memcpy(ptr, tname, pre_len);
   ptr += pre_len;
   memcpy(ptr, verb->name, verb_len);
   ptr += verb_len;
   *ptr = '\0';

   argeater_show_usage(verb->arg_map, buff);

   printf("\n%s\n", verb->desc);

   // Indent info 3 spaces:
   argeater_show_arguments(verb->arg_map, 3);
   argeater_show_options(verb->arg_map, 3);
}

/***** CONSTRUCT THE ARGUMENT MAP *****/

// File-scope status variable(s) that will contain the
// command-line argument settings after the argument map
// to which they are attached is used by argeater's
// `argeater_process` function.
static const char *verb_req = NULL;

// Array of items that associate status variables to the means
// to access them and text to be used in a help display:
AE_ITEM help_actions[] = {
   { &verb_req, "action", '\0', AET_ARGUMENT,
     "Display help about [action]", NULL, argeater_string_setter}
};

// File-scope instantiation of argument map to be used
// by the `argeater_process` function.
AE_MAP TEMPLATE_help_arg_map = INIT_MAP(help_actions);

/***** END CONSTRUCTION OF THE ARGUMENT MAP *****/

/***** Action Implementation *****/

int TEMPLATE_help(SHELL_VAR *sv_handle, ACLONE *args)
{
   int exit_code = EXECUTION_FAILURE;

   verb_req = NULL;

   if (!argeater_process(args, &TEMPLATE_help_arg_map))
   {
      (*ERROR_SINK)("Error process arguments.");
      result = EXECUTION_FAILURE;
   }
   else
   {
      if (verb_req)
      {
         AVERB *verb = find_verb(TEMPLATE_action_table,
                                 TEMPLATE_actions_count,
                                 verb_req);
         if (verb)
         {
            show_verb_help(verb);
            exit_code = EXECUTION_SUCCESS;
         }
         else
            (*ERROR_SINK)("Unrecognized verb '%s' not available for TEMPLATE", verb_req);
      }
      else
      {
         printf("Access help on any of the following actions by entering:\n"
                "   TEMPLATE help [action]\n\n"
                "List of available actions:\n");

         AVERB **verb = TEMPLATE_action_table;
         AVERB **end = verb + TEMPLATE_actions_count;
         while (verb < end)
         {
            printf("   %s\n", (*verb)->name);
            ++verb;
         }
         exit_code = EXECUTION_SUCCESS;
      }
   }


   return exit_code;
}

/***** AVERB for action table *****/

AVERB TEMPLATE_help_verb = {
   "help",                              // verb name to match
   "Show help display for an action",   // description to use for 'help' action
   "TEMPLATE help [verb]",              // invocation of verb
   false,                               // handle not required to show help
   TEMPLATE_help,                       // reference to action function
   &TEMPLATE_help_arg_map               // reference to argument map (for help display)
};

#include "dispatcher.h"
#include "argeater_setters.h"
#include "error_handling.h"
#include <alloca.h>
#include <stdio.h>

// The following includes provide access to maps for help information
#include "table_of_actions.h"
#include "dispatcher.h"


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

/***** CONSTRUCT THE ARGUMENT MAP *****/

static const char *verb_req = NULL;

AE_ITEM help_actions[] = {
   { &verb_req, "verb", '\0', AET_ARGUMENT,
     "Display help", NULL, argeater_string_setter}
};

AE_MAP TEMPLATE_help_arg_map = INIT_MAP(help_actions);


int TEMPLATE_help(H_TEMPLATE* handle, ACLONE *args)
{
   int exit_code = EXECUTION_FAILURE;

   verb_req = NULL;

   if (argeater_process(args, &TEMPLATE_help_arg_map))
   {
      if (verb_req)
      {
         AVERB *verb = find_verb(TEMPLATE_action_table,
                                 TEMPLATE_actions_count,
                                 verb_req);
         if (verb)
         {
            show_verb_help(verb->arg_map, verb->name);
            exit_code = EXECUTION_SUCCESS;
         }
         else
            (*ERROR_SINK)("Unrecognized verb '%s' not available for TEMPLATE", verb_req);
      }
      else
      {
         AVERB *verb = TEMPLATE_action_table;
         AVERB *end = verb + TEMPLATE_actions_count;
         while (verb < end)
         {
            printf("TEMPLATE action '%s'\n", verb->name);
            ++verb;
         }
         exit_code = EXECUTION_SUCCESS;
      }
   }


   return exit_code;
}

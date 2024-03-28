#include "local_builtin.h"
#include "argeater_setters.h"
#include "action_table.h"
#include "error_handling.h"
#include "action_dispatch.h"
#include <stdio.h>

#include <unistd.h>  // for read()

/**** BEGIN ARGUMENTS MANAGEMENT SECTION ****/

#include <argeater.h>

/**** END ARGUMENTS MANAGEMENT SECTION ****/

static int TEMPLATE_builtin(WORD_LIST *list)
{
   int  retval = EXECUTION_SUCCESS;

   if (list)
      retval = perform_verb(action_table, actions_count, list);
   else
   {
      (*ERROR_SINK)("Missing arguments for TEMPLATE");
      retval = EX_BADUSAGE;
   }

   return retval;
}

static char *desc_TEMPLATE[] = {
   "TEMPLATE - My Bash handle builtin",
   "",
   "Use Bash builtin handle 'TEMPLATE' to do cool stuff.",
   (char *)NULL
};

struct builtin TEMPLATE_struct = {
   .name      = "TEMPLATE",
   .function  = TEMPLATE_builtin,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_TEMPLATE,
   .short_doc = "TEMPLATE verb [arguments]",
   .handle    = 0
};

#include "local_builtin.h"

#include <stdio.h>

static int TEMPLATE_builtin(WORD_LIST *list)
{
   int  retval = EXECUTION_SUCCESS;

   printf("Hello from Bash builtin 'TEMPLATE'.\n");

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

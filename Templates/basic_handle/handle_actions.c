#include "action_dispatch.h"
#include "handle_actions.h"
#include <stdio.h>

int TEMPLATE_display_help(H_TEMPLATE* handle, ACLONE *args)
{
   int retval = EXECUTION_SUCCESS;
   printf("You're in the help action.\n");
   return retval;
}

int TEMPLATE_declare(H_TEMPLATE* handle, ACLONE *args)
{
   int retval = EXECUTION_SUCCESS;
   printf("You're in the declare action.\n");
   return retval;
}

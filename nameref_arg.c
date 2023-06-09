#include <builtins.h>
#ifndef EXECUTION_FAILURE
#include <shell.h>
#endif
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>
#include <stdlib.h>   // for getenv(), etc.

static int study_variable_by_name(char *nameref)
{
   // printf("About to show all var attributes.\n");
   // show_all_var_attributes(0, 0);
   SHELL_VAR *sval;
   sval = find_variable(nameref);
   if (sval)
   {
      printf("Found variable %s with a value of '%s'\n", nameref, sval->value);
      bind_variable_value(sval, "Replaced value", 0);

      return EXECUTION_SUCCESS;
   }
   else
   {
      printf("Failed to find variable '%s'\n", nameref);
      return EXECUTION_FAILURE;
   }



   // char vbuff[] = "This is the message";
   // SHELL_VAR *sval;
   // sval = builtin_bind_variable(nameref, vbuff, 0);
   // if (sval==0 || readonly_p(sval) || noassign_p(sval))
   // {
   //    printf("Error attempting to bind the variable.\n");
   //    return EXECUTION_FAILURE;
   // }
   // if (sval)
   // {
   //    printf("The value of '%s' is '%s' ('%s')\n", nameref, sval->value, sval->exportstr);
   //    builtin_unbind_variable(nameref);
   //    return EXECUTION_SUCCESS;
   // }

   // printf("Failed to bind to variable %s.\n", nameref);
   // return EXECUTION_FAILURE;
}

static int nameref_arg(WORD_LIST *list)
{
   int retval = EXECUTION_FAILURE;

   // There should be no options, recognized as arguments
   // that begin with a hyphen:
   if (no_options(list))
      return EX_USAGE;

   // We won't need reset_internal_getopt() or internal_getopt()
   // functions here because we're not recognizing any options.

   // Copying this code from bash/examples/loadables/rm.c before
   // I completely understand what it's doing
   list = loptend;

   if (list == 0)
   {
      printf("nameref_arg called with no arguments.\n");
      return EXECUTION_FAILURE;
   }

   WORD_LIST *lptr;
   char *nameref;

   for (lptr=list; lptr!=NULL; lptr = lptr->next)
   {
      nameref = lptr->word->word;

      retval = study_variable_by_name(nameref);

      if (retval)
         break;
   }

   fflush(stdout);

   return retval;
}


static char * desc_nameref_arg[] = {
   "This function demonstrates how to pull",
   "the command arguments off the stack.",
};

struct builtin nameref_arg_struct = {
   .name      = "nameref_arg",
   .function  = nameref_arg,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_nameref_arg,
   .short_doc = "nameref_arg ...",
   .handle    = 0
};

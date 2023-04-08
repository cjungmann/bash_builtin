#include <builtins.h>
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>

static int walk_args(WORD_LIST *list)
{
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
      printf("walk_args called with no arguments.\n");
      return EXECUTION_FAILURE;
   }

   WORD_LIST *lptr;
   for (lptr=list; lptr!=NULL; lptr = lptr->next)
      printf("Argument is '%s'\n", lptr->word->word);

   fflush(stdout);

   return EXECUTION_SUCCESS;
}


static char * desc_walk_args[] = {
   "This function demonstrates how to pull",
   "the command arguments off the stack.",
};

struct builtin walk_args_struct = {
   .name      = "walk_args",
   .function  = walk_args,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_walk_args,
   .short_doc = "walk_args ...",
   .handle    = 0
};

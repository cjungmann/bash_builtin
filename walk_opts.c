#include <builtins.h>
#ifndef EXECUTION_FAILURE
#include <shell.h>
#endif
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.

#include <stdio.h>

static int walk_opts(WORD_LIST *list)
{
   int opt;

   int opt_a=0;
   int opt_b=0;
   int opt_c=0;

   reset_internal_getopt();
   while ((opt = internal_getopt(list, "abc")) != -1)
   {
      switch (opt)
      {
         case 'a':
            opt_a = 1;
            break;
         case 'b':
            opt_b = 1;
            break;
         case 'c':
            opt_c = 1;
            break;
         default:
            printf("Unexpected argument %c.\n", opt);
            printf("A normal program would exit with an error here.\n");
            break;
      }
   }

   printf("After running the walk_opts program,\n");
   printf("  opt_a = %d\n", opt_a);
   printf("  opt_b = %d\n", opt_b);
   printf("  opt_c = %d\n", opt_c);

   fflush(stdout);

   return EXECUTION_SUCCESS;
}


static char * desc_walk_opts[] = {
   "This function demonstrates how to recognize",
   "command line options to the walk_opts function.",
};

struct builtin walk_opts_struct = {
   .name      = "walk_opts",
   .function  = walk_opts,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_walk_opts,
   .short_doc = "walk_opts ...",
   .handle    = 0
};

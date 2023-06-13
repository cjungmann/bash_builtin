#include <builtins.h>
#ifndef EXECUTION_FAILURE
#include <shell.h>
#endif
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.

#include <stdio.h>

void walk_raw_arguments(WORD_LIST *list)
{
   WORD_LIST *ptr = list;
   int count = 0;
   while (ptr)
   {
      printf("%02d: '%s'\n", ++count, ptr->word->word);
      ptr = ptr->next;
   }
}

static int walk_opts(WORD_LIST *list)
{
   int opt;

   int opt_a=0;
   int opt_b=0;
   int opt_c=0;

   const char * __attribute__((unused))dopt = NULL;
   int __attribute__ ((unused)) eopt = -1;

   reset_internal_getopt();
   while ((opt = internal_getopt(list, "abcd:e#")) != -1)
   {
      switch (opt)
      {
         case 'a':
            printf("Setting flag 'a' on.\n");
            opt_a = 1;
            break;
         case 'b':
            printf("Setting flag 'b' on.\n");
            opt_b = 1;
            break;
         case 'c':
            printf("Setting flag 'c' on.\n");
            opt_c = 1;
            break;
         case 'd':
            printf("Setting option 'd' to '%s'\n", list_optarg);
            dopt = list_optarg;
            break;
         case 'e':
            printf("Setting option 'e' to %d.\n", atoi(list_optarg));
            eopt = atoi(list_optarg);
            break;
         case '?':
            printf("Bad argument type.\n");
            goto bad_arg_exit;
         default:
            printf("Unexpected argument %c.\n", opt);
            printf("A normal program would exit with an error here.\n");
            break;
      }
   }

  bad_arg_exit:

   printf("After running the walk_opts program,\n");
   printf("  opt_a = %d\n", opt_a);
   printf("  opt_b = %d\n", opt_b);
   printf("  opt_c = %d\n", opt_c);

   walk_raw_arguments(list);

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

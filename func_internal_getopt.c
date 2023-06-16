// -*- mode:c -*-

/***
 * Todo:
 * - Update `FNAME()` function, delete or fill out as needed
 * - Add detailed instructions to desc_FNAME string array.
 * - Compose more helpful .short_doc string
 * - Finally, replace-string func_internal_getopt with your builtin function's name.
 ***/

#include <builtins.h>

// Prevent multiple inclusion of shell.h:
#ifndef EXECUTION_FAILURE
#include <shell.h>
#endif

#include <builtins/bashgetopt.h>  // for internal_getopt(), etc
#include <builtins/common.h>      // for no_options()

#include <stdio.h>

static int func_internal_getopt(WORD_LIST *list)
{
   int retval = EXECUTION_SUCCESS;

   // Show usage if no options are set
   // if (no_options(list))
   //    return EX_USAGE;

   reset_internal_getopt();

   int a_flag = 0;
   const char *b_string = NULL;
   int c_int = 0;
   const char *d_optional = "NOT SET";

   int opt = 0;
   while ( (opt = internal_getopt(list, "ab:c#d;")) != -1)
   {
      switch(opt)
      {
         case 'a':
            a_flag = 1;
            break;
         case 'b':
            b_string = list_optarg;
            break;
         case 'c':
            c_int = atoi(list_optarg);
            break;
         case 'd':
            if (list_optarg)
               d_optional = list_optarg;
            else
               d_optional = "INVOKED, no value";
            break;
         case '?':
            printf("Argument data-type failure.\n");
            retval = EXECUTION_FAILURE;
            goto bypass_output;
            break;
      }
   }

   printf("    a_flag is '%d'.\n", a_flag);
   printf("  b_string is '%s'.\n", b_string);
   printf("     c_int is '%d'.\n", c_int);
   printf("d_optional is '%s'.\n", d_optional);

  bypass_output:

   return retval;
}

static char *desc_func_internal_getopt[] = {
   "First of several lines providing comprehensive help.",
   (char*)NULL     // end of array marker
};

struct builtin func_internal_getopt_struct = {
   .name      = "func_internal_getopt",
   .function  = func_internal_getopt,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_func_internal_getopt,
   .short_doc = "func_internal_getopt is cool",
   .handle    = 0
};

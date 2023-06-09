#include <builtins.h>
#ifndef EXECUTION_FAILURE
#include <shell.h>
#endif
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>


void replace_array(const char *name)
{
   SHELL_VAR *svar;
   svar = find_variable(name);
   if (svar)
   {
      ARRAY *array = array_cell(svar);
      array_flush(array);

      array_insert(array, 0, "un");
      array_insert(array, 1, "deux");
      array_insert(array, 2, "trois");
      array_insert(array, 3, "quatre");
      array_insert(array, 4, "cinq");
      array_insert(array, 5, "six");
   }
}

static int write_array(WORD_LIST *list)
{
   // There should be no options, recognized as arguments
   // that begin with a hyphen:
   if (no_options(list))
      return EX_USAGE;

   list = loptend;

   if (list == 0)
   {
      builtin_usage();
      return EX_USAGE;
   }

   WORD_LIST *lptr = list;
   while (lptr != NULL)
   {
      const char *word = lptr->word->word;
      if (legal_identifier(word))
         replace_array(word);
      else
         printf("Name ref '%s' is not a legal identifier.\n", word);

      lptr = lptr->next;
   }

   return EXECUTION_SUCCESS;
}

static char *desc_write_array[] = {
   "This function will accept the name of an array variable,",
   "which will be cleared out and filled with new content."
};


struct builtin write_array_struct = {
   .name      = "write_array",
   .function  = write_array,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_write_array,
   .short_doc = "write_array array_name",
   .handle    = 0
};

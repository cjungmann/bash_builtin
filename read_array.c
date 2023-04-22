#include <builtins.h>
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>


void dump_array(const char *name)
{
   SHELL_VAR *svar;
   svar = find_variable(name);
   if (svar)
   {
      ARRAY *loc_array;
      loc_array = array_cell(svar);

      int el_count = 0;
      el_count = array_num_elements(loc_array);
      printf("The array, %s, contains %d elements.\n", name, el_count);

      ARRAY_ELEMENT *el;
      el = element_forw(loc_array->head);

      // Note the following end test: test for NULL will not work
      while(el != loc_array->head)
      {
         printf("   %s\n", element_value(el));
         el = element_forw(el);
      }
   }
   else
      printf("Failed to find variable '%s'\n", name);
}


static int read_array(WORD_LIST *list)
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
      dump_array((lptr->word->word));
      lptr = lptr->next;
   }

   return EXECUTION_SUCCESS;
}

static char *desc_read_array[] = {
   "This function accepts a variable name of an array.",
   "Succeess in this code is accomplished if the name",
   "can be attached and the elements can be printed.",
   (char*)NULL
};


struct builtin read_array_struct = {
   .name      = "read_array",
   .function  = read_array,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_read_array,
   .short_doc = "read_array array_name",
   .handle    = 0
};

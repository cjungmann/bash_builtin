#include <builtins.h>
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>

void name_svar(SHELL_VAR *svar)
{
   int attr = svar->attributes;
   char *name = svar->name;

   if (attr & att_local)
      printf("%s is a local variable.\n", name);
   else
      printf("%s is NOT a local variable.\n", name);

   if (attr & att_array)
      printf("%s is an array.\n", name);
   else if (attr & att_function)
      printf("%s is a function.\n", name);
   else if (attr & att_integer)
      printf("%s is an integer.\n", name);
   else if (attr & att_assoc)
      printf("%s is an associative array.\n", name);
   else if (attr & att_nameref)
      printf("%s is a nameref.\n", name);
   else
      printf("%s is an unspecified type.\n", name);
}

void internal_identify(const char *name)
{
   SHELL_VAR *svar;
   svar = find_variable(name);
   if (svar)
      name_svar(svar);
   else
      printf("%s is not a variable.\n", name);
   printf("\n");
}

static int identify_name(WORD_LIST *list)
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
         internal_identify(word);
      else
         printf("Name ref '%s' is not a legal identifier.\n", word);

      lptr = lptr->next;
   }

   return EXECUTION_SUCCESS;
}

static char *desc_identify_name[] = {
   "Given any name, this function determines if it is defined"
   "in the current context, and if defined, what kind of an"
   "object the name identifies."
};

struct builtin identify_name_struct = {
   .name      = "identify_name",
   .function  = identify_name,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_identify_name,
   .short_doc = "identify_name array_name",
   .handle    = 0
};


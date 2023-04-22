#include <builtins.h>
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc
#include <builtins/common.h>      // for no_options()

#include <stdio.h>

#include <unistd.h>      // for STDERR_FILENO

void display_attribute(const char *name, int value)
{
   const char *color = (value ? "\033[38;2;255;128;0m" : "");
   printf("  %s%14s: %s\033[m\n", color, name, value?"on":"off");
}

void enumerate_variable_attributes(SHELL_VAR *svar)
{
   const char *color = "\033[38;2;192;96;255m";
   printf("\nshell variable %s%s\033[m:\n", color, svar->name);

   // User-visible attributes
   display_attribute("att_exported", (svar->attributes & att_exported));
   display_attribute("att_readonly", (svar->attributes & att_readonly));
   display_attribute("att_array", (svar->attributes & att_array));
   display_attribute("att_function", (svar->attributes & att_function));
   display_attribute("att_integer", (svar->attributes & att_integer));
   display_attribute("att_local", (svar->attributes & att_local));
   display_attribute("att_assoc", (svar->attributes & att_assoc));
   display_attribute("att_trace", (svar->attributes & att_trace));
   display_attribute("att_uppercase", (svar->attributes & att_uppercase));
   display_attribute("att_lowercase", (svar->attributes & att_lowercase));
   display_attribute("att_capcase", (svar->attributes & att_capcase));
   display_attribute("att_nameref", (svar->attributes & att_nameref));

   // Internal bookkeeping attributes
   // display_attribute("att_invisible", (svar->attributes & att_invisible));
   // display_attribute("att_nounset", (svar->attributes & att_nounset));
   // display_attribute("att_noassign", (svar->attributes & att_noassign));
   // display_attribute("att_imported", (svar->attributes & att_imported));
   // display_attribute("att_special", (svar->attributes & att_special));
   // display_attribute("att_nofree", (svar->attributes & att_nofree));
   // display_attribute("att_regenerate", (svar->attributes & att_regenerate));

   // Internal scoping attributes
   // display_attribute("att_tempvar", (svar->attributes & att_tempvar));
   // display_attribute("att_propagate", (svar->attributes & att_propagate));
}

static int show_attributes(WORD_LIST *list)
{
   if (list == 0)
   {
      builtin_usage();
      return EX_USAGE;
   }

   WORD_LIST *lptr = list;
   SHELL_VAR *svar;
   const char *name;
   while (lptr != NULL)
   {
      name = lptr->word->word;

      svar = find_variable(name);
      if (!svar)
         svar = find_function(name);

      if (svar)
         enumerate_variable_attributes(svar);
      else
         dprintf(STDERR_FILENO, "Failed to recognize variable '%s'\n", name);

      lptr = lptr->next;
   }

   return EXECUTION_SUCCESS;
}

static char* desc_show_attributes[] = {
   "Pass the names of variables you want identified.",
   (char*)NULL
};


struct builtin show_attributes_struct = {
   .name      = "show_attributes",
   .function  = show_attributes,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_show_attributes,
   .short_doc = "show_attributes array_name",
   .handle    = 0
};


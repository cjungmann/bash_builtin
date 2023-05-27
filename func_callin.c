#include <builtins.h>
#include <command.h>              // for COMMAND.flags, among other things
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>

#include "word_list_stack.h"

// Copied from execute_cmd.h from bash source code, using code in command.h.
extern int execute_command PARAMS((COMMAND *));
extern int execute_shell_function PARAMS((SHELL_VAR *, WORD_LIST *));

// Copied from bash_src/externs.h, function code in parse.y, not found
extern COMMAND *parse_string_to_command PARAMS((char *, int));
// Alternative suggested in ChangeLog is xparse_dolparen (dollar parentheses)

// Let's try to enable the second command, bundle it and return it to the caller:
extern int enable_shell_command(char* name, int disable_p);


SHELL_VAR *make_callback_function(void)
{
   char command[] = "echo \"hello from make_callback_function.\";";
   COMMAND *cmd = parse_string_to_command(command, 0);
   if (cmd)
      printf("Successfully created a command.\n");
   else
      printf("Disgracefully failed to creat a command.\n");

   // char *fname = xparse_dolparen("callin", command, 0, 0);
   // if (fname)
   //    printf("fname is %s.\n", fname);
   // else
   //    printf("fname was not generated for some unknown reason.\n");

   return NULL;
}

int call_with_execute_command(const char *function_name)
{
   SHELL_VAR *sv = make_callback_function();

   // Array of strings to transform into WORD_LIST
   const char *words[]={
      function_name,
      "myfunc",
      NULL
   };

   // Prepare arguments to WL_APPEND macro
   WORD_LIST *tail=NULL, *head=NULL;
   const char **strarray = words;

   WL_WALK(head, tail, strarray);

   if (sv)
      WL_APPEND(tail,"newfunc");

   int flags = CMD_INHIBIT_EXPANSION | CMD_STDPATH;

   COMMAND *command;
   command = make_bare_simple_command();
   command->value.Simple->words = head;
   command->value.Simple->redirects = (REDIRECT *)NULL;
   command->value.Simple->flags = flags;
   command->flags = flags;

   return execute_command(command);
}


static int func_callin(WORD_LIST *list)
{
   printf("Entered 'func_callin'\n");
   if (list == NULL)
   {
      builtin_usage();
      return EX_USAGE;
   }

   enable_shell_command(NULL, 0);

   const char *callback_function = list->word->word;
   SHELL_VAR *svar = find_function(callback_function);
   if (svar)
      call_with_execute_command(callback_function);
   else
      printf("FAILED TO FIND function \x1b[32;1m%s\x1b[m.\n", callback_function);

   return EXECUTION_SUCCESS;
}


static char *desc_func_callin[] = {
   "This is a experiment aspiring to create function within",
   "a builtin that can be returned to a calling function to",
   "enable the calling script to initiate an action rather than",
   "wait for the function to call out.",
   (char *)NULL
};

struct builtin func_callin_struct = {
   .name      = "func_callin",
   .function  = func_callin,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_func_callin,
   .short_doc = "func_callin return_variable",
   .handle    = 0
};


static int func_callin_callback(WORD_LIST *list)
{
   return EXECUTION_SUCCESS;
}

static char *desc_func_callin_callback[] = {
   "This function should only be handed to a ascript by func_callin.",
   (char*)NULL
};


struct builtin func_callin_callback_struct = {
   .name      = "func_callin_callback",
   .function  = func_callin_callback,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_func_callin_callback,
   .short_doc = "func_callin_callback return_variable",
   .handle    = 0
};

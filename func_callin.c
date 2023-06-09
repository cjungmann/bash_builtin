#include <builtins.h>
#include <command.h>              // for COMMAND.flags, among other things
#ifndef EXECUTION_FAILURE
#include <shell.h>
#endif
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>
#include <stdarg.h>

#include "word_list_stack.h"

// Mysterious "line" value of COMMAND object: -538976289
// Is this number random or repeated?

// Copied from execute_cmd.h from bash source code, using code in command.h.
extern int execute_command PARAMS((COMMAND *));
extern int execute_shell_function PARAMS((SHELL_VAR *, WORD_LIST *));

SHELL_VAR *make_callback_function(void)
{
   // Make WORD_LIST with a command and its arguments
   WORD_LIST *head = NULL, *tail = NULL;
   const char *wlist[] = {
      "callout_target",
      "imaginary",
      "pretend",
      "bogus",
      // "echo",
      // "Hello from the make_callback_function",
      NULL
   };

   const char **wlarray = wlist;
   WL_WALK(head, tail, wlarray);

   // Make a COMMAND object, identify as `cm_simple` (command.h, enum command_type)
   COMMAND *cmd = (COMMAND*)alloca(sizeof(COMMAND));
   memset(cmd, 0, sizeof(COMMAND));
   cmd->type = cm_simple;
   cmd->value.Simple = (SIMPLE_COM*)alloca(sizeof(SIMPLE_COM));
   memset(cmd->value.Simple, 0, sizeof(SIMPLE_COM));
   cmd->value.Simple->words = head;

   execute_command(cmd);


   return NULL;
}

int call_with_execute_command(const char *function_name, ...)
{
   SHELL_VAR *sv = find_function(function_name);

   if (sv == NULL)
   {
      fprintf(stderr, "FAILED TO FIND function \x1b[32;1m%s\x1b[m.\n", function_name);
      return EXECUTION_FAILURE;
   }

   WORD_LIST *head = NULL, *tail = NULL;

   WL_APPEND(tail, function_name);
   head = tail;

   // Add any extra parameters to the WORD_LIST:
   va_list args_list;
   va_start(args_list, function_name);
   const char *val;
   while ((val = va_arg(args_list, const char*)))
      WL_APPEND(tail, val);
   va_end(args_list);

   int flags = CMD_INHIBIT_EXPANSION | CMD_STDPATH;

   COMMAND *command;
   command = make_bare_simple_command();
   command->value.Simple->words = head;
   command->value.Simple->redirects = (REDIRECT *)NULL;
   command->flags = command->value.Simple->flags = flags;

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

    /*****************************************
    * Begin constructing COMMAND for callin
    *****************************************/

   // Begin constructing stack-based SHELL_VAR* containing
   // a stack-based COMMAND.
   // Make WORD_LIST with a command and its arguments
   const char *bound_function_name = "CALLIN_CALLBACK";
   WORD_LIST *head = NULL, *tail = NULL;
   const char *wlist[] = {
      "callout_target",
      bound_function_name,
      "imaginary",
      "pretend",
      "bogus",
      // "echo",
      // "Hello from the make_callback_function",
      NULL
   };
   const char **wlarray = wlist;
   WL_WALK(head, tail, wlarray);

   // Make a COMMAND object, identify as `cm_simple` (command.h, enum command_type)
   COMMAND *cmd = (COMMAND*)alloca(sizeof(COMMAND));
   memset(cmd, 0, sizeof(COMMAND));
   cmd->type = cm_simple;
   cmd->value.Simple = (SIMPLE_COM*)alloca(sizeof(SIMPLE_COM));
   memset(cmd->value.Simple, 0, sizeof(SIMPLE_COM));
   cmd->value.Simple->words = head;

   SHELL_VAR* svar_command = bind_function(bound_function_name, cmd);
    /*****************************************
    * DONE constructing COMMAND for callin
    *****************************************/

   const char *callback_function = list->word->word;
   if (svar_command)
   {
      printf("Made a nice SVAR variable of type COMMAND of name %s.\n", bound_function_name);
      return call_with_execute_command(callback_function, bound_function_name, NULL);
   }
   else
   {
      fprintf(stderr, "Failed to bind_command for function %s.\n", callback_function);
      return EXECUTION_FAILURE;
   }

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

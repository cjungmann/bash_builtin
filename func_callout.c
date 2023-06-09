#include <builtins.h>
#include <command.h>              // for COMMAND.flags, among other things
#ifndef EXECUTION_FAILURE
#endif
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include "word_list_stack.h"

#include <stdio.h>

// Copied from execute_cmd.h from bash source code, using code in command.h.
extern int execute_command PARAMS((COMMAND *));
extern int execute_shell_function PARAMS((SHELL_VAR *, WORD_LIST *));

// prototype for function found in func_callback.c
extern int func_execute_shell_function(WORD_LIST *list);

// Prototypes from source file utilities.c
void bb_free_word_list(WORD_LIST *wl);
WORD_LIST *bb_append_string_to_word_list(WORD_LIST *wl, const char *string);
int bb_make_word_list(WORD_LIST **wl, ...);

static int func_callout(WORD_LIST *list)
{
   int retval = EXECUTION_FAILURE;

   WORD_LIST *exec_params = NULL;
   WORD_LIST *tail = NULL;

   // The first argument should be the name of the callback function:
   const char *func_to_call = list->word->word;
   if (!func_to_call)
   {
      builtin_usage();
      retval = EX_USAGE;
      goto exit;
   }

   // Start building the arguments WORD_LIST for the call:
   SHELL_VAR *svFunc = find_function(func_to_call);
   if (svFunc)
   {
      WL_APPEND(tail, func_to_call);
      exec_params = tail;
   }
   else
   {
      builtin_error("Unable to find shell function %s.", func_to_call);
      goto exit;
   }

   // Make a shell variable to pass as nameref
   char var_to_pass[] = "fc_var";
   SHELL_VAR *newvar = make_local_variable(var_to_pass, 0);
   if (newvar)
   {
      // Required argument, the name of a SHELL_VAR into which the
      // callback function will store values that we'll consume here.
      WL_APPEND(tail, var_to_pass);
      if (tail == NULL)
      {
         dispose_variable(newvar);
         goto exit;
      }

      // Optional argument passed to the callback function if
      // submitted to this builtin function
      const char *aname = NULL;
      if (list->next && (aname=list->next->word->word))
      {
         WL_APPEND(tail, aname);
         if (tail == NULL)
         {
            dispose_variable(newvar);
            goto exit;
         }
      }

      // This is where the callback occurs.  It will keep invoking
      // the callback while the callback returns SUCCESS (0).
      int count = 0;
      while (!(retval = execute_shell_function(svFunc, exec_params)))
      {
         printf("%2d: called '%s', which returned '%s'.\n",
                ++count,
                func_to_call,
                newvar->value);
      }
   }

  exit:
   return retval;
}

static char *desc_func_callout[] = {
   "Call this function with the name of a shell function."
   "The shell function should create a nameref variable from"
   "the first argument passed with the call to `func_callout`."
   "func_callout will call the function with the name of a"
   "shell variable allocated in func_callout.  When the function"
   "returns, func_callout will print the value of the function."
};

struct builtin func_callout_struct = {
   .name      = "func_callout",
   .function  = func_callout,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_func_callout,
   .short_doc = "func_callout array_name",
   .handle    = 0
};

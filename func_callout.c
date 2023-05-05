#include <builtins.h>
#include <command.h>              // for COMMAND.flags, among other things
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

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

   const char *func_to_call = list->word->word;
   if (!func_to_call)
   {
      builtin_usage();
      retval = EX_USAGE;
      goto exit;
   }

   SHELL_VAR *svFunc = find_function(func_to_call);
   if (svFunc)
      exec_params = bb_append_string_to_word_list(exec_params, func_to_call);
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
      if (!bb_append_string_to_word_list(exec_params, var_to_pass))
      {
         dispose_variable(newvar);
         goto exit;
      }

      const char *aname = NULL;
      if (list->next && (aname=list->next->word->word))
      {
         if (!bb_append_string_to_word_list(exec_params, aname))
         {
            dispose_variable(newvar);
            goto exit;
         }
      }

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
   bb_free_word_list(exec_params);
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

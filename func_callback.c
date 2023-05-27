#include <builtins.h>
#include <command.h>              // for COMMAND.flags, among other things
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>

// Copied from execute_cmd.h from bash source code, using code in command.h.
extern int execute_command PARAMS((COMMAND *));
extern int execute_shell_function PARAMS((SHELL_VAR *, WORD_LIST *));

static int func_execute_command(WORD_LIST *list)
{
   int retval = EXECUTION_FAILURE;

   int flags = CMD_INHIBIT_EXPANSION | CMD_STDPATH;
   // flags |= CMD_NO_FUNCTIONS | CMD_COMMAND_BUILTIN;

   // // Found this begin_unwind_frame() in `command.def` in bash
   // // source code, despite the fact that execute_command() includes
   // // another level of unwinding frames. I'm assuming that we dont'
   // // need it, but leaving it here to remind me if I need it later.
   // begin_unwind_frame("func_execute_command");

   printf("Using \x1b[32;1mexecute_command\x1b[m "
          "to call shell function \x1b[32;1m%s\x1b[m:\n",
          list->word->word);

   COMMAND *command;
   command = make_bare_simple_command();
   command->value.Simple->words = (WORD_LIST*)copy_word_list(list);
   command->value.Simple->redirects = (REDIRECT *)NULL;
   command->value.Simple->flags = flags;
   command->flags = flags;

   retval = execute_command(command);

   if (retval)
      printf("Attempt \x1b[31;mFAILED\x1b[m with error %d.\n", retval);
   else
      printf("Attempt \x1b[32;1mSUCCEEDED\x1b[m.\n");

   // // Mate to begin_unwind_frame above
   // run_unwind_frame("func_execute_command");

   return retval;
}

static int func_execute_shell_function(WORD_LIST *list)
{
   int retval = EXECUTION_FAILURE;
   const char *fname = list->word->word;

   printf("Using \x1b[32;1mexecute_shell_function\x1b[m "
          "to call shell function \x1b[32;1m%s\x1b[m:\n",
          fname);

   SHELL_VAR *svar = find_function(fname);
   if (svar)
   {
      retval = execute_shell_function(svar, list);

      if (retval)
         printf("Attempt \x1b[31;mFAILED\x1b[m with error %d.\n", retval);
      else
         printf("Attempt \x1b[32;1mSUCCEEDED\x1b[m.\n");
   }
   else
      printf("\x1b[31;1mFAILED\x1b[m: %s is an unknown function.\n", fname);

   return retval;
}

static int func_callback(WORD_LIST *list)
{
   int retval = EXECUTION_FAILURE;

   // list = loptend;

   if (list == 0)
   {
      builtin_usage();
      return EX_USAGE;
   }

   const char *function_name = list->word->word;
   printf("\n\x1b[32;1m%s\x1b[m to be called from C function func_callback\n", function_name);

   retval = func_execute_shell_function(list);
   retval = func_execute_command(list);

   return retval;
}

static char *desc_func_callback[] = {
   "This is the first attempt to call a shell function from the"
   "C code.  This is the last conceptual step before starting a"
   "project to create a useful Bash tool."
};

struct builtin func_callback_struct = {
   .name      = "func_callback",
   .function  = func_callback,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_func_callback,
   .short_doc = "func_callback array_name",
   .handle    = 0
};


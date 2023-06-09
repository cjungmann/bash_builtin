#include <builtins.h>
#ifndef EXECUTION_FAILURE
#include <shell.h>
#endif
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc
#include <builtins/common.h>      // for no_options()

#include <stdio.h>

#include <unistd.h>

static int hellow(WORD_LIST *list)
{
   if (no_options(list))
      return EX_USAGE;

   pid_t  pid = getpid();
   printf("Hello, world from process %d.\n", pid);
   fflush(stdout);
   return EXECUTION_SUCCESS;
}

static char* desc_hello_world[] = {
   "This is the long documentation",
   "",
   "It can span several lines.",
   "",
   "What do you think of it",
   (char*)NULL
};


struct builtin hellow_struct = {
   "hellow",             // string used to call command
   hellow,               // pointer to function (no quotes)
   BUILTIN_ENABLED,      // initial enabled state
   desc_hello_world,
   "brief doc",
   0
};


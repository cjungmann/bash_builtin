// -*- mode:c -*-

/***
 * Todo:
 * - Update `FNAME()` function, delete or fill out as needed
 * - Add detailed instructions to desc_FNAME string array.
 * - Compose more helpful .short_doc string
 * - Finally, replace-string func_iface with your builtin function's name.
 ***/

#include <builtins.h>
#ifndef EXECUTION_FAILURE
#include <shell.h>
#endif
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc
#include <builtins/common.h>      // for no_options()

#include <string.h>
#include <stdio.h>

#include <stdarg.h>
#include "word_list_stack.h"

// Prototype of undocumented Bash command
extern int execute_command PARAMS((COMMAND *));

int bb_make_unique_name(char *buffer, int bufflen, const char *stem)
{
   int len = strlen(stem);
   if (bufflen < len + 3)
      return 0;

   char **vars = all_variables_matching_prefix(stem);
   char **ptr = vars;
   int cur_val,  max_val = 0;
   while (*ptr)
   {
      const char *str = *ptr;
      cur_val = atoi(&str[len]);
      if (cur_val > max_val)
         max_val = cur_val;

      ++ptr;
   }

   snprintf(buffer, bufflen, "%s_%02d", stem, max_val+1);
   return 1;
}

/**
 * @brief Construct and execute a COMMAND with SIMPLE_COM from
 *        function arguments.
 *
 * The only required arguments are the function name and a
 * list-terminating NULL.
 *
 * The function uses stack memory, so freeing memory at the end
 * is handled by exiting the stack frame.
 *
 * @return EXECUTION_FAILURE for any problem, otherwise the
 *         exit code of execute_command()
 */
int bb_execute_command(const char *func_name, ...)
{
   int retval = EXECUTION_FAILURE;

   // Make the WORD_LIST
   WORD_LIST *head = NULL, *tail = NULL;
   WL_APPEND(tail, func_name);
   if (tail)
   {
      head = tail;
      va_list list_args;
      va_start(list_args, func_name);
      const char *arg;
      while((arg = va_arg(list_args, const char*)))
         WL_APPEND(tail, arg);
      va_end(list_args);

      SIMPLE_COM *scmd = (SIMPLE_COM*)alloca(sizeof(SIMPLE_COM));
      if (scmd)
      {
         memset(scmd, 0, sizeof(SIMPLE_COM));
         scmd->words = head;

         COMMAND *cmd = (COMMAND*)alloca(sizeof(COMMAND));
         if (cmd)
         {
            memset(cmd, 0, sizeof(COMMAND));
            cmd->type = cm_simple;
            cmd->value.Simple = scmd;

            retval = execute_command(cmd);
         }
      }
   }

   return retval;
}

struct iface_instance;
typedef struct iface_instance IFI;

typedef int (*if_setter)(IFI *iface, const char *new_name);
typedef const char * (*if_getter)(IFI *iface);

struct iface_instance {
   char buffer[128];
   if_setter setter;
   if_getter getter;
};

int iface_setter(IFI *iface, const char *new_name)
{
   int len = strlen(new_name);
   if (len-1 < sizeof(iface->buffer))
   {
      strcpy(iface->buffer, new_name);
      return 1;
   }

   return 0;
}

const char *iface_getter(IFI *iface)
{
   return iface->buffer;
}


int init_instance(IFI* instance, const char *name)
{
   memset(instance, 0, sizeof(IFI));
   instance->setter = iface_setter;
   instance->getter = iface_getter;

   return (*instance->setter)(instance, name);
}


static int func_iface(WORD_LIST *list)
{
   int retval = EXECUTION_FAILURE;
   const char *func_name = NULL;

   // SHELL_VAR *svar_inst = NULL;

   int opt;
   while (-1 != (opt = internal_getopt(list, "c:")))
   {
      switch(opt)
      {
         case 'c':
            func_name = list_optarg;
            break;
         default:
            break;
      }
   }

   // ONLY IF USING `internal_getopt`, reset the list
   // to process any remaining non-option arguments:
   list = loptend;

   if (func_name)
   {
      const static char instance_name_stem[] = "BB_IFACE_INSTANCE";
      int bufflen = strlen(instance_name_stem) + 4;  // _00\0
      char *handle_name = (char*)alloca(bufflen);

      if (bb_make_unique_name(handle_name, bufflen, instance_name_stem))
      {
         // Make an IFI instance
         IFI iface;
         init_instance(&iface, "starting value");

         int attributes = att_special | att_nofree | att_noassign | att_nounset | att_readonly;

         SHELL_VAR *svar_iface = bind_variable(handle_name, (char*)NULL, 0);
         if (svar_iface)
         {
            svar_iface->name = handle_name;
            svar_iface->value = (char*)&iface;
            svar_iface->exportstr = handle_name;
            VSETATTR(svar_iface, attributes);
         }

         retval = bb_execute_command(func_name, handle_name, "bogus", NULL);

         /*
          * NULL-out all SHELL_VAR member pointers that point to
          * stack-based memory.  This will prevent Bash from trying
          * to free them again.
          */

         // This NULLing is required because it's an unrecognized data structure:
         svar_iface->value = (char*)NULL;

         // These two NULLings could be avoided it we had used
         // savestring() to assign SHELL_VAR::name and SHELL_VAR:exportstr:
         svar_iface->name = svar_iface->exportstr = (char*)NULL;

         // Remove name from variables hash table:
         unbind_variable(handle_name);

         // Prevent future use of now invalid handle
         svar_iface = NULL;
      }
      else
         printf("Failed to generate unique name for called.\n");
   }
   else
      printf("Failed to find variable.\n");

   return retval;
}

static char *desc_func_iface[] = {
   "First of several lines providing comprehensive help.",
   (char*)NULL     // end of array marker
};

struct builtin func_iface_struct = {
   .name      = "func_iface",
   .function  = func_iface,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_func_iface,
   .short_doc = "func_iface is cool",
   .handle    = 0
};


static int func_iface_callback(WORD_LIST *list)
{
   SHELL_VAR *svar;
   svar = find_variable(list->word->word);
   if (svar)
   {
      IFI *instance = (IFI*)svar->value;

      if (list->next)
         (*instance->setter)(instance, list->next->word->word);
      else
      {
         const char *value = (*instance->getter)(instance);
         printf("value is '%s'\n", value);
      }

      return EXECUTION_SUCCESS;
   }

   return EXECUTION_FAILURE;
}

static char *desc_func_iface_callback[] = {
   "First of several lines providing comprehensive help.",
   (char*)NULL     // end of array marker
};

struct builtin func_iface_callback_struct = {
   .name      = "func_iface_callback",
   .function  = func_iface_callback,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_func_iface_callback,
   .short_doc = "func_iface_callback is cool",
   .handle    = 0
};

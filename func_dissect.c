#include <builtins.h>
#include <command.h>              // for COMMAND.flags, among other things
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>
#include <stdarg.h>

#include "word_list_stack.h"

// Copied from execute_cmd.h from bash source code, using code in command.h.
extern int execute_command PARAMS((COMMAND *));
extern int execute_shell_function PARAMS((SHELL_VAR *, WORD_LIST *));

const char *col_warm_yellow="\x1b[33;1m";
const char *col_cool_blue="\x1b[38;2;64;160;255m";
const char *col_off="\x1b[m";

const char *stringify_type(int type)
{
   switch(type)
   {
      case cm_for:          return "FOR_COM";        // loop with limits
      case cm_case:         return "CASE_COM";       // value, patterns with associated actions
      case cm_while:        return "WHILE_COM";      // condition for end, group containing task
      case cm_if:           return "IF_COM";         // condition, true and false branches
      case cm_simple:       return "SIMPLE_COM";     // simple WORD_LIST, command + arguments
      case cm_select:       return "SELECT_COM";     // makes selection "dialog"
      case cm_connection:   return "CONNECTION";     // connects command to subsequent comment
      case cm_function_def: return "FUNCTION_DEF";   // contains name and group
      case cm_group:        return "GROUP_COM";      // commands enclosed in curly-braces
      case cm_cond:         return "COND_COM";       // condition for while, if, until, etc
      case cm_arith_for:    return "ARITH_FOR_COM";  // arithmetic within (( ))

         // The following are not implemented yet
      case cm_arith:        return "ARITH_COM";
      case cm_subshell:     return "SUBSHELL_COM";
      case cm_coproc:       return "COPROC_COM";
      case cm_until:        return "cm_until";
      default:              return "unknown type";
   }
}

const char *stringify_command_type(const COMMAND *cmd)
{
   return stringify_type(cmd->type);
}

void indent_level(int level)
{
   for (int i=0; i<level; ++i)
      printf(". ");
}

void indent_word_list(int level, const WORD_LIST *wl)
{
   while(wl)
   {
      indent_level(level);
      printf("%s\n", wl->word->word);
      wl = wl->next;
   }
}

void print_hilighted_word_list(const WORD_LIST *wl)
{
   static const char *bg_on = "\x1b[44m";
   static const char *bg_off = "\x1b[m";
   while (wl)
   {
      printf("%s%s%s ", bg_on, wl->word->word, bg_off);
      wl = wl->next;
   }
}

void print_info_line(int level, COMMAND *cmd, const char *label)
{
   indent_level(level);

   printf("%s%s%s:%d %s%s%s\n",
          col_warm_yellow, label, col_off,
          level,
          col_cool_blue, stringify_command_type(cmd), col_off);
}

// Foreward declaration for entry-function
void dissect_command(int level, COMMAND *cmd);

void dissect_simple(int level, COMMAND *cmd)
{
   SIMPLE_COM *simple = cmd->value.Simple;
   indent_level(level);
   print_hilighted_word_list(simple->words);
   printf("\n");
}

void dissect_group(int level, COMMAND *cmd)
{
   GROUP_COM *group = cmd->value.Group;
   print_info_line(level, group->command, "command");
   dissect_command(level, group->command);
}

void dissect_for(int level, COMMAND *cmd)
{
   FOR_COM *for_com = cmd->value.For;
   const char *for_name = for_com->name->word;

   print_info_line(level, for_com->action, "For statement");

   ++level;
   printf("name is %s\n", for_name);
   indent_level(level);
   print_hilighted_word_list(for_com->map_list);
   printf("\n");

   dissect_command(level, for_com->action);
}

void dissect_case_pattern_list(int level, PATTERN_LIST *plist)
{
   WORD_LIST *wl = plist->patterns;
   while (wl)
   {
      indent_level(level);
      printf("pattern: %s%s%s\n", col_cool_blue, wl->word->word, col_off);
      wl = wl->next;
   }
   dissect_command(level, plist->action);
}

void dissect_case(int level, COMMAND *cmd)
{
   CASE_COM *case_com = cmd->value.Case;
   indent_level(level);
   printf("case_source: %s\n", case_com->word->word);
   PATTERN_LIST *pl = case_com->clauses;
   ++level;
   while (pl)
   {
      dissect_case_pattern_list(level, pl);
      pl = pl->next;
   }
   
}

void dissect_if(int level, COMMAND *cmd)
{
   IF_COM *if_com = cmd->value.If;
   print_info_line(level, if_com->test, "test");
   dissect_command(level, if_com->test);
   print_info_line(level, if_com->true_case, "if-true");
   dissect_command(level, if_com->true_case);
   print_info_line(level, if_com->false_case, "if_false");
   dissect_command(level, if_com->false_case);
}

void dissect_while(int level, COMMAND *cmd)
{
   WHILE_COM *while_com = cmd->value.While;
   print_info_line(level, while_com->test, "test");
   dissect_command(level, while_com->test);
   print_info_line(level, while_com->action, "action");
   dissect_command(level, while_com->action);
}

const char *CONDITION_TYPE[]={
   "Undefined",
   "AND",
   "OR",
   "UNARY",
   "BINARY",
   "TERM",
   "EXPR",
   NULL
};

/**
 * @brief Dissect version taking COND_COM to support recursion
 */
void dissect_cond_com(int level, COND_COM* cond_com)
{
   indent_level(level);
   printf("Condition command\n");
   ++level;
   indent_level(level);
   printf("%s (%s)\n", CONDITION_TYPE[cond_com->type], cond_com->op->word);
   if (cond_com->left)
      dissect_cond_com(level, cond_com->left);
   if (cond_com->right)
      dissect_cond_com(level, cond_com->right);
}

/**
 * @bbrief Dissect version taking COMMAND to be called by @ref dissect_command
 *
 * Calls @ref dissect_cond_com to do the real work
 */
void dissect_cond(int level, COMMAND *cmd)
{
   dissect_cond_com(level, cmd->value.Cond);
}

void dissect_function_def(int level, COMMAND *cmd)
{
   FUNCTION_DEF *function_def = cmd->value.Function_def;
   indent_level(level);
   printf("Dissecting function def for %s.\n", function_def->name->word);
   dissect_command(level, function_def->command);
}

void dissect_arith(int level, COMMAND *cmd)
{
   ARITH_COM *arith_com = cmd->value.Arith;
   WORD_LIST *wl = arith_com->exp;
   while (wl)
   {
      indent_level(level);
      printf(wl->word->word);
      wl = wl->next;
   }
}

void dissect_arith_for(int level, COMMAND *cmd)
{
   ARITH_FOR_COM *arith_for_com = cmd->value.ArithFor;
   indent_level(level);
   printf("Inits:\n");
   indent_word_list(level+1, arith_for_com->init);
   indent_level(level);
   printf("Test:\n");
   indent_word_list(level+1, arith_for_com->test);
   indent_level(level);
   printf("Step:\n");
   indent_word_list(level+1, arith_for_com->step);

   dissect_command(level, arith_for_com->action);
}

void dissect_select(int level, COMMAND *cmd)
{
   SELECT_COM *select_com = cmd->value.Select;
   indent_level(level);
   printf("Selecting into %s%s%s: from\n", col_cool_blue, select_com->name->word, col_off);
   indent_word_list(level+1, select_com->map_list);
}


void dissect_connection(int level, COMMAND *cmd)
{
   CONNECTION *connection = cmd->value.Connection;
   print_info_line(level, connection->first, "first");
   dissect_command(level, connection->first);
   print_info_line(level, connection->second, "second");
   dissect_command(level, connection->second);
}

void dissect_command(int level, COMMAND *cmd)
{
   ++level;
   switch(cmd->type)
   {
      case cm_simple:
         dissect_simple(level, cmd);
         break;
      case cm_group:
         dissect_group(level, cmd);
         break;
      case cm_for:
         dissect_for(level, cmd);
         break;
      case cm_case:
         dissect_case(level, cmd);
         break;
      case cm_if:
         dissect_if(level, cmd);
         break;
      case cm_while:
      case cm_until:
         dissect_while(level, cmd);
         break;
      case cm_cond:
         dissect_cond(level, cmd);
         break;
      case cm_function_def:
         dissect_function_def(level, cmd);
         break;
      case cm_arith:
         dissect_arith(level, cmd);
         break;
      case cm_arith_for:
         dissect_arith_for(level, cmd);
         break;
      case cm_select:
         dissect_select(level, cmd);
         break;
      case cm_connection:
         dissect_connection(level, cmd);
         break;
      default:
         print_info_line(level, cmd, "\x1b[31;1munsupported\x1b[m");
         break;
   }
}

int dissect_function(const char *name)
{
   SHELL_VAR *svar = find_function(name);
   if (!svar)
   {
      fprintf(stderr, "Failed to find function \x1b[31;1m%s\x1b[m.\n", name);
      return 0;
   }

   if (!function_p(svar))
   {
      fprintf(stderr, "Shell variable \x1b[31;1m%s\x1bm is NOT a function.\n", name);
      return 0;
   }

   COMMAND *cmd = function_cell(svar);

   print_info_line(0, cmd, name);
   dissect_command(0, cmd);

   return 1;
}

static int func_dissect(WORD_LIST *list)
{
   WORD_LIST *ptr = list;
   while (ptr)
   {
      dissect_function(ptr->word->word);
      // Process the option

      ptr = ptr->next;
   }

   return EXECUTION_SUCCESS;
}

static char *desc_func_dissect[] = {
   "Quick tool to see how a parsed function is constructed.",
   (char*)NULL     // end of array marker
};

struct builtin func_dissect_struct = {
   .name      = "func_dissect",
   .function  = func_dissect,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_func_dissect,
   .short_doc = "func_dissect is cool",
   .handle    = 0
};

#include "dispatcher.h"
#include "error_handling.h"
#include <stdio.h>

SHOW_HELP show_help = TEMPLATE_default_help;

void TEMPLATE_default_help(AVERB *verbs, int array_len)
{
   AVERB *ptr = verbs;
   AVERB *end = ptr + array_len;

   while (ptr < end)
   {
      printf("Action %s:\n   %s\n", ptr->name, ptr->desc);
      ++ptr;
   }
}

AVERB* find_verb(AVERB* verb_array, int array_len, const char *name)
{
   AVERB *ptr = verb_array;
   AVERB *end = ptr + array_len;

   while (ptr < end)
   {
      if (strcmp(name, ptr->name)==0)
         return ptr;

      ++ptr;
   }

   return NULL;
}


int perform_verb(AVERB* verb_array, int array_len, WORD_LIST *args)
{
   int retval = EXECUTION_FAILURE;
   WORD_LIST *w_ptr = args;
   H_TEMPLATE *handle = NULL;

   if (w_ptr && w_ptr->word)
   {
      const char *vname = w_ptr->word->word;
      w_ptr = w_ptr->next;
      AVERB *verb = find_verb(verb_array, array_len, vname);
      if (verb)
      {
         if (verb->needs_handle)
         {
            if (w_ptr && w_ptr->word)
            {
               const char *hname = w_ptr->word->word;
               w_ptr = w_ptr->next;
               SHELL_VAR *sv = find_variable(hname);
               if (sv)
               {
                  if (TEMPLATE_p(sv))
                     handle = TEMPLATE_cell(sv);
                  else
                  {
                     (*ERROR_SINK)("Variable '%s' is not a TEMPLATE handle", hname);
                     retval = EX_BADUSAGE;
                     goto early_exit;
                  }
               }
               else
               {
                  (*ERROR_SINK)("Failed to find handle '%s'", hname);
                  retval = EX_NOTFOUND;
                  goto early_exit;
               }
            }
            else
            {
               (*ERROR_SINK)("Action '%s' requires a handle argument", vname);
               retval = EX_USAGE;
               goto early_exit;
            }
         }

         ACLONE *aclones = CLONE_WORD_LIST(w_ptr);
         retval = (*verb->action)(handle, aclones);
      }
      else
      {
         (*ERROR_SINK)("Action verb '%s' is not recognized", vname);
         retval = EX_NOTFOUND;
         goto early_exit;
      }
   }
   else
      (*show_help)(verb_array, array_len);

  early_exit:
   return retval;
}

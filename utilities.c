#include <builtins.h>
#include <shell.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc
#include <builtins/common.h>      // for no_options()

#include <stdarg.h>
#include <errno.h>

void bb_free_word_list(WORD_LIST *wl)
{
   WORD_LIST *ptr = wl;
   WORD_LIST *waiting = wl;
   while (waiting)
   {
      // Pointer bookkeeping before frees discard pointers:
      ptr = waiting;
      waiting = ptr->next;

      xfree(ptr->word->word);
      xfree(ptr->word);
      xfree(ptr);
   }
}

WORD_LIST *bb_append_string_to_word_list(WORD_LIST *wl, const char *string)
{
   errno = 0;

   WORD_LIST *ret_word = NULL;
   if (!string)
   {
      errno = EINVAL;
      goto exit;
   }

   // Find the end of the linked list:
   if (wl && wl->next)
      wl = wl->next;

   WORD_DESC *wdesc = NULL;
   WORD_LIST *nword = (WORD_LIST*)xmalloc(sizeof(WORD_LIST));
   if (nword)
   {
      memset(nword, 0, sizeof(WORD_LIST));

      wdesc = nword->word = (WORD_DESC*)xmalloc(sizeof(WORD_DESC));
      if (wdesc)
      {
         memset(wdesc, 0, sizeof(WORD_DESC));

         wdesc->word = savestring(string);
         if (wdesc->word)
         {
            wdesc->flags = 0;
            if (wl)
               wl->next = nword;

            ret_word = nword;

            goto exit;
         }
         else
         {
            errno = ENOMEM;
            goto free_desc;
         }
      }
      else
      {
         errno = ENOMEM;
         goto free_word;
      }
   }
   else
   {
      errno = ENOMEM;
      goto exit;
   }

  free_desc:
   xfree(wdesc);

  free_word:
   xfree(nword);

  exit:
   return ret_word;
}

int bb_make_word_list(WORD_LIST **wl, ...)
{
   errno = 0;

   va_list list_words;
   va_start(list_words, wl);

   WORD_LIST *head = NULL;
   WORD_LIST *tail = NULL;

   const char *string;
   while ((string = va_arg(list_words, const char*)))
   {
      tail = bb_append_string_to_word_list(tail, string);
      if (!head)
         head = tail;
   }

   va_end(list_words);

   if (errno == 0)
      *wl = head;

   return errno == 0;
}

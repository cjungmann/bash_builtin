#include <string.h>
#include "TEMPLATE_utilities.h"

/**
 * @brief Returns memory required to save a string, 0 if NULL
 *
 * Function checks for NULL (returns 0 length in that case),
 * and includes room for a terminating \0.
 */
int get_string_saved_len(const char *str)
{
   int slen = 0;
   if (str && (slen = strlen(str))>0)
      ++slen;
   return slen;
}

/**
 * @brief Packs a string with other strings into a block of memory.
 *
 * Tracks progress packing block through pointer-to-pointer arguments.
 * No copy is made if the string is NULL or of zero-length.  @p string
 * will be set to NULL.
 *
 * @param "string"       pointer to copy of @p value in the block
 * @param "buff_pointer" pointer to next available address in the block
 * @param "buff_limit"   pointer to end of block to prevent overruns
 * @param "value"        string value to copy into the block
 *
 * @return true for success, false for failure due to insufficient memory
 */
bool pack_string_in_block(const char **string,
                          char **buff_pointer,
                          char *buff_limit,
                          const char *value)
{
   *string = NULL;
   int slen;
   if (value && (slen = strlen(value)) > 0)
   {
      // check for enough space
      if (buff_limit - *buff_pointer <= slen)
         return false;

      *string = *buff_pointer;
      memcpy(*buff_pointer, value, slen);
      (*buff_pointer) += slen;
      **buff_pointer = '\0';
      (*buff_pointer)++;
   }

   return true;
}

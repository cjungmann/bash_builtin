#include <builtins.h>
#include <shell.h>
#include <array.h>
#include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
#include <builtins/common.h>      // for no_options()

#include <stdio.h>
#include <errno.h>                // for errno-compliant return values


/**
 * @brief Convert unsigned integer to string of the declared base.
 *
 * Uses recursion to reverse order of digits pulled from the
 * right, printing the digits when unwinding the stack.
 *
 * Using a pointer-to-pointer to keep track of position in the buffer
 * when unwinding the stack so first unwind prints to the first
 * character of the buffer.
 *
 * @param "sptr"  pointer to pointer to buffer in which to copy the digits
 * @param "end"   pointer to byte just after buffer (serves as limit)
 * @param "value" number to convert to binary
 * @param "base"  target number base
 *
 * @returns 0 for success, ENOBUFS if buffer is too small.
 */
int enbase(char **sptr, char *end, unsigned int value, unsigned int base)
{
   int retval = 0;
   unsigned int digit;

   if (value > 0)
   {
      retval = enbase( sptr, end, value/base, base);

      // Truncate and warn if not enough room
      if (*sptr >= end)
         return ENOBUFS;

      digit = (value % base);
      if (digit < 10)
         digit += '0';
      else if (digit < 36)
         digit = digit - 10 + 'a';
      else if (digit < 62)
         digit = digit - 36 + 'A';
      else if (digit == 62)
         digit = '+';
      else if (digit == 63)
         digit = '/';

      **sptr = digit;
      ++(*sptr);
   }

   return retval;
}

/**
 * @brief Convert an integer to a string of the given number base.
 *
 * @param "buff"   buffer to which result is written
 * @param "len"    bytes length of buffer
 * @param "value"  value to be convert
 * @param "base"   number base desired for conversion (allowed values are
 *                 between 2 and 64)
 *
 * @return 0 for success  
 *         EINVAL if base is out-of-range  
 *         ENOBUFS if buffer too small
 */
int enbase_value(char *buff, unsigned int len, unsigned int value, unsigned int base)
{
   if ( base < 2 || base > 64 )
      return EINVAL;

   if (value == 0)
   {
      *buff++ = '0';
      *buff = '\0';
      return 0;
   }

   char *ptr = buff;
   int retval = enbase(&ptr, buff+len, value, base);
   *ptr = '\0';
   return retval;
}

/**
 * @brief Demonstrate manipulating a Bash associative array.
 *
 * @notes `assoc_cell` is in include/bash/variables.h
 *        'assoc_flush` and others in include/bash/assoc.h
 */
static int fill_assoc_array(SHELL_VAR *svar)
{
   HASH_TABLE *assoc = assoc_cell(svar);
   assoc_flush(assoc);

   char buff[16];
   unsigned int counter = 0;

   // SHELL_VAR **arr_svars = all_shell_variables();
   SHELL_VAR **arr_svars = all_visible_variables();
   while (*arr_svars != NULL)
   {
      enbase_value( buff, sizeof(buff), ++counter, 2);
      assoc_insert(assoc, buff, (*arr_svars)->name);
      ++arr_svars;
   }

   return EXECUTION_SUCCESS;
}


/**
 * @brief Demonstrate manipulating a bash array
 *
 * @notes `array_cell` is in include/bash/variables.h
 *        'array_flush` and others in include/bash/array.h
 */
static int fill_array(SHELL_VAR *svar)
{
   ARRAY *array = array_cell(svar);
   array_flush(array);

   // SHELL_VAR **arr_svars = all_shell_functions();
   SHELL_VAR **arr_svars = all_visible_functions();
   while (*arr_svars != NULL)
   {
      array_insert(array, array->max_index+1, (*arr_svars)->name);
      ++arr_svars;
   }

   return EXECUTION_SUCCESS;
}

/**
 * @brief Testing function for enbase_value function.
 *
 * @param "buff"   buffer to which result is written
 * @param "len"    number of characters in th buffer
 * @param "value"  value to be converted
 * @param "base"   base value from 2 to 64
 */
void unit_test_enbase(char *buff, unsigned int len, unsigned int value, unsigned int base)
{
   if (enbase_value(buff, len, value, base))
      printf("truncated to buffer size: ");

   printf("The value %u, converted to base %d, is '%s'.\n", value, base, buff);
}

void test_enbase(void)
{
   char buffer[48];
   for (unsigned int i=0; i<64; ++i)
      unit_test_enbase(buffer, sizeof(buffer), i, 4);
}


static int demo_collections(WORD_LIST *list)
{
   int retval = EXECUTION_FAILURE;

   // test_enbase();

   if (list == 0)
      return EX_USAGE;

   SHELL_VAR *svar = find_variable(list->word->word);
   if (svar)
   {
      if (svar->attributes & att_array)
         retval = fill_array(svar);
      else if (svar->attributes & att_assoc)
         retval = fill_assoc_array(svar);
      else if (svar->attributes & att_function)
      {
         printf("Not yet supporting variable type 'function'.\n");
         return EX_NOTFOUND;
      }
      else if (svar->attributes & att_integer)
      {
         printf("Not yet supporting variable type 'integer'.\n");
         return EX_NOTFOUND;
      }
      else if (svar->attributes & att_nameref)
      {
         printf("Not yet supporting variable type 'nameref'.\n");
         return EX_NOTFOUND;
      }
      else
      {
         char buff[65];
         if (!enbase_value(buff, sizeof(buff), svar->attributes, 4))
            printf("No recognized attributes in %d (%s)\n", svar->attributes, buff);
         else
            printf("No recognized attributes in %d)\n", svar->attributes);

         return EX_NOTFOUND;
      }
   }
   else
   {
      printf("%s does not refer to a variable.\n", list->word->word);
      return EX_NOTFOUND;
   }

   return retval;
}

static char *desc_demo_collections[] = {
   "This function is less for accomplishing anything worthwhile"
   "and more about playing around with various collections."
   "Before I start development on this exercise, I'm thinking"
   "about traversing collections at a minimum to handle what Bash"
   "hands us.  Additionally, I'd like to reach for a better"
   "understanding by building collections from scratch."
};

struct builtin demo_collections_struct = {
   .name      = "demo_collections",
   .function  = demo_collections,
   .flags     = BUILTIN_ENABLED,
   .long_doc  = desc_demo_collections,
   .short_doc = "demo_collections array_name",
   .handle    = 0
};


#include "handle.h"
#include "TEMPLATE_utilities.h"
#include "error_handling.h"

#include <assert.h>

bool TEMPLATE_p(SHELL_VAR *var)
{
   if (var && specialvar_p(var))
      return (0 == strcmp((char*)var->value, TEMPLATE_HANDLE_ID));

   return false;
}

int TEMPLATE_calc_handle_size(const char *string)
{
   int totsize = sizeof(TEMPLATEH);
   totsize += get_string_saved_len(string);

   return totsize;
}

TEMPLATEH* TEMPLATE_initialize_handle(char *buffer,
                                      int buffer_len,
                                      const char *string)
{
   memset(buffer, 0, buffer_len);
   TEMPLATEH *handle = (TEMPLATEH*)buffer;
   memcpy(buffer, TEMPLATE_HANDLE_ID, sizeof(TEMPLATE_HANDLE_ID));

   char *cur_buff = buffer;
   char *end_buff = buffer + buffer_len;

   // skip past the handle struct
   cur_buff += sizeof(TEMPLATEH);

   pack_string_in_block(&handle->data.string, &cur_buff, end_buff, string);

   assert(cur_buff == end_buff);

   // If necessary, set other handle data, including settings
   // contingent on the content of any submitted strings

   return handle;
}

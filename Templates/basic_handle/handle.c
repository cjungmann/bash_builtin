#include "handle.h"
#include "TEMPLATE_utilities.h"
#include "error_handling.h"

#include <assert.h>

bool TEMPLATE_p(SHELL_VAR *var)
{
   if (var && var->attributes & att_special)
      return (0 == strcmp((char*)var->value, TEMPLATE_HANDLE_ID));

   return false;
}

int TEMPLATE_calc_handle_size(const char *name, const char *value, const char *callback)
{
   int totsize = sizeof(TEMPLATEH);
   totsize += get_string_saved_len(name);
   totsize += get_string_saved_len(value);
   totsize += get_string_saved_len(callback);

   return totsize;
}

TEMPLATEH* TEMPLATE_initialize_handle(char *buffer,
                                      int buffer_len,
                                      const char *name,
                                      const char *value,
                                      const char *callback)
{
   memset(buffer, 0, buffer_len);
   TEMPLATEH *handle = (TEMPLATEH*)buffer;
   memcpy(buffer, TEMPLATE_HANDLE_ID, sizeof(TEMPLATE_HANDLE_ID));

   char *cur_buff = buffer;
   char *end_buff = buffer + buffer_len;

   // skip past the handle struct
   cur_buff += sizeof(TEMPLATEH);

   pack_string_in_block(&handle->data.name, &cur_buff, end_buff, name);
   pack_string_in_block(&handle->data.value, &cur_buff, end_buff, value);
   pack_string_in_block(&handle->data.callback, &cur_buff, end_buff, callback);

   assert(cur_buff == end_buff);

   // If necessary, set other handle data, including settings
   // contingent on the content of any submitted strings

   return handle;
}

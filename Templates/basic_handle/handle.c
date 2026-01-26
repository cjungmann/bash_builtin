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

   // TEMPLATE_declare should have calculated the appropriate
   // buffer length by calling TEMPLATE_calc_handle_size, so
   // we're just "protecting" with this assert:
   assert(cur_buff == end_buff);

   // If necessary, set other handle data, including settings
   // contingent on the content of any submitted strings

   // DEVELOPER NOTE: The calling function (TEMPLATE_declare())
   //                 assumes that any failure in this function will
   //                 have already called (*ERROR_SINK) with the
   //                 explanation.

   return handle;
}

void TEMPLATE_dispose(TEMPLATEH *handle)
{
   // Simply free by default, but provide appropriate code
   // if memory model demands additional steps.
   free(handle);
}

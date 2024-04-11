#include "handle.h"
#include "error_handling.h"

bool TEMPLATE_p(SHELL_VAR *var)
{
   if (var && var->attributes & att_special)
      return (0 == strcmp((char*)var->value, TEMPLATE_HANDLE_ID));

   return false;
}

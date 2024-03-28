#include "handle.h"

bool TEMPLATE_p(SHELL_VAR *var)
{
   if (var && var->attributes & att_special)
      return 0 == strcmp(TEMPLATE_cell(var)->id, TEMPLATE_HANDLE_ID);
   return false;
}

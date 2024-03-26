#include "local_builtin.h"
#include "error_handling.h"
#include "shell_vars.h"
#include <stdarg.h>
#include <stdio.h>

#define ERROR_NAME "TEMPLATE_ERROR"
error_sink_type ERROR_SINK = error_report_to_shell_var;


void error_report_clear_shell_var(void)
{
   SHELL_VAR *sv = find_variable(ERROR_NAME);
   if (!sv)
      sv = bind_variable(ERROR_NAME, "", 0);

   if (sv)
      (*DISPOSE_SHELL_VAR)(sv);
}

void error_report_to_shell_var(const char *format, ...)
{
   va_list args;
   va_start(args, format);
   int len = vsnprintf(NULL, 0, format, args);
   va_end(args);

   if (len > 0)
   {
      ++len;
      char *buffer = xmalloc(len);
      if (buffer)
      {
         va_start(args, format);
         vsnprintf(buffer, len, format, args);
         va_end(args);

         SHELL_VAR *sv = find_variable(ERROR_NAME);
         if (!sv)
            sv = bind_variable(ERROR_NAME, "", 0);

         if (sv)
         {
            (*DISPOSE_SHELL_VAR)(sv);
            sv->value = buffer;
         }
      }
   }
}

void error_report_to_stderr(const char *format, ...)
{
   va_list args;
   va_start(args, format);
   fprintf(stderr, format, args);
   va_end(args);
}



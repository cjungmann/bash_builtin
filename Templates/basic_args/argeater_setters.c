#include "local_builtin.h"
#include "error_handling.h"
#include "argeater_setters.h"

#include <errno.h>
#include <string.h>   // for strerror()
#include <stdio.h>    // for fopen()
#include <fcntl.h>    // for open()
#include <stdlib.h>   // for stdtol

bool TEMPLATE_argeater_stream_setter(const char **target, const char *value)
{
   FILE **f_target = (FILE**)target;

   *f_target = fopen(value, "r");
   if (*f_target)
      return true;
   else
   {
      (*ERROR_SINK)("Failed to open '%s': %s", value, strerror(errno));
      return false;
   }
}

bool TEMPLATE_argeater_file_setter(const char **target, const char *value)
{
   int *i_target = (int*)target;

   *i_target = open(value, O_RDONLY);
   if (*i_target == -1)
   {
      (*ERROR_SINK)("Failed to open '%s': %s", value, strerror(errno));
      return false;
   }
   else
      return true;
}

bool TEMPLATE_argeater_long_setter(const char **target, const char *value)
{
   long *l_target = (long*)target;
   char *endptr;
   long temp = strtol(value, &endptr, 10);
   if (endptr > value)
   {
      if (temp == LONG_MAX)
         (*ERROR_SINK)("Value '%s' results in an overflow", value);
      else if (temp == LONG_MIN)
         (*ERROR_SINK)("Value '%s' results in an underflow", value);
      else
      {
         *l_target = temp;
         return true;
      }
   }
   else
      (*ERROR_SINK)("Unable to convert '%s' to a long integer", value);

   return true;
}

bool TEMPLATE_argeater_bool_setter(const char **target, const char *value)
{
   *(bool*)target = true;
   return true;
}

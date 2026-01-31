void sterilize_shell_var(SHELL_VAR *sv);
bool get_or_make_shell_var(SHELL_VAR **sv, const char *name);
int vprintf_to_shell_var(SHELL_VAR *sv, const char *format, va_list ap_arg);
int printf_to_shell_var(SHELL_VAR *sv, const char *format, ...);

/**
 * @file utilities.c
 * @brief Small collection of Bash SHELL_VAR functions
 *
 * Isolated these utilities to permit the template or generated
 * file to maintain focus on its mission.
 */

/**
 * @brief Enhanced version of dispose_variable_value
 * This function was copied from variables.c from the Bash source
 * code and enhanced by removing data type flags to make a blank
 * slate of the supplied SHELL_VAR
 */
void sterilize_shell_var(SHELL_VAR *sv)
{
   if (sv->value)
   {
      if (function_p(sv))
      {
         dispose_command(function_cell(sv));
         VUNSETATTR(sv, att_function);
      }
      else if (array_p(sv))
      {
         array_dispose(array_cell(sv));
         VUNSETATTR(sv, att_array);
      }
      else if (assoc_p(sv))
      {
         assoc_dispose(assoc_cell(sv));
         VUNSETATTR(sv, att_assoc);
      }
      else if (nameref_p(sv))
      {
         free(nameref_cell(sv));
         VUNSETATTR(sv, att_nameref);
      }
      else
      {
         free(sv->value);
         VUNSETATTR(sv, att_integer);
      }

      sv->value = NULL;
   }
}

/**
 * @brief Get a SHELL_VAR by name, even if it must be created.
 *
 * In getting the SHELL_VAR, this function tries to find the
 * variable by name in the current context.  If it fails, a new
 * SHELL_VAR will be created with the requested name.
 */
bool get_or_make_shell_var(SHELL_VAR **sv, const char *name)
{
   SHELL_VAR *tsv = find_variable(name);
   // Use existing only if it's in the same context
   if (!tsv || tsv->context != variable_context)
   {
      if (variable_context == 0)
         tsv = bind_variable(name, "", 0);
      else
         tsv = make_local_variable(name, 0);
   }

   if (tsv)
      *sv = tsv;

   return tsv != NULL;
}

/**
 * @brief Report error details to a shell var for script use.
 *
 * Leaving the error message in a shell variable permits
 * detailed and specific error information and the ability
 * of a script to decide whether and how to respond to an
 * error.
 *
 * The function has a small fail-safe in that in the unlikely
 * occurrence that the BOGUS_ERROR shell variable is
 * unavailable, the error message will be printed to _stderr_.
 *
 * @note Returns int to match the function signature of
 *       printf.  The way it's called means the return
 *       value will never be examined.
 */
int vprintf_to_shell_var(SHELL_VAR *sv, const char *format, va_list ap_arg)
{
   sterilize_shell_var(sv);

   // Use copy of ap_arg to measure string length:
   va_list ap_copy;
   va_copy(ap_copy, ap_arg);
   int ccount = vsnprintf(NULL, 0, format, ap_copy);
   va_end(ap_copy);

   if (ccount>0)
   {
      ++ccount;

      char *buff = (char*)xmalloc(ccount);
      if (buff)
      {
         vsnprintf(buff, ccount, format, ap_arg);

         sv->value = buff;
         VUNSETATTR(sv, att_invisible|att_integer|att_array|att_assoc);
      }
   }

   return ccount;
}

/**
 * @brief Basically just collects arguments into a va_list and
 *        dispatches them to vprintf_to_shell_var().
 */
int printf_to_shell_var(SHELL_VAR *sv, const char *format, ...)
{
  va_list list_args;
  va_start(list_args, format);

  int len = vprintf_to_shell_var(sv, format, list_args);

  va_end(list_args);

  return len;
}


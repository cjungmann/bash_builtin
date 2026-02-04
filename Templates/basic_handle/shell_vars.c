#include "shell_vars.h"
#include "error_handling.h"
#include "handle.h"

/**
 * @brief Implementation of private Bash function `dispose_variable_value`.
 *
 * According to the data type of the SHELL_VAR, this function
 * frees its *value* member and sets it to NULL.
 *
 * Since this function is primarily employed to reuse an existing
 * SHELL_VAR that may be of any data type, it unsets the
 * type-identifying attribute that identifies the nature of the
 * SHELL_VAR's *value* member.
 *
 * This function could be customized to handle bespoke project
 * shell variables.
 *
 * Although this can't be used by Bash to dispose of your custom
 * variable, if you have one, it does at least permit you to manage
 * your custom variable's contents in the context of your own code.
 */
void local_dispose_variable_value(SHELL_VAR *sv)
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
      else if (TEMPLATE_p(sv))
      {
         TEMPLATE_dispose(TEMPLATE_cell(sv));
         free(sv->value);
         VUNSETATTR(sv, att_special);
      }
      else
      {
         free(sv->value);
         VUNSETATTR(sv, att_integer);
      }

      sv->value = NULL;
   }
}

// Use Bash-provided implementation unless a custom
// variable (handle?) requires special treatment:
dispose_shell_var_type DISPOSE_SHELL_VAR = local_dispose_variable_value;

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
   // Acceptable existing variable must be in same context
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
 * @brief Simplify handle-making with generic creation of shell var
 *
 * This method for populating a SHELL_VAR does many things:
 * 1. It reuses an existing SHELL_VAR if it has the same name.
 * 2. Creates a new SHELL_VAR if an existing one is not available.
 * 3. Uses @ref local_dispose_variable_value to appropriately
 *    reuse a SHELL_VAR of the same type to prevent memory leaks.
 * 4. Uses *ERROR_SINK to report errors
 */
int install_payload_to_shell_var(SHELL_VAR **sv, const char *name, void *payload)
{
   int exit_code = EXECUTION_FAILURE;
   SHELL_VAR *tsv = NULL;

   // `bind_variable` is better than `make_local_variable` for enabling
   // recursion in scripts
   tsv = bind_variable(name, "", 0);

   if (tsv)
   {
      local_dispose_variable_value(tsv);
      tsv->value = payload;
      VSETATTR(tsv, att_special);
      if (invisible_p(tsv))
         VUNSETATTR(tsv, att_invisible);

      // Use is allowed to ignore the instance of a new shell_var
      // (success or failure indicated by exit code).
      if (sv)
         *sv = tsv;

      exit_code = EXECUTION_SUCCESS;
   }
   else
      (*ERROR_SINK)("Failed to create variable '%s'.", name);

   return exit_code;
}


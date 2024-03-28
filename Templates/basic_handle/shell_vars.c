#include "shell_vars.h"

/**
 * @brief Duplicate of private Bash function `dispose_variable_value`.
 *
 * Used to replace contents of shell variables.
 *
 * This function could be customized to handle bespoke project
 * shell variables.
 *
 * Although this can't be used by Bash to dispose of your custom
 * variable, if you have one, it does at least permit you to manage
 * your custom variable's contents in the context of your own code.
 */
void local_dispose_variable_value(SHELL_VAR *var)
{
   if (array_p(var))
      array_dispose(array_cell(var));
   else if (assoc_p(var))
      assoc_dispose(assoc_cell(var));
   else if (function_p(var))
      dispose_command(function_cell(var));
   else if (nameref_p(var))
      FREE(nameref_cell(var));
   else
      FREE(value_cell(var));

   var->value = NULL;
}

// Use Bash-provided implementation unless a custom
// variable (handle?) requires special treatment:
dispose_shell_var_type DISPOSE_SHELL_VAR = local_dispose_variable_value;


/*
 * Custom variable (handle?) section
 *
 * To conform to Bash variable practices, a custom variable needs
 * to have an identifier macro (TEMPLATE_p), an type-access macro
 * (TEMPLATE_cell), and if necessary, a custom dispose_variable_value
 * function that handles deleting the resources of this and other
 * project-specific custom variables.
 */


/**
 * Replacement disposal function incorporation necessary resource
 * releasing for custom variables
 */
// void TEMPLATE_dispose_variable_value(SHELL_VAR *var)
// {
//    if (TEMPLATE_p(var))
//    {
//       // custom disposal code
//    }
//    else
//       dispose_variable_value(var);
// }

#ifndef SHELL_VARS_H
#define SHELL_VARS_H

#include <stdbool.h>
#include "local_builtin.h"

typedef void (*dispose_shell_var_type)(SHELL_VAR *var);
extern dispose_shell_var_type DISPOSE_SHELL_VAR;

bool get_or_make_shell_var(SHELL_VAR **sv, const char *name);
int install_payload_to_shell_var(SHELL_VAR **sv, const char *name, void *payload);


#endif

#ifndef HANDLE_H
#define HANDLE_H

#include "local_builtin.h"
#include <stdbool.h>

#define TEMPLATE_HANDLE_ID "TEMPLATE handle"

typedef struct TEMPLATE_handle {
   char id[sizeof(TEMPLATE_HANDLE_ID)+1];
   void *data;
} H_TEMPLATE;

bool TEMPLATE_p(SHELL_VAR *var);
#define TEMPLATE_cell(var) ((H_TEMPLATE*)(var)->value)

#endif

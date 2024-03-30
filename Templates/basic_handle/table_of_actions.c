#include "dispatcher.h"

// Action prototypes from "action_ssss.c" files:

int TEMPLATE_help(H_TEMPLATE* handle, ACLONE *args);
extern AE_MAP TEMPLATE_help_arg_map;

int TEMPLATE_declare(H_TEMPLATE* handle, ACLONE *args);
extern AE_MAP TEMPLATE_declare_arg_map;

AVERB TEMPLATE_action_table[] = {
   { "help", "show a help display", "TEMPLATE help", false,
     TEMPLATE_help, &TEMPLATE_help_arg_map },

   { "declare", "create a new TEMPLATE handle", "TEMPLATE handle", false,
     TEMPLATE_declare, &TEMPLATE_declare_arg_map }
};

__attribute__((unused))
unsigned int TEMPLATE_actions_count = sizeof(TEMPLATE_action_table) / sizeof(AVERB);


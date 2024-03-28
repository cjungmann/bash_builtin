#include "action_dispatch.h"
#include "handle_actions.h"

AVERB action_table[] = {
   { "help", "show a help display", "TEMPLATE help", false, TEMPLATE_display_help },
   { "declare", "create a new TEMPLATE handle", "TEMPLATE handle", false, TEMPLATE_declare }
};

__attribute__((unused))
unsigned int actions_count = sizeof(action_table) / sizeof(AVERB);


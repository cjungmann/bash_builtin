#include "dispatcher.h"

// Action prototypes from "action_ssss.c" files:

// The help action will work for all verbs if the action table
// below is properly setup.  Only consider replace this if the
// default help provider doesn't suit your needs.

// Add actions by copying action_boilerplate.def to action_[new_verb].c,
// modify code in the C file, then add the AVERB declaration in the new
// C file to the extern declarations and the action_table:

extern AVERB TEMPLATE_help_verb;
extern AVERB TEMPLATE_declare_verb;
// END-OF-EXTERNS

AVERB *TEMPLATE_action_table[] = {
   &TEMPLATE_help_verb,
   &TEMPLATE_declare_verb,
   // END-OF-VERBS
   NULL
};

__attribute__((unused))
unsigned int TEMPLATE_actions_count = (sizeof(TEMPLATE_action_table) / sizeof(AVERB*)) - 1;


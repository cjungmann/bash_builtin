#include "dispatcher.h"

// Action prototypes from "action_ssss.c" files:

// The help action will work for all verbs if the action table
// below is properly setup.  Only consider replace this if the
// default help provider doesn't suit your needs.

extern AVERB TEMPLATE_help_verb;
extern AVERB TEMPLATE_boilerplate_verb;
extern AVERB TEMPLATE_declare_verb;

AVERB *TEMPLATE_action_table[] = {
   &TEMPLATE_help_verb,
   &TEMPLATE_boilerplate_verb,
   &TEMPLATE_declare_verb
};

__attribute__((unused))
unsigned int TEMPLATE_actions_count = sizeof(TEMPLATE_action_table) / sizeof(AVERB*);


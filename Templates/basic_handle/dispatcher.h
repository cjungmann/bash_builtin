#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "local_builtin.h"
#include "handle.h"

#include <argeater.h>
#include <stdbool.h>

typedef int (*TEMPLATE_action)(H_TEMPLATE* handle, ACLONE *args);

typedef struct TEMPLATE_action_verb {
   const char *name;
   const char *desc;
   const char *usage;
   bool needs_handle;
   TEMPLATE_action action;
   AE_MAP *arg_map;
} AVERB;

typedef void (*SHOW_HELP)(AVERB *verbs, int array_len);

AVERB* find_verb(AVERB* verb_array, int array_len, const char *name);
int perform_verb(AVERB* verb_array, int array_len, WORD_LIST *args);
void TEMPLATE_default_help(AVERB *verbs, int array_len);

extern SHOW_HELP show_help;

#endif

#ifndef JBI_BUILTIN_H
#define JBI_BUILTIN_H

#if defined(_STDIO_H) || defined(ARGEATER_H)
#  error jbi_builtin.h must be first include in a source file.
#  include <jbi_builtin.h_must_be_first_include_in_source_file>
#endif


#include <bash/builtins.h>
// Prevent multiple inclusion of shell.h:
#ifndef EXECUTION_FAILURE
#include <bash/shell.h>
#endif

extern int execute_command PARAMS((COMMAND *));

#endif


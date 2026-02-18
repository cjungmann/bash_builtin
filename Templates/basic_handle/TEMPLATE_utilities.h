#ifndef TEMPLATE_UTILITIES_H
#define TEMPLATE_UTILITIES_H

#include <stdbool.h>
#include <stdarg.h>

/**
 * @defgroup string_block Save consecutive strings into a single memory block
 * These functions are to be used to pack a set of strings
 * after the fixed-length part of a handle.
 * @{
 */
int get_string_saved_len(const char *str);

bool pack_string_in_block(const char **string,
                          char **buff_pointer,
                          char *buff_limit,
                          const char *value);

/** @} */

char* savevprintf(const char *format, va_list ap_arg);
char* saveprintf(const char *format, ...);

#endif

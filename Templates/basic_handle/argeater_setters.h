#ifndef ARGEATER_SETTERS_H
#define ARGEATER_SETTERS_H

#include <argeater.h>

bool TEMPLATE_argeater_stream_setter(const char **target, const char *value);
bool TEMPLATE_argeater_file_setter(const char **target, const char *value);
bool TEMPLATE_argeater_long_setter(const char **target, const char *value);
bool TEMPLATE_argeater_bool_setter(const char **target, const char *value);

bool TEMPLATE_argeater_handle_setter(const char **target, const char *value);
bool TEMPLATE_argeater_function_setter(const char **target, const char *value);
bool TEMPLATE_argeater_return_sv_setter(const char **target, const char *value);


#endif

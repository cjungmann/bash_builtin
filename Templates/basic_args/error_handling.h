#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

typedef void (*error_sink_type)(const char *format, ...);

extern error_sink_type ERROR_SINK;

void error_report_to_shell_var(const char *format, ...);
void error_report_to_stderr(const char *format, ...);

void error_report_clear_shell_var(void);

#endif

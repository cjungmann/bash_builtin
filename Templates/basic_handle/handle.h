#ifndef HANDLE_H
#define HANDLE_H

#include "local_builtin.h"
#include <stdbool.h>

#define TEMPLATE_HANDLE_ID "TEMPLATE handle"

/**
 * @brief Example of struct to 'declare' argument strings
 *
 * Add and remove struct members according to your needs
 */
typedef struct TEMPLATE_data {
   const char *string;
} D_TEMPLATE;

typedef struct TEMPLATE_handle {
   char id[sizeof(TEMPLATE_HANDLE_ID)+1];
   D_TEMPLATE data;
} TEMPLATEH;

/**
 * @defgroup IMHEADER
 * @brief Predict size and populate memory for immutable handle
 *
 * In general, the arguments should match the strings used in the
 * TEMPLATE_data struct.  The actual strings will be saved after the
 * final data member of TEMPLATE_data, with the addresses of the
 * strings saved in the TEMPLATE_data data.
 * @{
 */
int TEMPLATE_calc_handle_size(const char *string);

TEMPLATEH * TEMPLATE_initialize_handle(char *buffer,
                                       int buffer_len,
                                       const char *string);

void TEMPLATE_dispose(TEMPLATEH *handle);
/** @} */

/**
 * @defgroup Bash-modelled interface between SHELL_VAR* and TEMPLATE_handle*
 *
 * To follow Bash code convention and to promote understanding thereof,
 * these functions use established suffixes for identifying and accessing
 * the content of a Bash SHELL_VAR.
 * @{
 */
bool TEMPLATE_p(SHELL_VAR *var);
#define TEMPLATE_cell(var) ((TEMPLATEH*)(var)->value)
/** @} */


#endif

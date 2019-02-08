

#ifndef SCRIPT_BUILDERS_H
# define SCRIPT_BUILDERS_H

#include "shell_script_parser.h"


t_bresult		simple_command_build(t_token *list, size_t length);

t_node			*ast_new_node(void *value, enum e_token_type token_type,
											enum e_node_type node_type);
void			ast_free_recursive(t_node *node);

#endif


#include "shell_script.h"
#include "shell_script_parser.h"
#include "shell_script_builders.h"

t_bresult	simple_command_build(t_token *list, size_t length)
{
	t_node				*node;
	t_bresult			result;
	struct s_command	*command;

	command = (struct s_command *)ft_memalloc(sizeof(struct s_command));

	node = ast_new_node(command, TOKEN_NOT_APPLICABLE, NODE_COMMAND);
	result.ast_root = node;
	result.error = NULL;
	return (result);
}

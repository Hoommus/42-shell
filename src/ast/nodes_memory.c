
#include "shell_script.h"
#include "shell_script_parser.h"

t_node	*ast_new_node(void *value, enum e_token_type token_type,
									enum e_node_type node_type)
{
	t_node	*node;

	node = ft_memalloc(sizeof(t_node));
	node->token_type = token_type;
	node->node_type = node_type;
//	if (node_type == NODE_GENERIC)
//		node->value = ft_strdup(value);
//	else
		node->value = value;
	return (node);
}

void	ast_free_recursive(t_node *node)
{
	if (node->left != NULL)
		ast_free_recursive(node->left);
	if (node->right != NULL)
		ast_free_recursive(node->right);
	free(node->value);
	free(node);
}

#include "shell_script.h"
#include "shell_script_parser.h"

void			run_script(t_token *stream_head, bool log_recursion)
{
	t_state				state;
	struct s_result		result;

	if (stream_head == NULL)
		return ;
	ft_bzero(&state, sizeof(t_state));
	state.list_head = stream_head;
	state.list_offset = stream_head;
	state.rule = &g_program;
	state.depth = -2;
	state.logging = log_recursion;
	result = is_syntax_valid(state);
	if (result.error != NULL)
		ft_printf("Syntax error: %s\n", result.error->text);
	state.list_offset = offset_list(state.list_offset, result.consumed);
	ft_printf("consumed: %d\n", result.consumed);
	if (state.list_offset != NULL)
		ft_printf("21sh: syntax error near token '%s'\n",
			ft_strcmp(state.list_offset->value, "\n") == 0 ? "\\n" :
			state.list_offset->value);
	else
		ft_printf("Syntax valid\n");
//  TODO: execute(tree);
//  TODO: free(tree);
//  TODO: free(stream_head);
}

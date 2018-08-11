#include "../../include/script_lang.h"
#include "../../include/twenty_one_sh.h"

struct s_token	*new_token(char *value)
{
	struct s_token	*new;

	new = (struct s_token *)malloc(sizeof(struct s_token));
	ft_bzero(new, sizeof(struct s_token));
	new->value = value;
	new->type = DUMMY;
	return (new);
}

void			free_token(struct s_token *token)
{
	if (token == NULL)
		return ;
	chfree(token->value);
	chfree(token);
}

/*
void			push_token(struct s_token *head, struct s_token *token)
{

}
*/
/*
void			insert_as_child(struct s_token *parent)
{

}

void			insert_as_sibling(struct s_token *left_brother)
{

}
 */
#include "../../include/script_lang.h"
#include "../../include/twenty_one_sh.h"

char			*g_keywords[] = {
	"fi",
	"case",
	"do",
	"done",
	"elif",
	"else",
	"esac",
	"fi",
	"for",
	"if",
	"in",
	"then",
	"until",
	"while"
};

char			*g_operators[] = {
	"<",
	">",
	"+",
	"-",
	"/",
	"*",
	"&&",
	"||",
	"$"
};

struct s_token	*tokenize(char **array)
{
	struct s_token	*tokens;

	tokens = new_token(*array);
	return (tokens);
}

<<<<<<< HEAD
#include "inc/minishell.h"

int	is_pipe(char c)
{
	return (c == '|');
}

int	is_redirect(char c)
{
	return (c == '>' || c == '<');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

t_quote	get_quote_type(char c)
{
	if (c == '\'')
		return (SINGLE_QUOTE);
	return (DOUBLE_QUOTE);
}

t_token_type	get_token_type_from_quote(t_quote quote)
{
	if (quote == SINGLE_QUOTE)
		return (T_SINGLE_QUOTED);
	return (T_DOUBLE_QUOTED);
=======
#include "inc/minishell.h"

int	is_pipe(char c)
{
	return (c == '|');
}

int	is_redirect(char c)
{
	return (c == '>' || c == '<');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

t_quote	get_quote_type(char c)
{
	if (c == '\'')
		return (SINGLE_QUOTE);
	return (DOUBLE_QUOTE);
}

t_token_type	get_token_type_from_quote(t_quote quote)
{
	if (quote == SINGLE_QUOTE)
		return (T_SINGLE_QUOTED);
	return (T_DOUBLE_QUOTED);
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
}
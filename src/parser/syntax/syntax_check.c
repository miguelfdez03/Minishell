#include "../../minishell.h"

static int	check_pipe_syntax(t_token *current)
{
	if (!current->next)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (0);
	}
	if (current->next->type == PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (0);
	}
	return (1);
}

int	check_syntax(t_data **data)
{
	t_token	*current;

	if (!data || !(*data) || !(*data)->tokens)
		return (1);
	current = (*data)->tokens;
	if (current->type == PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (0);
	}
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!check_pipe_syntax(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

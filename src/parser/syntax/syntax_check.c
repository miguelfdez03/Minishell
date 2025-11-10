#include "../../minishell.h"

static int	is_redir_token(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

static int	is_operator_token(t_token_type type)
{
	return (type == PIPE || is_redir_token(type));
}

static void	print_redir_error(t_token_type type)
{
	if (type == PIPE)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `|'", 2);
	else if (type == REDIR_IN)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `<'", 2);
	else if (type == REDIR_OUT)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `>'", 2);
	else if (type == REDIR_APPEND)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `>>'", 2);
	else if (type == HEREDOC)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `<<'", 2);
}

static int	check_pipe_syntax(t_token *current)
{
	if (!current->next)
	{
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `newline'", 2);
		return (0);
	}
	if (current->next->type == PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `|'", 2);
		return (0);
	}
	return (1);
}

static int	check_redir_syntax(t_token *current)
{
	if (!current->value || current->value[0] == '\0')
	{
		if (!current->next || is_operator_token(current->next->type))
		{
			if (!current->next)
				ft_putendl_fd("minishell: syntax error near unexpected "
					"token `newline'", 2);
			else
				print_redir_error(current->next->type);
			return (0);
		}
	}
	return (1);
}

static int	check_first_token(t_token *current)
{
	if (is_operator_token(current->type))
	{
		if (current->type == PIPE)
			ft_putendl_fd("minishell: syntax error near unexpected "
				"token `|'", 2);
		else
			print_redir_error(current->type);
		return (0);
	}
	return (1);
}

static int	check_tokens_loop(t_token *current)
{
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!check_pipe_syntax(current))
				return (0);
		}
		else if (is_redir_token(current->type))
		{
			if (!check_redir_syntax(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_syntax(t_data **data)
{
	t_token	*current;

	if (!data || !(*data) || !(*data)->tokens)
		return (1);
	current = (*data)->tokens;
	if (!check_first_token(current))
		return (0);
	return (check_tokens_loop(current));
}

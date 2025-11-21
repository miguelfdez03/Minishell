/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:54 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:41:55 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_redir_token(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

int	is_operator_token(t_token_type type)
{
	return (type == PIPE || is_redir_token(type));
}

void	print_redir_error(t_token_type type)
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

int	check_pipe_syntax(t_token *current)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:55 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:41:56 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_redir_syntax(t_token *current)
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

int	check_first_token(t_token *current)
{
	if (current->type == PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `|'", 2);
		return (0);
	}
	return (1);
}

int	check_tokens_loop(t_token *current)
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

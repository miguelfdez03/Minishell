/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:03 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:42:04 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_first_cmd_args(t_token **tmp, t_data **data)
{
	t_token	*current;

	current = *tmp;
	while (current && current->type != PIPE)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			if (current->value)
				add_redir(&((*data)->cmd->redirections), current->type,
					ft_strdup(current->value));
		}
		else if (current->type != EMPTY && current->value
			&& (current->type == WORD || current->type == STRING
				|| current->type == ARGS || current->type == SIMPLE_Q))
			add_cmd_arg((*data)->cmd, current->value);
		current = current->next;
	}
	*tmp = current;
}

int	check_and_exp(t_data **data)
{
	t_token	*tmp;
	t_token	*start;

	if (!(*data)->tokens)
		return (EXIT_FAILURE);
	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);
	concatenate_tokens(&(*data)->tokens);
	start = (*data)->tokens;
	tmp = (*data)->tokens;
	if (init_first_cmd(data, &tmp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	process_first_cmd_args(&start, data);
	if (start && start->type == PIPE)
	{
		if (process_pipes(*data) == -1)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

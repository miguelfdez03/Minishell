/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 19:18:31 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/04 22:42:25 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_next_cmd(t_cmd *current_cmd, t_token **tokens)
{
	t_token	*tmp;

	if (!tokens || !*tokens)
		return (-1);
	current_cmd->next = malloc(sizeof(t_cmd));
	if (!current_cmd->next)
		return (-1);
	init_cmd(current_cmd->next);
	tmp = *tokens;
	if (tmp && tmp->value && (tmp->type == WORD || tmp->type == STRING))
	{
		current_cmd->next->name = ft_strdup(tmp->value);
		if (!current_cmd->next->name)
		{
			free(current_cmd->next);
			current_cmd->next = NULL;
			return (-1);
		}
		current_cmd->next->builtin_id = identify_builtin(tmp->value);
		*tokens = (*tokens)->next;
	}
	return (0);
}

static void	process_cmd_args(t_cmd *cmd, t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->value && (tmp->type == WORD || tmp->type == STRING
				|| tmp->type == ARGS || tmp->type == SIMPLE_Q))
			add_cmd_arg(cmd, tmp->value);
		tmp = tmp->next;
		*tokens = tmp;
	}
}

int	handle_pipe(t_cmd *current_cmd, t_token **tokens)
{
	if (!current_cmd || !tokens || !*tokens)
		return (-1);
	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		ft_printf("syntax error near unexpected token `|'\n");
		return (-1);
	}
	if (init_next_cmd(current_cmd, tokens) == -1)
		return (-1);
	process_cmd_args(current_cmd->next, tokens);
	return (0);
}

int	process_pipes(t_data *data)
{
	t_token	*tmp;
	t_cmd	*current_cmd;

	if (!data || !data->cmd || !data->tokens)
		return (-1);
	tmp = data->tokens;
	current_cmd = data->cmd;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (handle_pipe(current_cmd, &tmp) == -1)
				return (-1);
			current_cmd = current_cmd->next;
		}
		else
			tmp = tmp->next;
	}
	return (0);
}

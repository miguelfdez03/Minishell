/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 19:38:21 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/25 19:39:21 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_tokens(t_token *token)
{
	token->value = NULL;
	token->type = EMPTY;
}

int	init_cmd_data(t_data **data)
{
	(*data)->cmd = malloc(sizeof(t_cmd));
	if (!(*data)->cmd)
	{
		ft_printf("Error: Failed to allocate memory for cmd structure\n");
		return (-1);
	}
	(*data)->cmd->name = NULL;
	(*data)->cmd->args = NULL;
	(*data)->cmd->builtin_id = BUILTIN_NONE;
	(*data)->cmd->redirections = NULL;
	(*data)->cmd->next = NULL;
	return (0);
}

int	init_data(t_data **data, char **env, t_env *env_t)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
		return (-1);
	(*data)->input = NULL;
	(*data)->tokens = NULL;
	env_t = malloc(sizeof(t_env));
	if (!env_t)
	{
		free(*data);
		return (-1);
	}
	(*data)->env = env_t;
	if (init_env(env, env_t) == -1)
	{
		free(env_t);
		free(*data);
		return (-1);
	}
	if (init_cmd_data(data) == -1)
	{
		free(env_t);
		free(*data);
		return (-1);
	}
	(*data)->path = NULL;
	(*data)->pipe_flag = -1;
	(*data)->exit_status = 0;
	return (0);
}

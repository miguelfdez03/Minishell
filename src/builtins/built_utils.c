/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:40:42 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:40:43 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_command(t_data *data)
{
	t_cmd	*cmd;
	int		exit_status;

	cmd = data->cmd;
	if ((!cmd->name || cmd->name[0] == '\0') && (!cmd->args || !cmd->args[0]))
	{
		if (cmd->redirections)
			return (process_redirections_only(data));
		return (0);
	}
	if (!cmd->name || cmd->name[0] == '\0')
	{
		exit_status = handle_empty_cmd(cmd, data);
		if (exit_status != -1)
			return (exit_status);
	}
	if (cmd->next)
		return (execute_pipeline(data));
	return (execute_single_command(data, cmd));
}

int	execute_builtin_by_id(t_data *data)
{
	t_builtin_type	type;

	type = data->cmd->builtin_id;
	if (type == BUILTIN_CD)
		return (builtin_cd(data));
	if (type == BUILTIN_PWD)
		return (builtin_pwd(data));
	if (type == BUILTIN_EXIT)
		return (builtin_exit(data));
	if (type == BUILTIN_ECHO)
		return (builtin_echo(data));
	if (type == BUILTIN_ENV)
		return (builtin_env(data));
	if (type == BUILTIN_EXPORT)
		return (builtin_export(data));
	if (type == BUILTIN_UNSET)
		return (builtin_unset(data));
	printf("\nUnknown builtin\n");
	return (1);
}

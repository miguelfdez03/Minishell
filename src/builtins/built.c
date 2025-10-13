/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:06:29 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/13 12:22:45 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_command(t_data *data)
{
	t_cmd *cmd;

	cmd = data->cmd;
	if (cmd->type == CMD_EXTERNAL)
		printf("external command \n");
	else
		execute_builtin_by_id(data);
	return (0);
}

// Ejecutar built-in
static int	handle_basic_builtins(t_data *data)
{
	t_builtin_type type;

	type = data->cmd->builtin_id;
	if (type == BUILTIN_CD)
		return (builtin_cd(data));
	else if (type == BUILTIN_PWD)
		return (builtin_pwd(data));
	else if (type == BUILTIN_EXIT)
		return (builtin_exit(data));
	else if (type == BUILTIN_ECHO)
		return (builtin_echo(data));
	return (-1);
}

static int	handle_env_builtins(t_data *data)
{
	t_builtin_type type;

	type = data->cmd->builtin_id;
	if (type == BUILTIN_ENV)
		return (builtin_env(data));
	else if (type == BUILTIN_EXPORT)
		return (printf("\nbuiltin export\n"), 0);
	else if (type == BUILTIN_UNSET)
		return (printf("\nbuiltin unset\n"), 0);
	return (-1);
}

int	execute_builtin_by_id(t_data *data)
{
	int	result;
	t_cmd	*cmd;

	cmd = data->cmd;
	result = handle_basic_builtins(data);
	if (result != -1)
		return (result);
	result = handle_env_builtins(data);
	if (result != -1)
		return (result);
	printf("\nUnknown builtin\n");
	return (1);
}

int	builtin_pwd(t_data *data)
{
	t_cmd	*cmd = data->cmd;
	char	*pwd;

	if (cmd->args && cmd->args[1])
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putendl_fd("pwd: error getting current directory", 2);
		return (1);
	}
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (0);
}

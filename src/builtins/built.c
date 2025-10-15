/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:06:29 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/15 13:27:02 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_command(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmd;
	if (cmd->type == CMD_EXTERNAL)
		printf("external command \n");
	else
		execute_builtin_by_id(data);
	return (0);
}

static int	handle_basic_builtins(t_data *data)
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
	return (-1);
}

static int	handle_env_builtins(t_data *data)
{
	t_builtin_type	type;

	type = data->cmd->builtin_id;
	if (type == BUILTIN_ENV)
		return (builtin_env(data));
	if (type == BUILTIN_EXPORT)
		return (printf("\nbuiltin export\n"), 0);
	if (type == BUILTIN_UNSET)
		return (printf("\nbuiltin unset\n"), 0);
	return (-1);
}

int	execute_builtin_by_id(t_data *data)
{
	int	result;

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
	t_cmd	*cmd;
	char	*pwd;

	cmd = data->cmd;
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

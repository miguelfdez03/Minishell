/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:40:48 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:57:58 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_cd_error(t_data *data, char *path)
{
	if (data->cmd->args && data->cmd->args[0]
		&& ft_strcmp2(data->cmd->args[0], "-") == 0)
		ft_putendl_fd("cd: OLDPWD not set", 2);
	else
		ft_putendl_fd("cd: HOME not set", 2);
	return (1);
}

static int	execute_cd(t_data *data, char *path)
{
	char	*oldpwd;
	char	*newpwd;

	if (data->cmd->args && data->cmd->args[0]
		&& ft_strcmp2(data->cmd->args[0], "-") == 0)
		ft_putendl_fd(path, 1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		perror("cd");
		if (oldpwd)
			free(oldpwd);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	update_env_pwd(data, oldpwd, newpwd);
	return (0);
}

int	builtin_cd(t_data *data)
{
	char	*path;

	if (!data || !data->cmd)
		return (1);
	if (count_args(data->cmd->args) > 1)
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (2);
	}
	path = get_cd_path(data->cmd, data->env);
	if (!path)
		return (handle_cd_error(data, path));
	return (execute_cd(data, path));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:54:11 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/25 23:45:15 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_cd(t_data *data)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;

	if (!data || !data->cmd)
		return (1);
	path = get_cd_path(data->cmd, data->env);
	if (!path)
	{
		if (!data->cmd->args || count_args(data->cmd->args) <= 1)
			ft_putendl_fd("cd: HOME not set", 2);
		else if (ft_strcmp2(data->cmd->args[0], "-") == 0)
			ft_putendl_fd("cd: OLDPWD not set", 2);
		return (1);
	}
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
	if (data->cmd->args && data->cmd->args[0] 
		&& ft_strcmp2(data->cmd->args[0], "-") == 0)
		ft_putendl_fd(get_env_value_from_list(data->env, "PWD"), 1);
	return (0);
}

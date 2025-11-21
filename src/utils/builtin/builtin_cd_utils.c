/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:18 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:42:19 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_args(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

char	*get_cd_path(t_cmd *cmd, t_env *env)
{
	if (!cmd->args || count_args(cmd->args) == 0)
		return (get_env_value_from_list(env, "HOME"));
	if (ft_strcmp2(cmd->args[0], "~") == 0)
		return (get_env_value_from_list(env, "HOME"));
	if (ft_strcmp2(cmd->args[0], "-") == 0)
		return (get_env_value_from_list(env, "OLDPWD"));
	return (cmd->args[0]);
}

int	do_chdir(char *path, char *oldpwd)
{
	if (chdir(path) == -1)
	{
		perror("cd");
		if (oldpwd)
			free(oldpwd);
		return (1);
	}
	return (0);
}

void	update_env_pwd(t_data *data, char *oldpwd, char *newpwd)
{
	if (oldpwd)
		set_env_value(&data->env, "OLDPWD", oldpwd);
	if (newpwd)
		set_env_value(&data->env, "PWD", newpwd);
	if (oldpwd)
		free(oldpwd);
	if (newpwd)
		free(newpwd);
}

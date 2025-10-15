/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:00:00 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/15 16:37:23 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	int	argc;

	argc = count_args(cmd->args);
	if (argc == 1 || (argc == 2 && ft_strcmp2(cmd->args[1], "~") == 0))
		return (get_env_value_from_list(env, "HOME"));
	else if (argc == 2 && ft_strcmp2(cmd->args[1], "-") == 0)
		return (get_env_value_from_list(env, "OLDPWD"));
	else if (argc == 2)
		return (cmd->args[1]);
	return (NULL);
}

int	handle_cd_error(char *path, int argc, char **args)
{
	if (!path)
	{
		if (argc == 2 && ft_strcmp2(args[1], "-") == 0)
			ft_putendl_fd("cd: OLDPWD not set", 2);
		else
			ft_putendl_fd("cd: HOME not set", 2);
		return (1);
	}
	return (0);
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

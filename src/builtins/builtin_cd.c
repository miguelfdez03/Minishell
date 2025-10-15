/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:54:11 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/15 16:37:23 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_cd(t_data *data)
{
	t_cmd	*cmd;
	char	*path;
	char	*oldpwd;
	char	*newpwd;
	int		argc;

	cmd = data->cmd;
	argc = count_args(cmd->args);
	if (argc > 2)
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	path = get_cd_path(cmd, data->env);
	if (handle_cd_error(path, argc, cmd->args))
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (do_chdir(path, oldpwd))
		return (1);
	newpwd = getcwd(NULL, 0);
	update_env_pwd(data, oldpwd, newpwd);
	if (argc == 2 && ft_strcmp2(cmd->args[1], "-") == 0)
		ft_putendl_fd(get_env_value_from_list(data->env, "PWD"), 1);
	return (0);
}

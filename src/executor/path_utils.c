/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:07:11 by miguel-f          #+#    #+#             */
/*   Updated: 2025/11/03 22:23:54 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_command_path(char *cmd, char **envp)
{
	char	*path_env;
	char	*path_copy;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value("PATH", envp);
	if (!path_env)
		return (NULL);
	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	return (search_in_path_dirs(path_copy, cmd));
}

char	*search_in_path_dirs(char *path_copy, char *cmd)
{
	char	**path_dirs;
	char	*full_path;
	int		i;

	path_dirs = ft_split(path_copy, ':');
	free(path_copy);
	if (!path_dirs)
		return (NULL);
	i = 0;
	while (path_dirs[i])
	{
		full_path = build_full_path(path_dirs[i], cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_string_array(path_dirs);
			return (full_path);
		}
		if (full_path)
			free(full_path);
		i++;
	}
	free_string_array(path_dirs);
	return (NULL);
}

char	*get_env_value(char *var_name, char **envp)
{
	int	i;
	int	len;

	i = 0;
	if (!var_name || !envp)
		return (NULL);
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*build_full_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	if (!dir || !cmd)
		return (NULL);
	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

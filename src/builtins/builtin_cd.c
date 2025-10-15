/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:54:11 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/15 13:26:53 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Función para contar argumentos
static int	count_args(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

// Función para obtener valor de variable de entorno de la lista
static char	*get_env_value_from_list(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp2(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

// Función para obtener el path destino del cd
static char	*get_cd_path(t_cmd *cmd, t_env *env)
{
	int	argc;

	argc = count_args(cmd->args);
	if (argc == 1 || (argc == 2 && ft_strcmp2(cmd->args[1], "~") == 0))
		return (get_env_value_from_list(env, "HOME"));
	else if (argc == 2)
		return (cmd->args[1]);
	return (NULL);
}

int	builtin_cd(t_data *data)
{
	t_cmd	*cmd;
	char	*path;
	int		argc;

	cmd = data->cmd;
	argc = count_args(cmd->args);
	if (argc > 2)
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	path = get_cd_path(cmd, data->env);
	if (!path)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		return (1);
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	ft_printf("Changed to: %s\n", path);
	return (0);
}

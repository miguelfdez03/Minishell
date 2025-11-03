/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 19:00:00 by miguel-f          #+#    #+#             */
/*   Updated: 2025/11/03 20:02:06 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Convierte la lista enlazada de variables de entorno a array de strings
 * Necesario para execve que requiere char **envp
 */
static char	**env_list_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	char	*temp;
	int		count;
	int		i;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		temp = ft_strjoin(current->key, "=");
		env_array[i] = ft_strjoin(temp, current->value);
		free(temp);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

/*
 * Construye el array de argumentos para execve
 * Formato: [nombre_comando, arg1, arg2, ..., NULL]
 */
static char	**build_args_array(t_cmd *cmd)
{
	char	**args;
	int		count;
	int		i;

	count = 1;
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			count++;
			i++;
		}
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	args[0] = cmd->name;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		args[i + 1] = cmd->args[i];
		i++;
	}
	args[count] = NULL;
	return (args);
}

/*
 * Busca el comando en PATH usando la lista de env de minishell
 */
static char	*find_cmd_in_path(char *cmd, t_env *env)
{
	char	*path_value;
	char	**env_array;
	char	*cmd_path;

	if (!cmd || !env)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	env_array = env_list_to_array(env);
	if (!env_array)
		return (NULL);
	path_value = get_env_value("PATH", env_array);
	if (!path_value)
	{
		free_string_array(env_array);
		return (NULL);
	}
	cmd_path = find_command_path(cmd, env_array);
	free_string_array(env_array);
	return (cmd_path);
}

/*
 * Ejecuta un comando externo usando fork + execve
 * Basado en la implementación de pipex-42
 */
int	execute_external_command(t_data *data)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**args;
	char	**env_array;

	cmd_path = find_cmd_in_path(data->cmd->name, data->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(data->cmd->name, 2);
		return (127);
	}
	args = build_args_array(data->cmd);
	env_array = env_list_to_array(data->env);
	if (!args || !env_array)
	{
		free(cmd_path);
		if (args)
			free(args);
		if (env_array)
			free_string_array(env_array);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free(cmd_path);
		free(args);
		free_string_array(env_array);
		return (1);
	}
	if (pid == 0)
	{
		if (execve(cmd_path, args, env_array) == -1)
		{
			perror("minishell: execve");
			free(cmd_path);
			free(args);
			free_string_array(env_array);
			exit(127);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		free(args);
		free_string_array(env_array);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:28 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:18:54 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: count_env_vars
 * ----------------------
 * Cuenta cuántas variables de entorno hay en la lista.
 * 
 * Recorre toda la lista enlazada contando los nodos.
 * 
 * env: Lista de variables de entorno
 * 
 * Retorna: Número de variables
 */
int	count_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
 * Función: env_list_to_array
 * -------------------------
 * Convierte la lista de variables de entorno a array de strings.
 * 
 * Proceso:
 * 1. Cuenta variables y reserva memoria para array
 * 2. Para cada variable con valor:
 *    - Construye string "KEY=value"
 *    - Lo añade al array
 * 3. Añade NULL al final del array
 * 
 * Formato de salida: ["PATH=/usr/bin", "HOME=/home/user", NULL]
 * 
 * env: Lista de variables de entorno
 * 
 * Retorna: Array de strings, NULL si falla malloc
 */
char	**env_list_to_array(t_env *env)
{
	char	**env_array;
	char	*temp;
	int		i;

	env_array = malloc(sizeof(char *) * (count_env_vars(env) + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value)
		{
			temp = ft_strjoin(env->key, "=");
			env_array[i] = ft_strjoin(temp, env->value);
			free(temp);
			i++;
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

/*
 * Función: count_cmd_args
 * ----------------------
 * Cuenta cuántos argumentos totales tiene el comando.
 * 
 * Cuenta:
 * 1. El nombre del comando (+1)
 * 2. Todos los argumentos en cmd->args
 * 
 * Ejemplo: "ls -la /home" -> count = 3 (ls, -la, /home)
 * 
 * cmd: Comando con sus argumentos
 * 
 * Retorna: Número total de elementos
 */
static int	count_cmd_args(t_cmd *cmd)
{
	int	count;
	int	i;

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
	return (count);
}

/*
 * Función: build_args_array
 * ------------------------
 * Construye el array de argumentos para execve.
 * 
 * Formato requerido por execve:
 * args[0] = nombre del comando
 * args[1..n] = argumentos
 * args[n+1] = NULL
 * 
 * Ejemplo:
 * cmd->name = "ls"
 * cmd->args = ["-la", "/home", NULL]
 * Resultado: ["ls", "-la", "/home", NULL]
 * 
 * cmd: Comando con nombre y argumentos
 * 
 * Retorna: Array de strings, NULL si falla malloc
 */
char	**build_args_array(t_cmd *cmd)
{
	char	**args;
	int		count;
	int		i;

	count = count_cmd_args(cmd);
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
 * Función: check_absolute_path
 * ---------------------------
 * Verifica si el comando es una ruta absoluta o relativa.
 * 
 * Rutas válidas: "/bin/ls", "./programa", ".", ".."
 * 
 * Validaciones:
 * 1. Verifica si el path existe con stat()
 * 2. Si es directorio: error "Is a directory" -> retorna (char *)-1
 * 3. Si no tiene permisos de ejecución: error "Permission denied" -> (char *)-1
 * 4. Si todo ok: retorna duplicado del comando
 * 
 * cmd: Comando a verificar
 * 
 * Retorna: Duplicado del comando si válido,
 *          (char *)-1 si es directorio o sin permisos,
 *          NULL si no es ruta absoluta/relativa
 */
char	*check_absolute_path(char *cmd)
{
	struct stat	path_stat;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/')
		|| ft_strcmp2(cmd, ".") == 0 || ft_strcmp2(cmd, "..") == 0)
	{
		if (stat(cmd, &path_stat) != 0)
			return (NULL);
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Is a directory", 2);
			return ((char *)-1);
		}
		if (access(cmd, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Permission denied", 2);
			return ((char *)-1);
		}
		return (ft_strdup(cmd));
	}
	return (NULL);
}

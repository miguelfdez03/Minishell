/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:38 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:18:54 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: find_command_path
 * -------------------------
 * Busca la ruta completa de un comando en PATH.
 * 
 * Lógica:
 * 1. Si empieza con / o ./: verifica si es ejecutable y lo retorna
 * 2. Si no: busca en variable PATH
 * 3. Duplica PATH y busca en cada directorio
 * 
 * cmd: Nombre del comando
 * envp: Array de variables de entorno
 * 
 * Retorna: Ruta completa del comando o NULL si no se encuentra
 */
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

/*
 * Función: search_in_path_dirs
 * ----------------------------
 * Busca el comando en cada directorio de PATH.
 * 
 * Proceso:
 * 1. Divide PATH en directorios usando ':' como separador
 *    Ejemplo: "/usr/bin:/bin:/usr/local/bin"
 * 2. Para cada directorio:
 *    - Construye ruta completa: dir + "/" + cmd
 *    - Verifica si es ejecutable con access()
 *    - Si sí: lo retorna
 * 3. Si no encuentra en ningún directorio: retorna NULL
 * 
 * path_copy: Copia de PATH (se libera al final)
 * cmd: Nombre del comando a buscar
 * 
 * Retorna: Ruta completa del comando o NULL
 */
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

/*
 * Función: get_env_value
 * ---------------------
 * Obtiene el valor de una variable de entorno del array.
 * 
 * Busca "VARIABLE=valor" y retorna el valor.
 * 
 * Ejemplo:
 * envp = ["PATH=/usr/bin:/bin", "HOME=/home/user", NULL]
 * get_env_value("PATH", envp) -> "/usr/bin:/bin"
 * 
 * var_name: Nombre de la variable a buscar
 * envp: Array de variables de entorno
 * 
 * Retorna: Puntero al valor (después del '=') o NULL si no existe
 */
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

/*
 * Función: build_full_path
 * -----------------------
 * Construye la ruta completa concatenando directorio y comando.
 * 
 * Ejemplo:
 * dir = "/usr/bin"
 * cmd = "ls"
 * Resultado: "/usr/bin/ls"
 * 
 * 1. Une dir + "/"
 * 2. Une resultado + cmd
 * 3. Libera temporales
 * 
 * dir: Directorio base
 * cmd: Nombre del comando
 * 
 * Retorna: Ruta completa (debe liberarse) o NULL si falla malloc
 */
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

/*
 * Función: free_string_array
 * -------------------------
 * Libera un array de strings.
 * 
 * Proceso:
 * 1. Libera cada string individual
 * 2. Libera el array en sí
 * 
 * Se usa para liberar arrays como env_array, args, path_dirs.
 * 
 * array: Array de strings terminado en NULL
 */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:07:11 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/02 12:07:14 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Función para encontrar la ruta completa de un comando
char	*find_command_path(char *cmd, char **envp)
{
	char	*path_env;
	char	*path_copy;
	char	*dir;
	char	*full_path;

	ft_printf("🔍 Searching for command: %s\n", cmd);
	// Si el comando ya es una ruta absoluta o relativa
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
		{
			ft_printf("✅ Command is already a full path: %s\n", cmd);
			return (ft_strdup(cmd));
		}
		ft_printf("❌ Path not executable: %s\n", cmd);
		return (NULL);
	}
	// Buscar PATH en las variables de entorno
	path_env = get_env_value("PATH", envp);
	if (!path_env)
	{
		ft_printf("❌ PATH not found in environment\n");
		return (NULL);
	}
	ft_printf("🕸️ PATH found\n"); // Simplificado para evitar problemas con %.50s
	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	// Recorrer cada directorio del PATH usando ft_split en lugar de strtok
	return (search_in_path_dirs(path_copy, cmd));
}

// Función auxiliar para buscar en directorios del PATH
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
			ft_printf("✅ Found command at: %s\n", full_path);
			free_string_array(path_dirs);
			return (full_path);
		}
		if (full_path)
			free(full_path);
		i++;
	}
	free_string_array(path_dirs);
	ft_printf("❌ Command not found in PATH: %s\n", cmd);
	return (NULL);
}

// Función auxiliar para obtener valor de variable de entorno
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

// Función auxiliar para construir ruta completa
char	*build_full_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	if (!dir || !cmd)
		return (NULL);
	// Usar ft_strjoin para concatenar dir + "/"
	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	// Usar ft_strjoin para concatenar temp + cmd
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

// Función auxiliar para liberar array de strings
void	free_string_array(char **array)
{
	int i;

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
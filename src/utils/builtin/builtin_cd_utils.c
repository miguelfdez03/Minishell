/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:18 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Función: count_args
 * -------------------
 * Cuenta cuántos argumentos hay en el array.
 * 
 * Recorre el array hasta encontrar NULL.
 * 
 * args: Array de cadenas terminado en NULL
 * 
 * Retorna: Número de argumentos
 */
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

/*
 * Función: get_cd_path
 * --------------------
 * Determina qué ruta debe usar cd según el argumento.
 * 
 * Lógica:
 * 1. Sin argumentos: va a HOME
 * 2. "~": va a HOME
 * 3. "-": va a OLDPWD (directorio anterior)
 * 4. Cualquier otra cosa: usa ese path directamente
 * 
 * Si HOME u OLDPWD no existen, retorna NULL.
 * 
 * cmd: Comando con los argumentos
 * env: Lista de variables de entorno
 * 
 * Retorna: Ruta a donde ir o NULL si hay error
 */
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

/*
 * Función: do_chdir
 * -----------------
 * Ejecuta el cambio de directorio usando chdir().
 * 
 * Intenta cambiar al directorio especificado:
 * - Si tiene éxito: retorna 0
 * - Si falla: muestra error, libera oldpwd y retorna 1
 * 
 * path: Ruta al directorio destino
 * oldpwd: Puntero a liberar en caso de error
 * 
 * Retorna: 0 si éxito, 1 si error
 */
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

/*
 * Función: update_env_pwd
 * -----------------------
 * Actualiza las variables PWD y OLDPWD después de hacer cd.
 * 
 * Proceso:
 * 1. Si hay oldpwd, actualiza OLDPWD con ese valor
 * 2. Si hay newpwd, actualiza PWD con ese valor
 * 3. Libera oldpwd y newpwd
 * 
 * data: Estructura con la lista de variables de entorno
 * oldpwd: Valor del directorio anterior
 * newpwd: Valor del nuevo directorio actual
 */
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

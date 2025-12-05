/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:40:48 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: handle_cd_error
 * -------------------------
 * Maneja los errores cuando cd no puede obtener el directorio destino.
 * 
 * 1. Verifica si el comando fue 'cd -' (OLDPWD no existe)
 * 2. Si no, asume que fue 'cd' sin argumentos (HOME no existe)
 * 3. Imprime el mensaje de error correspondiente
 * 
 * data: Estructura principal del minishell
 * path: Ruta que se intentó obtener (puede ser NULL)
 * 
 * Retorna: 1 (código de error)
 */
static int	handle_cd_error(t_data *data, char *path)
{
	if (data->cmd->args && data->cmd->args[0]
		&& ft_strcmp2(data->cmd->args[0], "-") == 0)
		ft_putendl_fd("cd: OLDPWD not set", 2);
	else
		ft_putendl_fd("cd: HOME not set", 2);
	return (1);
}

/*
 * Función: execute_cd
 * -------------------
 * Ejecuta el cambio de directorio y actualiza las variables de entorno.
 * 
 * 1. Si el comando fue 'cd -', imprime la ruta del OLDPWD
 * 2. Guarda el directorio actual (futuro OLDPWD)
 * 3. Ejecuta chdir() para cambiar al directorio destino
 * 4. Si falla, muestra el error del sistema y libera memoria
 * 5. Si tiene éxito, obtiene el nuevo directorio (futuro PWD)
 * 6. Actualiza las variables PWD y OLDPWD en el entorno
 * 
 * data: Estructura principal del minishell
 * path: Ruta del directorio de destino
 * 
 * Retorna: 0 si tiene éxito, 1 si falla
 */
static int	execute_cd(t_data *data, char *path)
{
	char	*oldpwd;
	char	*newpwd;

	if (data->cmd->args && data->cmd->args[0]
		&& ft_strcmp2(data->cmd->args[0], "-") == 0)
		ft_putendl_fd(path, 1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		perror("cd");
		if (oldpwd)
			free(oldpwd);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	update_env_pwd(data, oldpwd, newpwd);
	return (0);
}

/*
 * Función: builtin_cd
 * ------------------
 * Implementa el comando cd (change directory) del shell.
 * 
 * 1. Valida que no haya más de un argumento
 * 2. Obtiene el directorio destino:
 *    - Sin argumentos: va a HOME
 *    - Argumento '-': va a OLDPWD
 *    - Otro argumento: va a ese directorio
 * 3. Si no puede obtener el directorio, muestra error
 * 4. Ejecuta el cambio de directorio
 * 
 * data: Estructura principal del minishell
 * 
 * Retorna: 0 si tiene éxito, 1 si falla, 2 si hay demasiados argumentos
 */
int	builtin_cd(t_data *data)
{
	char	*path;

	if (!data || !data->cmd)
		return (1);
	if (count_args(data->cmd->args) > 1)
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (2);
	}
	path = get_cd_path(data->cmd, data->env);
	if (!path)
		return (handle_cd_error(data, path));
	return (execute_cd(data, path));
}

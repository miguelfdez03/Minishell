/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:40:42 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:15:26 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: execute_command
 * -----------------------
 * Función principal para ejecutar cualquier comando.
 * 
 * Lógica de decisión:
 * 1. Si no hay nombre ni args:
 *    - Si hay redirecciones: las procesa sin comando
 *    - Si no: retorna 0 (no hace nada)
 * 2. Si no hay nombre:
 *    - Maneja comando vacío (puede ser error o asignación)
 * 3. Si hay pipes (cmd->next):
 *    - Ejecuta pipeline completo
 * 4. Si no:
 *    - Ejecuta comando único
 * 
 * data: Estructura con el comando a ejecutar
 * 
 * Retorna: Código de salida de la ejecución
 */
int	execute_command(t_data *data)
{
	t_cmd	*cmd;
	int		exit_status;

	cmd = data->cmd;
	if ((!cmd->name || cmd->name[0] == '\0') && (!cmd->args || !cmd->args[0]))
	{
		if (cmd->redirections)
			return (process_redirections_only(data));
		return (0);
	}
	if (!cmd->name || cmd->name[0] == '\0')
	{
		exit_status = handle_empty_cmd(cmd, data);
		if (exit_status != -1)
			return (exit_status);
	}
	if (cmd->next)
		return (execute_pipeline(data));
	return (execute_single_command(data, cmd));
}

/*
 * Función: execute_builtin_by_id
 * -----------------------------
 * Ejecuta el builtin correspondiente según su ID.
 * 
 * Switch/dispatcher que llama a la función correcta según
 * el tipo de builtin identificado previamente.
 * 
 * Builtins soportados:
 * - cd, pwd, exit, echo, env, export, unset
 * 
 * Si el ID no corresponde a ninguno, muestra error.
 * 
 * data: Estructura con el comando y el builtin_id
 * 
 * Retorna: Código de salida del builtin ejecutado
 */
int	execute_builtin_by_id(t_data *data)
{
	t_builtin_type	type;

	type = data->cmd->builtin_id;
	if (type == BUILTIN_CD)
		return (builtin_cd(data));
	if (type == BUILTIN_PWD)
		return (builtin_pwd(data));
	if (type == BUILTIN_EXIT)
		return (builtin_exit(data));
	if (type == BUILTIN_ECHO)
		return (builtin_echo(data));
	if (type == BUILTIN_ENV)
		return (builtin_env(data));
	if (type == BUILTIN_EXPORT)
		return (builtin_export(data));
	if (type == BUILTIN_UNSET)
		return (builtin_unset(data));
	printf("\nUnknown builtin\n");
	return (1);
}

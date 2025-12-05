/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:40:44 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: execute_builtin_with_redir
 * -----------------------------------
 * Ejecuta un comando builtin aplicando sus redirecciones.
 * 
 * Proceso:
 * 1. Procesa todos los heredocs primero
 * 2. Guarda los descriptores originales de stdin y stdout
 * 3. Aplica las redirecciones
 * 4. Ejecuta el builtin
 * 5. Restaura los descriptores originales
 * 6. Cierra los descriptores guardados
 * 
 * Esto permite que los builtins escriban/lean donde corresponda
 * sin afectar al shell después de la ejecución.
 * 
 * data: Estructura con el comando y sus datos
 * 
 * Retorna: Código de salida del builtin
 */
static int	execute_builtin_with_redir(t_data *data)
{
	int	exit_status;
	int	saved_stdin;
	int	saved_stdout;

	if (process_all_heredocs(data) != 0)
		return (data->exit_status);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(data) == -1)
		exit_status = 1;
	else
		exit_status = execute_builtin_by_id(data);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (exit_status);
}

/*
 * Función: process_redirections_only
 * ----------------------------------
 * Procesa redirecciones sin ejecutar ningún comando.
 * 
 * Usado cuando hay redirecciones pero no hay comando válido.
 * 
 * Proceso:
 * 1. Procesa todos los heredocs
 * 2. Guarda stdin/stdout originales
 * 3. Aplica las redirecciones (abre archivos, verifica permisos)
 * 4. Restaura stdin/stdout originales
 * 5. Cierra descriptores guardados
 * 
 * data: Estructura con las redirecciones a procesar
 * 
 * Retorna: 0 si éxito, 1 si error en redirecciones
 */
int	process_redirections_only(t_data *data)
{
	int	exit_status;
	int	saved_stdin;
	int	saved_stdout;

	if (process_all_heredocs(data) != 0)
		return (data->exit_status);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	exit_status = apply_redirections(data);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	if (exit_status == -1)
		return (1);
	return (0);
}

/*
 * Función: handle_empty_cmd
 * -------------------------
 * Maneja el caso de comando vacío o sin argumentos.
 * 
 * Casos:
 * 1. Si no hay args y hay pipes o redirecciones:
 *    - Muestra ": command not found"
 *    - Retorna 127 (comando no encontrado)
 * 2. Si no hay args ni pipes/redirecciones:
 *    - No hace nada, retorna 0
 * 3. Si hay args:
 *    - Copia el primer argumento como nombre del comando
 *    - Identifica si es un builtin
 *    - Retorna -1 para indicar que hay que ejecutar
 * 
 * cmd: Comando a verificar
 * data: Estructura para actualizar exit_status
 * 
 * Retorna: 0 si comando vacío, 127 si error, -1 si hay comando
 */
int	handle_empty_cmd(t_cmd *cmd, t_data *data)
{
	if (!cmd->args || !cmd->args[0])
	{
		if (cmd->next || cmd->redirections)
		{
			ft_putendl_fd(": command not found", 2);
			data->exit_status = 127;
			return (data->exit_status);
		}
		return (0);
	}
	cmd->name = ft_strdup(cmd->args[0]);
	cmd->builtin_id = identify_builtin(cmd->name);
	return (-1);
}

/*
 * Función: execute_single_command
 * ------------------------------
 * Ejecuta un único comando (sin pipes).
 * 
 * Decide si el comando es:
 * 1. Externo (ejecutable): ejecuta con execute_external_command()
 * 2. Builtin: ejecuta con execute_builtin_with_redir()
 * 
 * Actualiza el exit_status en data y resetea g_signal_received.
 * 
 * data: Estructura con el comando
 * cmd: Comando a ejecutar
 * 
 * Retorna: Código de salida del comando
 */
int	execute_single_command(t_data *data, t_cmd *cmd)
{
	int	exit_status;

	if (cmd->builtin_id == BUILTIN_NONE)
	{
		exit_status = execute_external_command(data);
		data->exit_status = exit_status;
		return (exit_status);
	}
	exit_status = execute_builtin_with_redir(data);
	g_signal_received = 0;
	data->exit_status = exit_status;
	return (exit_status);
}

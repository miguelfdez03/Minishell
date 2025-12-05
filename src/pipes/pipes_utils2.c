/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:09 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:32:34 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: cleanup_and_exit
 * -------------------------
 * Limpia memoria y sale del proceso hijo.
 * 
 * Usado en procesos hijo de pipeline para salir limpiamente:
 * 1. Restaura puntero al comando original
 * 2. Limpia historial de readline
 * 3. Libera toda la memoria de data
 * 4. Sale con el código especificado
 * 
 * data: Estructura a liberar
 * original_cmd: Comando original (para restaurar puntero)
 * code: Código de salida
 */
void	cleanup_and_exit(t_data *data, t_cmd *original_cmd, int code)
{
	data->cmd = original_cmd;
	rl_clear_history();
	free_data(data);
	exit(code);
}

/*
 * Función: exec_external_cmd
 * -------------------------
 * Ejecuta comando externo en proceso hijo de pipeline.
 * 
 * Proceso:
 * 1. Verifica que haya nombre de comando
 * 2. Busca comando en PATH
 * 3. Si no existe o no es ejecutable: sale con error
 * 4. Construye array de argumentos y env
 * 5. Ejecuta con execve
 * 6. Si execve falla: muestra error y sale
 * 
 * Siempre termina con exit(), nunca retorna.
 * 
 * data: Estructura del shell
 * cmd: Comando a ejecutar
 * original_cmd: Comando original (para cleanup)
 */
void	exec_external_cmd(t_data *data, t_cmd *cmd, t_cmd *original_cmd)
{
	char	*cmd_path;
	char	**args;
	char	**env_array;

	if (!cmd->name || cmd->name[0] == '\0')
	{
		ft_putendl_fd(": command not found", 2);
		cleanup_and_exit(data, original_cmd, 127);
	}
	cmd_path = find_cmd_in_path(cmd->name, data->env);
	if (cmd_path == (char *)-1)
		cleanup_and_exit(data, original_cmd, 126);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->name, 2);
		cleanup_and_exit(data, original_cmd, 127);
	}
	args = build_args_array(cmd);
	env_array = env_list_to_array(data->env);
	if (!args || !env_array)
		cleanup_and_exit(data, original_cmd, 1);
	execve(cmd_path, args, env_array);
	perror("minishell: execve");
	cleanup_and_exit(data, original_cmd, 127);
}

/*
 * Función: collect_children
 * ------------------------
 * Espera a todos los procesos hijo del pipeline.
 * 
 * Proceso:
 * 1. Espera cualquier proceso hijo con waitpid(-1)
 * 2. Si es el último comando:
 *    - Guarda su exit status
 *    - Si terminó por señal: calcula 128 + número de señal
 * 3. Continúa hasta que no queden hijos
 * 
 * Solo el exit status del último comando importa para el shell.
 * 
 * last_cmd_pid: PID del último comando en pipeline
 * exit_status: Salida - código de salida del último comando
 * last_signal: Salida - señal que terminó el último comando (0 si ninguna)
 */
static void	collect_children(
	pid_t last_cmd_pid, int *exit_status, int *last_signal)
{
	int		status;
	pid_t	current_pid;

	*last_signal = 0;
	while (1)
	{
		current_pid = waitpid(-1, &status, 0);
		if (current_pid <= 0)
			break ;
		if (current_pid == last_cmd_pid)
		{
			if (WIFEXITED(status))
				*exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				*exit_status = 128 + WTERMSIG(status);
				*last_signal = WTERMSIG(status);
			}
		}
	}
}

/*
 * Función: wait_all_processes
 * --------------------------
 * Espera a todos los procesos y maneja señales.
 * 
 * Proceso:
 * 1. Espera a todos los hijos con collect_children()
 * 2. Si el último comando terminó con:
 *    - SIGINT (Ctrl+C): imprime newline
 *    - SIGQUIT (Ctrl+\): imprime "Quit (core dumped)"
 * 
 * exit_status: Puntero donde guardar exit status final
 * last_cmd_pid: PID del último comando
 * 
 * Retorna: Exit status del último comando
 */
int	wait_all_processes(int *exit_status, pid_t last_cmd_pid)
{
	int	last_signal;

	collect_children(last_cmd_pid, exit_status, &last_signal);
	if (last_signal == SIGINT)
		write(1, "\n", 1);
	else if (last_signal == SIGQUIT)
		write(2, "Quit (core dumped)\n", 19);
	return (*exit_status);
}

/*
 * Función: exec_cmd_in_child
 * -------------------------
 * Ejecuta comando en proceso hijo de pipeline.
 * 
 * Proceso:
 * 1. Guarda comando original
 * 2. Configura señales para proceso hijo
 * 3. Aplica redirecciones del comando
 * 4. Cierra todos los descriptores no necesarios
 * 5. Si es builtin: ejecuta y sale
 * 6. Si es externo: ejecuta con execve
 * 
 * Siempre termina con exit(), nunca retorna.
 * 
 * data: Estructura del shell
 * cmd: Comando a ejecutar
 * is_last_cmd: 1 si es el último comando del pipeline
 */
void	exec_cmd_in_child(t_data *data, t_cmd *cmd, int is_last_cmd)
{
	int		exit_code;
	t_cmd	*original_cmd;

	original_cmd = data->cmd;
	setup_signals_child();
	data->cmd = cmd;
	if (apply_redirections(data) == -1)
		cleanup_and_exit(data, original_cmd, 1);
	close_all_fds();
	if (cmd->builtin_id != BUILTIN_NONE)
	{
		exit_code = execute_builtin_by_id(data);
		cleanup_and_exit(data, original_cmd, exit_code);
	}
	exec_external_cmd(data, cmd, original_cmd);
}

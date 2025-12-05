/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:31 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:18:54 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: handle_fork_error
 * -------------------------
 * Maneja el error cuando fork() falla.
 * 
 * Cuando no se puede crear proceso hijo:
 * 1. Muestra error con perror
 * 2. Libera toda la memoria reservada (cmd_path, args, env_array)
 * 3. Restaura señales a modo interactivo
 * 
 * cmd_path: Ruta del comando a liberar
 * args: Array de argumentos a liberar
 * env_array: Array de variables de entorno a liberar
 * 
 * Retorna: 1 (código de error)
 */
int	handle_fork_error(char *cmd_path, char **args, char **env_array)
{
	perror("minishell: fork");
	free(cmd_path);
	free(args);
	free_string_array(env_array);
	setup_signals_interactive();
	return (1);
}

/*
 * Función: wait_and_cleanup
 * ------------------------
 * Espera a que termine el proceso hijo y limpia recursos.
 * 
 * Proceso:
 * 1. Espera al proceso hijo con waitpid
 * 2. Libera toda la memoria (cmd_path, args, env_array)
 * 3. Analiza cómo terminó el proceso:
 *    - WIFEXITED: terminó normalmente -> retorna exit status
 *    - WIFSIGNALED: terminó por señal:
 *      * SIGINT (Ctrl+C): imprime newline
 *      * SIGQUIT (Ctrl+\): imprime "Quit (core dumped)"
 *      * Retorna 128 + número de señal
 * 
 * pid: PID del proceso hijo
 * cmd_path: Ruta del comando a liberar
 * args: Array de argumentos a liberar
 * env_array: Array de variables de entorno a liberar
 * 
 * Retorna: Código de salida del proceso hijo
 */
int	wait_and_cleanup(pid_t pid, char *cmd_path, char **args,
		char **env_array)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	free(args);
	free_string_array(env_array);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

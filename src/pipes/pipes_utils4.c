/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:13 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:32:34 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: process_next_cmd_token
 * -------------------------------
 * Procesa el primer token válido del siguiente comando.
 * 
 * Proceso:
 * 1. Inicializa nombre del siguiente comando con el token
 * 2. Si falla: libera next_cmd y retorna error
 * 3. Marca el token como EMPTY (ya procesado)
 * 4. Avanza tokens al siguiente
 * 
 * current_cmd: Comando actual (tiene next_cmd ya creado)
 * tmp: Token con el nombre del comando
 * tokens: Puntero a token actual (se actualiza)
 * 
 * Retorna: 0 si éxito, -1 si error
 */
static int	process_next_cmd_token(t_cmd *current_cmd, t_token *tmp,
		t_token **tokens)
{
	if (init_next_cmd_name(current_cmd->next, tmp) == -1)
	{
		current_cmd->next = NULL;
		return (-1);
	}
	tmp->type = EMPTY;
	*tokens = tmp->next;
	return (0);
}

/*
 * Función: init_next_cmd
 * --------------------
 * Inicializa el siguiente comando en pipeline.
 * 
 * Proceso:
 * 1. Reserva memoria para next_cmd
 * 2. Inicializa estructura del comando
 * 3. Busca primer token válido (WORD, STRING, ARGS, SIMPLE_Q):
 *    - Salta redirecciones (solo toma su archivo, no el símbolo)
 *    - Cuando encuentra nombre: lo procesa y retorna
 * 4. Si no encuentra nombre válido: retorna error
 * 
 * current_cmd: Comando actual (se le añadirá next)
 * tokens: Puntero a tokens a procesar
 * 
 * Retorna: 0 si éxito, -1 si error
 */
int	init_next_cmd(t_cmd *current_cmd, t_token **tokens)
{
	t_token	*tmp;

	if (!tokens || !*tokens)
		return (-1);
	current_cmd->next = malloc(sizeof(t_cmd));
	if (!current_cmd->next)
		return (-1);
	init_cmd(current_cmd->next);
	tmp = *tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT
			|| tmp->type == REDIR_APPEND || tmp->type == HEREDOC)
			tmp = tmp->next;
		else if (tmp->value && (tmp->type == WORD || tmp->type == STRING
				|| tmp->type == ARGS || tmp->type == SIMPLE_Q))
			return (process_next_cmd_token(current_cmd, tmp, tokens));
		else
			tmp = tmp->next;
	}
	return (-1);
}

/*
 * Función: process_pipeline_cmd
 * ----------------------------
 * Procesa un comando dentro del pipeline.
 * 
 * Lógica:
 * 1. Si hay siguiente comando: ejecuta con pipe
 *    - Crea pipe y conecta salida de este comando a entrada del siguiente
 * 2. Si es el último: ejecuta con stdout normal
 *    - Cierra input_fd si no es stdin
 * 3. Avanza al siguiente comando
 * 
 * data: Estructura del shell
 * current: Puntero al comando actual (se actualiza)
 * input_fd: Puntero a descriptor de entrada (se actualiza)
 * last_cmd_pid: Puntero donde guardar PID del último comando
 */
void	process_pipeline_cmd(t_data *data, t_cmd **current, int *input_fd,
		pid_t *last_cmd_pid)
{
	if ((*current)->next)
		handle_pipe_cmd(data, *current, input_fd);
	else
	{
		*last_cmd_pid = exec_sig_cmd(data, *current, *input_fd, STDOUT_FILENO);
		if (*input_fd != STDIN_FILENO)
			close(*input_fd);
	}
	*current = (*current)->next;
}

/*
 * Función: execute_pipeline
 * ------------------------
 * Ejecuta un pipeline completo.
 * 
 * Proceso completo:
 * 1. Procesa todos los heredocs primero
 * 2. Inicializa variables (input_fd=stdin, exit_status=0)
 * 3. Configura señales para modo ejecución
 * 4. Para cada comando:
 *    - Si no es el último: crea pipe y ejecuta
 *    - Si es el último: ejecuta con stdout
 * 5. Espera a todos los procesos hijo
 * 6. Restaura señales a modo interactivo
 * 7. Actualiza exit_status en data
 * 
 * data: Estructura con comandos a ejecutar
 * 
 * Retorna: Exit status del último comando del pipeline
 */
int	execute_pipeline(t_data *data)
{
	t_cmd	*current;
	int		input_fd;
	int		exit_status;
	pid_t	last_cmd_pid;

	if (process_all_heredocs(data) != 0)
		return (data->exit_status);
	current = data->cmd;
	input_fd = STDIN_FILENO;
	exit_status = 0;
	last_cmd_pid = -1;
	setup_signals_executing();
	while (current)
		process_pipeline_cmd(data, &current, &input_fd, &last_cmd_pid);
	wait_all_processes(&exit_status, last_cmd_pid);
	setup_signals_interactive();
	data->exit_status = exit_status;
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	return (exit_status);
}

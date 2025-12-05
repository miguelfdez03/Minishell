/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:08 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:32:34 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: exec_sig_cmd
 * --------------------
 * Ejecuta un comando en un proceso hijo dentro de pipeline.
 * 
 * Proceso:
 * 1. Determina si es el último comando del pipeline
 * 2. Hace fork para crear proceso hijo
 * 3. Proceso hijo:
 *    - Configura descriptores de archivo (stdin/stdout)
 *    - Ejecuta el comando
 * 4. Proceso padre: retorna PID del hijo
 * 
 * data: Estructura con datos del shell
 * cmd: Comando a ejecutar
 * input_fd: Descriptor de entrada (stdin o pipe)
 * output_fd: Descriptor de salida (stdout o pipe)
 * 
 * Retorna: PID del proceso hijo o -1 si falla fork
 */
int	exec_sig_cmd(t_data *data, t_cmd *cmd, int input_fd, int output_fd)
{
	pid_t	pid;
	int		is_last_cmd;

	is_last_cmd = (cmd->next == NULL);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		setup_child_fds(input_fd, output_fd);
		exec_cmd_in_child(data, cmd, is_last_cmd);
	}
	return (pid);
}

/*
 * Función: handle_pipe_cmd
 * ------------------------
 * Maneja la ejecución de un comando con pipe.
 * 
 * Proceso:
 * 1. Crea pipe (pipe_fd[0]=lectura, pipe_fd[1]=escritura)
 * 2. Marca descriptores como close-on-exec
 * 3. Ejecuta comando con:
 *    - Entrada: input_fd (puede ser stdin o pipe anterior)
 *    - Salida: pipe_fd[1] (escritura del nuevo pipe)
 * 4. Cierra extremo de escritura del pipe
 * 5. Cierra input_fd anterior si no es stdin
 * 6. Actualiza input_fd para siguiente comando
 * 
 * data: Estructura con datos del shell
 * current: Comando actual a ejecutar
 * input_fd: Puntero a descriptor de entrada (se actualiza)
 * 
 * Retorna: PID del proceso hijo o -1 si falla
 */
int	handle_pipe_cmd(t_data *data, t_cmd *current, int *input_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	fcntl(pipe_fd[0], F_SETFD, FD_CLOEXEC);
	fcntl(pipe_fd[1], F_SETFD, FD_CLOEXEC);
	pid = exec_sig_cmd(data, current, *input_fd, pipe_fd[1]);
	close(pipe_fd[1]);
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	*input_fd = pipe_fd[0];
	return (pid);
}

/*
 * Función: process_pipes
 * ---------------------
 * Procesa todos los pipes en la lista de tokens.
 * 
 * Recorre los tokens buscando PIPE:
 * 1. Cuando encuentra pipe: crea siguiente comando
 * 2. Avanza al siguiente comando en la cadena
 * 3. Continúa hasta procesar todos los pipes
 * 
 * Esta función construye la cadena de comandos enlazados
 * que luego se ejecutará como pipeline.
 * 
 * data: Estructura con tokens y comandos
 * 
 * Retorna: 0 si éxito, -1 si error
 */
int	process_pipes(t_data *data)
{
	t_token	*tmp;
	t_cmd	*current_cmd;

	if (!data || !data->cmd || !data->tokens)
		return (-1);
	tmp = data->tokens;
	current_cmd = data->cmd;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (handle_pipe(current_cmd, &tmp) == -1)
				return (-1);
			current_cmd = current_cmd->next;
		}
		else
			tmp = tmp->next;
	}
	return (0);
}

/*
 * Función: process_cmd_args
 * ------------------------
 * Procesa argumentos y redirecciones de un comando hasta pipe.
 * 
 * Recorre tokens hasta encontrar PIPE:
 * 1. Si es redirección: añade a lista de redirections
 * 2. Si es WORD, STRING, ARGS, SIMPLE_Q: añade a argumentos
 * 3. Ignora tokens EMPTY
 * 
 * Actualiza el puntero tokens al final.
 * 
 * cmd: Comando donde añadir args y redirecciones
 * tokens: Puntero a token actual (se actualiza)
 */
void	process_cmd_args(t_cmd *cmd, t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT
			|| tmp->type == REDIR_APPEND || tmp->type == HEREDOC)
		{
			if (tmp->value)
				add_redir(&(cmd->redirections), tmp->type,
					ft_strdup(tmp->value));
		}
		else if (tmp->type != EMPTY && tmp->value && (tmp->type == WORD
				|| tmp->type == STRING || tmp->type == ARGS
				|| tmp->type == SIMPLE_Q))
			add_cmd_arg(cmd, tmp->value);
		tmp = tmp->next;
		*tokens = tmp;
	}
}

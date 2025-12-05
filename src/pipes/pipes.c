/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:15 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:32:34 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: setup_child_fds
 * -----------------------
 * Configura los descriptores de archivo en proceso hijo.
 * 
 * Redirige entrada/salida del proceso:
 * 1. Si input_fd no es stdin: redirige stdin a input_fd
 * 2. Si output_fd no es stdout: redirige stdout a output_fd
 * 3. Cierra los descriptores originales después de duplicar
 * 
 * Esto permite que el proceso hijo lea/escriba donde corresponda
 * en el pipeline.
 * 
 * input_fd: Descriptor de entrada (puede ser stdin o pipe)
 * output_fd: Descriptor de salida (puede ser stdout o pipe)
 */
void	setup_child_fds(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

/*
 * Función: handle_pipe
 * -------------------
 * Procesa un token de pipe y crea el siguiente comando.
 * 
 * Proceso:
 * 1. Avanza al siguiente token después del pipe
 * 2. Verifica que haya algo después del pipe (error si no)
 * 3. Inicializa el siguiente comando en la cadena
 * 4. Procesa argumentos y redirecciones del nuevo comando
 * 
 * current_cmd: Comando actual (se le añadirá el siguiente)
 * tokens: Puntero al token actual (se actualiza)
 * 
 * Retorna: 0 si éxito, -1 si error
 */
int	handle_pipe(t_cmd *current_cmd, t_token **tokens)
{
	t_token	*segment_start;

	if (!current_cmd || !tokens || !*tokens)
		return (-1);
	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		ft_printf("syntax error near unexpected token `|'\\n");
		return (-1);
	}
	segment_start = *tokens;
	if (init_next_cmd(current_cmd, tokens) == -1)
		return (-1);
	process_cmd_args(current_cmd->next, &segment_start);
	*tokens = segment_start;
	return (0);
}

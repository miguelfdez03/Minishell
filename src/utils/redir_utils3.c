/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:40 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:42:41 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: process_heredoc_redir
 * -----------------------------
 * Procesa una redirección de tipo heredoc.
 * 
 * Proceso:
 * 1. Ejecuta handle_heredoc para crear archivo temporal
 * 2. Si falla o se interrumpe: retorna error
 * 3. Redirige archivo temporal a stdin
 * 4. Elimina archivo temporal
 * 
 * file: Delimitador del heredoc
 * data: Estructura del shell
 * 
 * Retorna: 0 si ok, -1 error, -2 interrumpido
 */
static int	process_heredoc_redir(char *file, t_data *data)
{
	int	result;

	result = handle_heredoc(file, data);
	if (result == -1 || result == -2)
		return (result);
	if (handle_input_redir("/tmp/.minishell_heredoc") == -1)
		return (-1);
	unlink("/tmp/.minishell_heredoc");
	return (0);
}

/*
 * Función: process_single_redir
 * ----------------------------
 * Procesa una única redirección según su tipo.
 * 
 * Tipos:
 * - REDIR_IN (<): redirige entrada desde archivo
 * - REDIR_OUT (>): redirige salida a archivo (trunca)
 * - REDIR_APPEND (>>): redirige salida a archivo (append)
 * - HEREDOC (<<): procesa heredoc
 * 
 * redir: Redirección a procesar
 * data: Estructura del shell
 * 
 * Retorna: 0 si ok, -1 error, -2 interrumpido
 */
int	process_single_redir(t_redir *redir, t_data *data)
{
	if (redir->type == REDIR_IN)
	{
		if (handle_input_redir(redir->file) == -1)
			return (-1);
	}
	else if (redir->type == REDIR_OUT)
	{
		if (handle_output_redir(redir->file, 0) == -1)
			return (-1);
	}
	else if (redir->type == REDIR_APPEND)
	{
		if (handle_output_redir(redir->file, 1) == -1)
			return (-1);
	}
	else if (redir->type == HEREDOC)
		return (process_heredoc_redir(redir->file, data));
	return (0);
}

/*
 * Función: process_all_heredocs
 * ----------------------------
 * Procesa todos los heredocs de todos los comandos.
 * 
 * Proceso:
 * 1. Recorre todos los comandos
 * 2. Para cada comando:
 *    - Recorre sus redirecciones
 *    - Si encuentra HEREDOC: lo procesa
 *    - Si falla o se interrumpe: retorna error
 * 
 * Se llama antes de ejecutar pipeline para procesar
 * todos los heredocs de una vez.
 * 
 * data: Estructura del shell
 * 
 * Retorna: 0 si ok, -1 error, -2 interrumpido
 */
int	process_all_heredocs(t_data *data)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		result;

	cmd = data->cmd;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				result = handle_heredoc(redir->file, data);
				if (result == -1 || result == -2)
					return (result);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

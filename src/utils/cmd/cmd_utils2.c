/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:22 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:36:53 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Función: process_cmd_token
 * --------------------------
 * Procesa el primer token válido como nombre de comando.
 * 
 * Proceso:
 * 1. Crea comando con el valor del token
 * 2. Marca el token como EMPTY (ya procesado)
 * 3. Avanza al siguiente token
 * 
 * current: Token a procesar
 * data: Estructura donde crear el comando
 * tmp: Puntero a token actual (se actualiza)
 * 
 * Retorna: EXIT_SUCCESS si éxito, EXIT_FAILURE si falla
 */
static int	process_cmd_token(t_token *current, t_data **data, t_token **tmp)
{
	if (create_cmd(current->value, (*data)->cmd) == 0)
		return (EXIT_FAILURE);
	current->type = EMPTY;
	*tmp = current->next;
	return (EXIT_SUCCESS);
}

/*
 * Función: init_first_cmd
 * ----------------------
 * Inicializa el primer comando de la línea.
 * 
 * Busca el primer token válido para nombre de comando:
 * 1. Salta redirecciones (solo toma su archivo, no el símbolo)
 * 2. Cuando encuentra WORD, STRING, ARGS o SIMPLE_Q:
 *    - Lo procesa como nombre del comando
 *    - Retorna éxito
 * 3. Si solo hay redirecciones: retorna éxito
 * 4. Si no hay nada válido: retorna fallo
 * 
 * data: Estructura donde crear el comando
 * tmp: Puntero a tokens (se actualiza)
 * 
 * Retorna: EXIT_SUCCESS si éxito, EXIT_FAILURE si falla
 */
int	init_first_cmd(t_data **data, t_token **tmp)
{
	t_token	*current;
	int		has_redir;

	current = *tmp;
	has_redir = 0;
	while (current && current->type != PIPE)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			has_redir = 1;
			current = current->next;
		}
		else if (current->value && (current->type == WORD
				|| current->type == STRING || current->type == ARGS
				|| current->type == SIMPLE_Q))
			return (process_cmd_token(current, data, tmp));
		else
			current = current->next;
	}
	if (has_redir)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

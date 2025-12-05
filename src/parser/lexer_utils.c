/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:03 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:27:29 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: process_first_cmd_args
 * ------------------------------
 * Procesa tokens del primer comando hasta encontrar pipe.
 * 
 * Recorre tokens y:
 * 1. Si es redirección (<, >, <<, >>):
 *    - Añade a la lista de redirections del comando
 * 2. Si es WORD, STRING, ARGS o SIMPLE_Q:
 *    - Añade a los argumentos del comando
 * 3. Si encuentra PIPE: para y actualiza puntero
 * 
 * Ignora tokens EMPTY.
 * 
 * tmp: Puntero a token actual (se actualiza al pipe o NULL)
 * data: Estructura con el comando a procesar
 */
void	process_first_cmd_args(t_token **tmp, t_data **data)
{
	t_token	*current;

	current = *tmp;
	while (current && current->type != PIPE)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			if (current->value)
				add_redir(&((*data)->cmd->redirections), current->type,
					ft_strdup(current->value));
		}
		else if (current->type != EMPTY && current->value
			&& (current->type == WORD || current->type == STRING
				|| current->type == ARGS || current->type == SIMPLE_Q))
			add_cmd_arg((*data)->cmd, current->value);
		current = current->next;
	}
	*tmp = current;
}

/*
 * Función: check_and_exp
 * ---------------------
 * Procesa tokens: expande variables, concatena y crea comandos.
 * 
 * Flujo completo:
 * 1. Expande variables ($VAR, $?, $$) en todos los tokens
 * 2. Concatena tokens pegados (sin espacio entre ellos)
 * 3. Inicializa el primer comando
 * 4. Procesa argumentos y redirecciones del primer comando
 * 5. Si hay pipes: procesa comandos adicionales
 * 
 * Esta función transforma la lista de tokens en estructura
 * de comandos lista para ejecutar.
 * 
 * data: Estructura con tokens a procesar
 * 
 * Retorna: EXIT_SUCCESS si éxito, EXIT_FAILURE si error
 */
int	check_and_exp(t_data **data)
{
	t_token	*tmp;
	t_token	*start;

	if (!(*data)->tokens)
		return (EXIT_FAILURE);
	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);
	concatenate_tokens(&(*data)->tokens);
	start = (*data)->tokens;
	tmp = (*data)->tokens;
	if (init_first_cmd(data, &tmp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	process_first_cmd_args(&start, data);
	if (start && start->type == PIPE)
	{
		if (process_pipes(*data) == -1)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

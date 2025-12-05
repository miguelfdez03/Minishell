/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:48 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:47:03 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: add_to_token
 * -------------------
 * Añade un token al final de la lista.
 * 
 * Proceso:
 * 1. Crea nuevo token con tipo y valor
 * 2. Inicializa space a 0 y next a NULL
 * 3. Si lista vacía: establece como primero
 *    Si no: añade al final de la lista
 * 
 * tokens: Puntero a lista de tokens
 * type: Tipo del token (WORD, PIPE, REDIR_IN, etc.)
 * value: Valor del token (string)
 */
void	add_to_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->type = type;
	new_token->value = value;
	new_token->space = 0;
	new_token->next = NULL;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/*
 * Función: set_token_space
 * ----------------------
 * Marca si el último token tiene espacio después.
 * 
 * Proceso:
 * 1. Si lista vacía: retorna
 * 2. Busca último token de la lista
 * 3. Establece su campo space
 * 
 * Se usa para saber si hay espacio entre tokens,
 * importante para concatenar o separar argumentos.
 * 
 * tokens: Lista de tokens
 * has_space: 1 si hay espacio, 0 si no
 */
void	set_token_space(t_token *tokens, int has_space)
{
	t_token	*current;

	if (!tokens)
		return ;
	current = tokens;
	while (current->next)
		current = current->next;
	current->space = has_space;
}

/*
 * Función: is_space
 * ---------------
 * Verifica si un carácter es espacio en blanco.
 * 
 * c: Carácter a verificar
 * 
 * Retorna: EXIT_SUCCESS (0) si es espacio o tab
 *          EXIT_FAILURE (1) si no lo es
 */
int	is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}

/*
 * Función: is_quotes
 * ----------------
 * Verifica si un carácter es una comilla.
 * 
 * c: Carácter a verificar
 * 
 * Retorna: EXIT_SUCCESS (0) si es " o '
 *          EXIT_FAILURE (1) si no lo es
 */
int	is_quotes(char c)
{
	if (c == '\"' || c == '\'')
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}

/*
 * Función: is_symbols
 * -----------------
 * Verifica si un carácter es un símbolo especial.
 * 
 * c: Carácter a verificar
 * 
 * Retorna: EXIT_SUCCESS (0) si es >, <, o |
 *          EXIT_FAILURE (1) si no lo es
 */
int	is_symbols(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}

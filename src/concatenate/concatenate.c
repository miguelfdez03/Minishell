/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:26 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:19:56 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: should_concatenate
 * -------------------------
 * Determina si un token puede ser concatenado con otros.
 * 
 * No se pueden concatenar tokens especiales:
 * - PIPE (|)
 * - REDIR_APPEND (>>)
 * - REDIR_IN (<)
 * - REDIR_OUT (>)
 * - HEREDOC (<<)
 * 
 * Tokens que SÍ se pueden concatenar: WORD, variables expandidas, etc.
 * 
 * token: Token a verificar
 * 
 * Retorna: 1 si se puede concatenar, 0 si no
 */
static int	should_concatenate(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == PIPE || token->type == REDIR_APPEND
		|| token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == HEREDOC)
		return (0);
	return (1);
}

/*
 * Función: join_free
 * ----------------
 * Une dos strings y libera el primero.
 * 
 * Útil para concatenar sin perder memoria:
 * - Une s1 + s2
 * - Libera s1 (ya no se necesita)
 * - Retorna el resultado
 * 
 * s1: String que se liberará después de unir
 * s2: String que se concatena (no se libera)
 * 
 * Retorna: String concatenado (s1 + s2)
 */
static char	*join_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	return (res);
}

/*
 * Función: remove_tok_list
 * ----------------------
 * Elimina un token de la lista enlazada.
 * 
 * Proceso:
 * 1. Conecta el token anterior con el siguiente (salta current)
 * 2. Libera el valor del token
 * 3. Libera el token en sí
 * 
 * prev: Token anterior
 * current: Token a eliminar
 */
static void	remove_tok_list(t_token *prev, t_token *current)
{
	prev->next = current->next;
	free(current->value);
	free(current);
}

/*
 * Función: concatenate_tokens
 * -------------------------
 * Concatena tokens consecutivos sin espacios entre ellos.
 * 
 * Ejemplo:
 * Entrada: ["echo"] [" "] ["hola"] ["mundo"] -> sin espacio entre hola y mundo
 * Salida:  ["echo"] [" "] ["holamundo"]
 * 
 * Condiciones para concatenar:
 * 1. current->space == 0 (no hay espacio antes del token actual)
 * 2. Ambos tokens pueden concatenarse (no son pipe ni redirecciones)
 * 
 * Proceso:
 * 1. Recorre la lista de tokens
 * 2. Si encuentra tokens pegados: los une y elimina el segundo
 * 3. Si no: avanza al siguiente
 * 
 * tokens: Puntero a la lista de tokens
 */
void	concatenate_tokens(t_token **tokens)
{
	t_token	*prev;
	t_token	*current;
	t_token	*temp;

	if (!tokens || !*tokens || !(*tokens)->next)
		return ;
	prev = *tokens;
	current = prev->next;
	while (current)
	{
		if (current->space == 0 && should_concatenate(prev) == 1
			&& should_concatenate(current) == 1)
		{
			prev->value = join_free(prev->value, current->value);
			temp = current->next;
			remove_tok_list(prev, current);
			current = temp;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

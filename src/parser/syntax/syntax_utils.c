/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:55 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:27:30 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Función: check_redir_syntax
 * --------------------------
 * Verifica sintaxis de redirecciones.
 * 
 * Errores detectados:
 * 1. Redirección sin archivo (value vacío o NULL):
 *    - Si es final de línea: "syntax error near unexpected token `newline'"
 *    - Si sigue otro operador: muestra error del operador
 * 
 * Ejemplos de errores:
 * - "cat <" -> falta archivo después de <
 * - "echo > |" -> > seguido de |
 * 
 * current: Token de redirección a verificar
 * 
 * Retorna: 1 si sintaxis correcta, 0 si hay error
 */
int	check_redir_syntax(t_token *current)
{
	if (!current->value || current->value[0] == '\0')
	{
		if (!current->next || is_operator_token(current->next->type))
		{
			if (!current->next)
				ft_putendl_fd("minishell: syntax error near unexpected "
					"token `newline'", 2);
			else
				print_redir_error(current->next->type);
			return (0);
		}
	}
	return (1);
}

/*
 * Función: check_first_token
 * -------------------------
 * Verifica que el primer token no sea un pipe.
 * 
 * Error:
 * Si la línea empieza con |:
 * -> "syntax error near unexpected token `|'"
 * 
 * Ejemplo de error: "| ls" -> no se puede empezar con pipe
 * 
 * current: Primer token de la lista
 * 
 * Retorna: 1 si es válido, 0 si es pipe
 */
int	check_first_token(t_token *current)
{
	if (current->type == PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `|'", 2);
		return (0);
	}
	return (1);
}

/*
 * Función: check_tokens_loop
 * -------------------------
 * Recorre todos los tokens verificando sintaxis.
 * 
 * Para cada token:
 * 1. Si es PIPE: verifica sintaxis de pipe
 * 2. Si es redirección: verifica sintaxis de redirección
 * 3. Si encuentra error: retorna 0
 * 
 * Recorre toda la lista hasta encontrar error o terminar.
 * 
 * current: Primer token a verificar
 * 
 * Retorna: 1 si todo correcto, 0 si hay error
 */
int	check_tokens_loop(t_token *current)
{
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!check_pipe_syntax(current))
				return (0);
		}
		else if (is_redir_token(current->type))
		{
			if (!check_redir_syntax(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

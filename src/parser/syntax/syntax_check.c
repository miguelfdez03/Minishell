/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:54 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:28:57 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Función: is_redir_token
 * ----------------------
 * Verifica si un token es una redirección.
 * 
 * Redirecciones:
 * - REDIR_IN: <
 * - REDIR_OUT: >
 * - REDIR_APPEND: >>
 * - HEREDOC: <<
 * 
 * type: Tipo de token a verificar
 * 
 * Retorna: 1 si es redirección, 0 si no
 */
int	is_redir_token(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

/*
 * Función: is_operator_token
 * -------------------------
 * Verifica si un token es un operador (pipe o redirección).
 * 
 * Operadores:
 * - PIPE: |
 * - Cualquier redirección (<, >, <<, >>)
 * 
 * type: Tipo de token a verificar
 * 
 * Retorna: 1 si es operador, 0 si no
 */
int	is_operator_token(t_token_type type)
{
	return (type == PIPE || is_redir_token(type));
}

/*
 * Función: print_redir_error
 * -------------------------
 * Imprime mensaje de error de sintaxis según el token.
 * 
 * Muestra mensajes específicos para cada tipo de operador:
 * - PIPE: "syntax error near unexpected token `|'"
 * - REDIR_IN: "syntax error near unexpected token `<'"
 * - REDIR_OUT: "syntax error near unexpected token `>'"
 * - Y así para cada operador
 * 
 * type: Tipo de token que causó el error
 */
void	print_redir_error(t_token_type type)
{
	if (type == PIPE)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `|'", 2);
	else if (type == REDIR_IN)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `<'", 2);
	else if (type == REDIR_OUT)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `>'", 2);
	else if (type == REDIR_APPEND)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `>>'", 2);
	else if (type == HEREDOC)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `<<'", 2);
}

/*
 * Función: check_pipe_syntax
 * -------------------------
 * Verifica sintaxis correcta de pipes.
 * 
 * Errores detectados:
 * 1. Pipe al final de la línea (sin siguiente comando)
 *    -> "syntax error near unexpected token `newline'"
 * 2. Dos pipes consecutivos: | |
 *    -> "syntax error near unexpected token `|'"
 * 
 * current: Token de tipo PIPE a verificar
 * 
 * Retorna: 1 si sintaxis correcta, 0 si hay error
 */
int	check_pipe_syntax(t_token *current)
{
	if (!current->next)
	{
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `newline'", 2);
		return (0);
	}
	if (current->next->type == PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected "
			"token `|'", 2);
		return (0);
	}
	return (1);
}

/*
 * Función: check_syntax
 * -------------------
 * Verifica la sintaxis completa de los tokens.
 * 
 * Proceso:
 * 1. Verifica que haya tokens
 * 2. Verifica el primer token (no puede ser pipe)
 * 3. Verifica todos los tokens en bucle
 * 
 * Esta es la función principal de validación de sintaxis.
 * 
 * data: Estructura con tokens a verificar
 * 
 * Retorna: 1 si sintaxis correcta, 0 si hay error
 */
int	check_syntax(t_data **data)
{
	t_token	*current;

	if (!data || !(*data) || !(*data)->tokens)
		return (1);
	current = (*data)->tokens;
	if (!check_first_token(current))
		return (0);
	return (check_tokens_loop(current));
}

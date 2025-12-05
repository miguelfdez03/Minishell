/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:04 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:27:29 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: had_space_before
 * ------------------------
 * Verifica si había un espacio antes de la posición actual.
 * 
 * Casos:
 * - Si i=0: no hay nada antes, retorna 0
 * - Si el carácter anterior es espacio: retorna 1
 * - Si no: retorna 0
 * 
 * Esto ayuda a determinar si los tokens deben concatenarse.
 * 
 * line: Línea de entrada
 * i: Posición actual
 * 
 * Retorna: 1 si había espacio antes, 0 si no
 */
static int	had_space_before(char *line, int i)
{
	if (i == 0)
		return (0);
	if (i > 0 && is_space(line[i - 1]) == EXIT_SUCCESS)
		return (1);
	return (0);
}

/*
 * Función: handle_words_and_args
 * -----------------------------
 * Procesa palabras y argumentos especiales.
 * 
 * Casos especiales:
 * 1. '=' solo: crea token WORD con "="
 * 2. '-' : procesa como argumento (ej: -la, -n)
 * 3. '+' seguido de número o comillas: procesa como argumento
 * 4. Otros: procesa como palabra normal
 * 
 * Siempre marca si había espacio antes del token.
 * 
 * line: Línea de entrada
 * i: Posición actual
 * data: Estructura para añadir tokens
 * has_space: 1 si había espacio antes, 0 si no
 * 
 * Retorna: Nueva posición después de procesar
 */
static int	handle_words_and_args(char *line, int i, t_data **data,
		int has_space)
{
	int	result;

	if (line[i] == '=' && (is_space(line[i + 1]) == EXIT_SUCCESS || line[i
				+ 1] == '\0'))
	{
		add_to_token(&((*data)->tokens), WORD, ft_strdup("="));
		set_token_space((*data)->tokens, has_space);
		return (i + 1);
	}
	if (line[i] == '-' || (line[i] == '+' && (ft_isdigit(line[i + 1]) || line[i
					+ 1] == '"' || line[i + 1] == '\'')))
	{
		result = handle_args(line, i + 1, data);
		if (result > i)
			set_token_space((*data)->tokens, has_space);
		return (result);
	}
	result = handle_words(line, i, data);
	if (result > i)
		set_token_space((*data)->tokens, has_space);
	return (result);
}

/*
 * Función: handle_lexer_char
 * -------------------------
 * Procesa un solo carácter de la línea de entrada.
 * 
 * Flujo:
 * 1. Si es espacio: salta
 * 2. Verifica si había espacio antes
 * 3. Intenta procesar como comilla o símbolo
 * 4. Si no: intenta procesar como palabra o argumento
 * 5. Si nada funciona: avanza 1 posición
 * 
 * line: Línea de entrada
 * i: Posición actual
 * data: Estructura para añadir tokens
 * 
 * Retorna: Nueva posición después de procesar
 */
static int	handle_lexer_char(char *line, int i, t_data **data)
{
	int	result;
	int	has_space;

	if (is_space(line[i]) == EXIT_SUCCESS)
		return (i + 1);
	has_space = had_space_before(line, i);
	result = handle_quotes_and_symbols(line, i, data, has_space);
	if (result != -1)
		return (result);
	result = handle_words_and_args(line, i, data, has_space);
	if (result != -1)
		return (result);
	return (i + 1);
}

/*
 * Función: lexer
 * -------------
 * Tokeniza la línea de entrada completa.
 * 
 * Proceso completo:
 * 1. Recorre la línea carácter por carácter
 * 2. Crea tokens con handle_lexer_char()
 * 3. Si encuentra error (-1) o comillas sin cerrar (-2): para
 * 4. Verifica sintaxis con check_syntax()
 * 5. Si sintaxis correcta: expande variables y concatena tokens
 * 
 * Esta es la función principal del parser que convierte
 * texto plano en lista de tokens.
 * 
 * line: Línea a tokenizar
 * data: Estructura para almacenar tokens
 * 
 * Retorna: Posición final o -1 si hay error de sintaxis
 */
int	lexer(char *line, t_data **data)
{
	int	i;

	i = 0;
	while (line[i] && i >= 0)
	{
		i = handle_lexer_char(line, i, data);
		if (i == -1)
			return (EXIT_FAILURE);
		if (i == -2)
			break ;
	}
	if (!check_syntax(data))
	{
		(*data)->exit_status = 2;
		return (-1);
	}
	check_and_exp(data);
	return (i);
}

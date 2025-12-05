/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:02 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:27:29 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: handle_words
 * --------------------
 * Procesa palabras normales (comandos, argumentos, nombres).
 * 
 * Proceso:
 * 1. Salta espacios en blanco al inicio
 * 2. Calcula la longitud de la palabra usando ft_word_length()
 *    (hasta espacio, comilla o símbolo especial)
 * 3. Extrae la palabra
 * 4. Crea token de tipo WORD
 * 
 * Ejemplo: "echo hola" -> extrae "echo" como WORD
 * 
 * line: Línea de entrada
 * i: Posición actual
 * data: Estructura para añadir tokens
 * 
 * Retorna: Nueva posición después de la palabra o -1 si falla malloc
 */
int	handle_words(char *line, int i, t_data **data)
{
	int		last;
	char	*str;

	while (is_space(line[i]) == EXIT_SUCCESS)
		i++;
	last = ft_word_length(line, i);
	str = ft_substr(line, i, last - i);
	if (!str)
		return (-1);
	add_to_token(&((*data)->tokens), WORD, str);
	return (last);
}

/*
 * Función: handle_args
 * -------------------
 * Procesa argumentos especiales que empiezan con - o +.
 * 
 * Casos:
 * 1. Si hay espacio después: crea token WORD con solo el símbolo
 * 2. Si no: incluye lo que sigue y crea token ARGS
 * 
 * Ejemplo: "-la" -> token ARGS con value "-la"
 *          "- " -> token WORD con value "-"
 * 
 * line: Línea de entrada
 * i: Posición después del - o +
 * data: Estructura para añadir tokens
 * 
 * Retorna: Nueva posición después del argumento o -1 si falla malloc
 */
int	handle_args(char *line, int i, t_data **data)
{
	int		last;
	char	*str;

	if (is_space(line[i]) == EXIT_SUCCESS)
		last = i;
	else
		last = ft_word_length(line, i);
	str = ft_substr(line, i - 1, last - i + 1);
	if (!str)
		return (-1);
	if (last == i)
		add_to_token(&((*data)->tokens), WORD, str);
	else
		add_to_token(&((*data)->tokens), ARGS, str);
	return (last);
}

/*
 * Función: handle_quotes_and_symbols
 * ---------------------------------
 * Maneja comillas y símbolos especiales (redirecciones, pipes).
 * 
 * Proceso:
 * 1. Si encuentra comilla (' o "):
 *    - Llama a handle_quotes() para procesarla
 *    - Marca si había espacio antes
 * 2. Si encuentra símbolo (<, >, |):
 *    - Llama a check_redir() para procesarlo
 *    - Marca si había espacio antes
 * 3. Si no es ninguno: retorna -1
 * 
 * line: Línea de entrada
 * i: Posición actual
 * data: Estructura para añadir tokens
 * has_space: 1 si había espacio antes, 0 si no
 * 
 * Retorna: Nueva posición después de procesar o -1 si no es comilla ni símbolo
 */
int	handle_quotes_and_symbols(char *line, int i, t_data **data,
	int has_space)
{
	int	result;

	if (is_quotes(line[i]) == EXIT_SUCCESS)
	{
		result = handle_quotes(line, i, data);
		if (result > i)
			set_token_space((*data)->tokens, has_space);
		return (result);
	}
	else if (is_symbols(line[i]) == EXIT_SUCCESS)
	{
		result = check_redir(line, i, data);
		if (result > 0)
			set_token_space((*data)->tokens, has_space);
		return (i + result);
	}
	return (-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:57 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:27:29 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: handle_redir
 * --------------------
 * Procesa una redirección y extrae su archivo destino.
 * 
 * Proceso:
 * 1. Salta el símbolo de redirección (<, >, <<, >>)
 * 2. Salta espacios en blanco
 * 3. Extrae el nombre del archivo:
 *    - Puede estar entre comillas (maneja " y ')
 *    - Termina en espacio o símbolo especial
 * 4. Añade token con el tipo de redirección y el archivo
 * 
 * Ejemplo: "< infile" -> tipo=REDIR_IN, value="infile"
 *          ">> 'out file'" -> tipo=REDIR_APPEND, value="'out file'"
 * 
 * line: Línea de entrada
 * i: Posición del segundo carácter de redirección
 * data: Estructura para añadir tokens
 * type: Tipo de redirección (REDIR_IN, REDIR_OUT, etc.)
 * 
 * Retorna: Nueva posición después de procesar
 */
int	handle_redir(char *line, int i, t_data **data, t_token_type type)
{
	int		start;
	char	quote;

	i++;
	while (is_space(line[i]) == EXIT_SUCCESS)
		i++;
	start = i;
	while (line[i] && is_symbols(line[i]) == EXIT_FAILURE)
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (line[i] == quote)
				i++;
		}
		else if (is_space(line[i]) == EXIT_SUCCESS)
			break ;
		else
			i++;
	}
	add_to_token(&((*data)->tokens), type, ft_substr(line, start, i - start));
	return (i);
}

/*
 * Función: check_redir
 * -------------------
 * Identifica y procesa símbolos de redirección y pipes.
 * 
 * Reconoce:
 * - << : heredoc
 * - >> : append (añadir al final)
 * - > : redirección de salida
 * - < : redirección de entrada
 * - | : pipe
 * 
 * Para cada símbolo:
 * 1. Identifica el tipo
 * 2. Llama a handle_redir() para extraer el archivo (excepto pipe)
 * 3. Retorna cuántos caracteres avanzar
 * 
 * line: Línea de entrada
 * i: Posición actual
 * data: Estructura para añadir tokens
 * 
 * Retorna: Número de caracteres procesados (0 si no es redirección)
 */
int	check_redir(char *line, int i, t_data **data)
{
	if (line[i] == '<' && line[i + 1] == '<')
		return (handle_redir(line, i + 1, data, HEREDOC) - i);
	else if (line[i] == '>' && line[i + 1] == '>')
		return (handle_redir(line, i + 1, data, REDIR_APPEND) - i);
	else if (line[i] == '>' && line[i + 1] != '>')
		return (handle_redir(line, i, data, REDIR_OUT) - i);
	else if (line[i] == '<' && line[i + 1] != '<')
		return (handle_redir(line, i, data, REDIR_IN) - i);
	else if (line[i] == '|')
	{
		add_to_token(&((*data)->tokens), PIPE, ft_strdup("|"));
		return (1);
	}
	return (0);
}

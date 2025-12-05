/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:00 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:27:29 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: check_for_closed
 * ------------------------
 * Busca la comilla de cierre correspondiente.
 * 
 * Avanza desde la posición actual buscando la comilla que cierra.
 * 
 * line: Línea de entrada
 * i: Posición de la comilla de apertura
 * quote: Tipo de comilla a buscar (' o ")
 * 
 * Retorna: Posición de la comilla de cierre o -1 si no se encuentra
 */
int	check_for_closed(char *line, int i, char quote)
{
	i++;
	while (line[i] && line[i] != quote)
		i++;
	if (!line[i])
		return (-1);
	return (i);
}

/*
 * Función: check_unclosed_quotes
 * -----------------------------
 * Verifica si hay comillas sin cerrar en la línea.
 * 
 * Recorre toda la línea:
 * 1. Cuando encuentra comilla (' o "): busca su cierre
 * 2. Si no encuentra cierre: retorna el tipo de comilla sin cerrar
 * 3. Si todas están cerradas: retorna 0
 * 
 * Se usa para detectar si se necesita leer más líneas.
 * 
 * line: Línea a verificar
 * 
 * Retorna: Tipo de comilla sin cerrar (' o ") o 0 si todo está cerrado
 */
char	check_unclosed_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
				return (quote);
			i++;
		}
		else
			i++;
	}
	return (0);
}

/*
 * Función: handle_quotes
 * ---------------------
 * Procesa texto entre comillas y crea token.
 * 
 * Proceso:
 * 1. Busca la comilla de cierre
 * 2. Si no hay cierre: retorna -2 (error)
 * 3. Extrae el contenido entre comillas (sin incluir las comillas)
 * 4. Crea token según el tipo:
 *    - Comillas simples ('): tipo SIMPLE_Q (no se expanden variables)
 *    - Comillas dobles ("): tipo STRING (sí se expanden variables)
 * 
 * Ejemplo: "'hola'" -> token tipo SIMPLE_Q con value "hola"
 *          "\"mundo\"" -> token tipo STRING con value "mundo"
 * 
 * line: Línea de entrada
 * i: Posición de la comilla de apertura
 * data: Estructura para añadir tokens
 * 
 * Retorna: Posición después de la comilla de cierre, -1 si falla malloc, -2 si no cierra
 */
int	handle_quotes(char *line, int i, t_data **data)
{
	char	*str;
	int		last;

	last = check_for_closed(line, i, line[i]);
	if (last == -1)
		return (-2);
	str = ft_substr(line, i + 1, last - i - 1);
	if (!str)
		return (-1);
	if (line[i] == '\'')
		add_to_token(&((*data)->tokens), SIMPLE_Q, str);
	else
		add_to_token(&((*data)->tokens), STRING, str);
	return (last + 1);
}

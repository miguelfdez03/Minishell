/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:42 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:47:03 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: ft_word_length
 * ----------------------
 * Calcula la longitud de una palabra desde la posición i.
 * 
 * Proceso:
 * 1. Avanza mientras no encuentre:
 *    - Espacio en blanco
 *    - Símbolos especiales (|, <, >)
 *    - Comillas (' o ")
 * 2. Retorna la posición final
 * 
 * Se usa en el lexer para extraer palabras.
 * 
 * line: Línea de entrada
 * i: Posición inicial
 * 
 * Retorna: Posición donde termina la palabra
 */
int	ft_word_length(char *line, int i)
{
	while (line[i] && is_space(line[i]) != EXIT_SUCCESS
		&& line[i] != '|' && line[i] != '<' && line[i] != '>'
		&& line[i] != '\'' && line[i] != '"')
		i++;
	return (i);
}

/*
 * Función: ft_strcmp2
 * -----------------
 * Compara dos strings.
 * 
 * Proceso:
 * 1. Si alguno es NULL: retorna -1
 * 2. Compara carácter por carácter
 * 3. Retorna diferencia de caracteres
 * 
 * Similar a strcmp estándar pero maneja NULL.
 * 
 * str1: Primer string
 * str2: Segundo string
 * 
 * Retorna: 0 si iguales, <0 si str1 < str2, >0 si str1 > str2
 *          -1 si alguno es NULL
 */
int	ft_strcmp2(char *str1, char *str2)
{
	int	i;

	if (!str1 || !str2)
		return (-1);
	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

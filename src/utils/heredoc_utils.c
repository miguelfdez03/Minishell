/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:27 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:41:15 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: copy_quoted_content
 * ----------------------------
 * Copia el contenido entre comillas sin las comillas.
 * 
 * Proceso:
 * 1. Guarda el tipo de comilla (' o ")
 * 2. Salta la comilla de apertura
 * 3. Copia caracteres hasta encontrar comilla de cierre
 * 4. Salta la comilla de cierre
 * 
 * delimiter: String original con comillas
 * i: Índice actual (se modifica)
 * result: String destino
 * j: Índice en result (se modifica)
 */
static void	copy_quoted_content(char *delimiter, int *i, char *result, int *j)
{
	char	quote;

	quote = delimiter[(*i)++];
	while (delimiter[*i] && delimiter[*i] != quote)
		result[(*j)++] = delimiter[(*i)++];
	if (delimiter[*i] == quote)
		(*i)++;
}

/*
 * Función: remove_quotes_from_delimiter
 * ------------------------------------
 * Elimina todas las comillas del delimitador.
 * 
 * Proceso:
 * 1. Reserva memoria para resultado
 * 2. Recorre delimitador:
 *    - Si encuentra comilla: copia contenido sin comillas
 *    - Si no: copia carácter normal
 * 3. Añade terminador nulo
 * 
 * Ejemplo: "EOF" -> EOF, 'end' -> end
 * 
 * delimiter: Delimitador original
 * 
 * Retorna: Delimitador sin comillas o NULL si falla malloc
 */
char	*remove_quotes_from_delimiter(char *delimiter)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(delimiter) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '"' || delimiter[i] == '\'')
			copy_quoted_content(delimiter, &i, result, &j);
		else
			result[j++] = delimiter[i++];
	}
	result[j] = '\0';
	return (result);
}

/*
 * Función: process_heredoc_line_interactive
 * ---------------------------------------
 * Procesa una línea de heredoc en modo interactivo.
 * 
 * Proceso:
 * 1. Si debe expandir: llama a expand_string
 * 2. Libera línea original
 * 3. Usa línea expandida o string vacío si falla
 * 4. Escribe línea en archivo temporal
 * 5. Añade salto de línea
 * 6. Libera línea
 * 
 * here_s: Estructura con parámetros del heredoc
 * line: Línea leída por readline
 */
void	process_heredoc_line_interactive(t_heredoc_s *here_s, char *line)
{
	char	*expanded;

	if (here_s->expand)
	{
		expanded = expand_string(line, here_s->data->env,
				here_s->data->exit_status);
		free(line);
		if (expanded)
			line = expanded;
		else
			line = ft_strdup("");
	}
	if (line)
	{
		write(here_s->fd, line, ft_strlen(line));
		write(here_s->fd, "\n", 1);
		free(line);
	}
}

/*
 * Función: check_heredoc_exit
 * -------------------------
 * Verifica si el heredoc debe terminar.
 * 
 * Condiciones de salida:
 * 1. Señal recibida (Ctrl+C): retorna 1
 * 2. EOF (Ctrl+D) o señal: muestra warning y retorna 1
 * 3. Línea igual al delimitador: retorna 1
 * 4. Línea normal: retorna 0 (continúa)
 * 
 * line: Línea leída (puede ser NULL)
 * clean_delim: Delimitador sin comillas
 * 
 * Retorna: 1 si debe salir, 0 si continúa
 */
int	check_heredoc_exit(char *line, char *clean_delim)
{
	extern volatile sig_atomic_t	g_signal_received;

	if (g_signal_received == 130)
		return (1);
	if (!line || g_signal_received)
	{
		if (!g_signal_received)
			ft_putstr_fd("minishell: warning: heredoc EOF\n", 2);
		if (line)
			free(line);
		return (1);
	}
	if (ft_strcmp2(line, clean_delim) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

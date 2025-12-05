/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:49:42 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:41:15 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: setup_heredoc_io
 * ------------------------
 * Configura entrada/salida para heredoc interactivo.
 * 
 * Proceso:
 * 1. Guarda stdout actual
 * 2. Redirige stdout a stderr temporalmente
 * 
 * Esto evita que el prompt ">" de readline
 * interfiera con la salida del heredoc.
 * 
 * saved_stdout: Puntero donde guardar stdout
 */
void	setup_heredoc_io(int *saved_stdout)
{
	*saved_stdout = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
}

/*
 * Función: write_heredoc_interactive
 * --------------------------------
 * Escribe heredoc en modo interactivo (terminal).
 * 
 * Proceso:
 * 1. Guarda stdin original
 * 2. Configura señales para heredoc
 * 3. Configura entrada/salida
 * 4. Lee líneas con readline mostrando ">"
 * 5. Para cada línea:
 *    - Verifica condiciones de salida
 *    - Procesa y escribe línea
 * 6. Restaura stdout y stdin
 * 
 * here_s: Estructura con parámetros del heredoc
 */
void	write_heredoc_interactive(t_heredoc_s *here_s)
{
	char	*line;
	int		saved_stdout;
	int		stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	setup_signals_heredoc();
	setup_heredoc_io(&saved_stdout);
	while (1)
	{
		line = readline("> ");
		if (check_heredoc_exit(line, here_s->clean_delim))
			break ;
		process_heredoc_line_interactive(here_s, line);
	}
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
}

/*
 * Función: process_buffer_line
 * --------------------------
 * Procesa y escribe una línea en modo pipe.
 * 
 * Proceso:
 * 1. Si debe expandir:
 *    - Expande variables con expand_string
 *    - Escribe resultado expandido
 *    - Libera string expandido
 * 2. Si no: escribe buffer tal cual
 * 3. Añade salto de línea
 * 
 * here_s: Estructura del heredoc
 * buffer: Línea a procesar
 */
void	process_buffer_line(t_heredoc_s *here_s, char *buffer)
{
	char	*expanded;

	if (here_s->expand)
	{
		expanded = expand_string(buffer, here_s->data->env,
				here_s->data->exit_status);
		if (expanded)
		{
			write(here_s->fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
	}
	else
		write(here_s->fd, buffer, ft_strlen(buffer));
	write(here_s->fd, "\n", 1);
}

/*
 * Función: process_pipe_newline
 * ----------------------------
 * Procesa salto de línea en modo pipe.
 * 
 * Proceso:
 * 1. Si buffer == delimitador: retorna 1 (fin)
 * 2. Si no: procesa y escribe línea
 * 
 * here_s: Estructura del heredoc
 * buffer: Línea completa hasta \n
 * 
 * Retorna: 1 si debe terminar, 0 si continúa
 */
int	process_pipe_newline(t_heredoc_s *here_s, char *buffer)
{
	if (ft_strcmp2(buffer, here_s->clean_delim) == 0)
		return (1);
	process_buffer_line(here_s, buffer);
	return (0);
}

/*
 * Función: write_heredoc_pipe
 * -------------------------
 * Escribe heredoc en modo pipe (entrada redirigida).
 * 
 * Proceso:
 * 1. Lee byte a byte de stdin
 * 2. Acumula en buffer hasta encontrar \n
 * 3. Al encontrar \n:
 *    - Termina string con \0
 *    - Verifica si es delimitador
 *    - Si no: procesa línea
 *    - Limpia buffer y reinicia
 * 4. Sale si lee <= 0 bytes o buffer lleno
 * 
 * Usado cuando se ejecuta: echo "text" | ./minishell
 * 
 * here_s: Estructura del heredoc
 */
void	write_heredoc_pipe(t_heredoc_s *here_s)
{
	char	buffer[10000];
	ssize_t	bytes_read;
	int		i;

	i = 0;
	while (1)
	{
		bytes_read = read(STDIN_FILENO, buffer + i, 1);
		if (bytes_read <= 0 || i >= 9999)
			break ;
		if (buffer[i] == '\n')
		{
			buffer[i] = '\0';
			if (process_pipe_newline(here_s, buffer))
				break ;
			ft_memset(buffer, 0, i + 1);
			i = 0;
		}
		else
			i++;
	}
}

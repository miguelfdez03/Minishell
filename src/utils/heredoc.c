/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:28 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:41:15 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: should_expand_heredoc
 * ----------------------------
 * Determina si se deben expandir variables en un heredoc.
 * 
 * Proceso:
 * 1. Recorre el delimitador original
 * 2. Si encuentra comillas (" o '): no expande
 * 3. Si no hay comillas: expande variables
 * 
 * Ejemplo: <<EOF expande, <<"EOF" no expande
 * 
 * original_delimiter: Delimitador del heredoc
 * 
 * Retorna: 1 si debe expandir, 0 si no
 */
int	should_expand_heredoc(char *original_delimiter)
{
	int	i;

	i = 0;
	while (original_delimiter[i])
	{
		if (original_delimiter[i] == '"' || original_delimiter[i] == '\'')
			return (0);
		i++;
	}
	return (1);
}

/*
 * Función: write_heredoc_content
 * ----------------------------
 * Escribe el contenido del heredoc en el archivo temporal.
 * 
 * Proceso:
 * 1. Prepara estructura heredoc_s con parámetros
 * 2. Si stdin es terminal: modo interactivo
 * 3. Si no: modo pipe (para scripts)
 * 
 * fd: File descriptor donde escribir
 * clean_delim: Delimitador sin comillas
 * expand: 1 si debe expandir variables
 * data: Estructura con variables de entorno
 */
void	write_heredoc_content(int fd, char *clean_delim, int expand,
		t_data *data)
{
	t_heredoc_s	here_s;

	here_s.fd = fd;
	here_s.clean_delim = clean_delim;
	here_s.expand = expand;
	here_s.data = data;
	if (isatty(STDIN_FILENO))
		write_heredoc_interactive(&here_s);
	else
		write_heredoc_pipe(&here_s);
}

/*
 * Función: open_and_write_heredoc
 * ------------------------------
 * Abre el archivo temporal y escribe el heredoc.
 * 
 * Proceso:
 * 1. Crea archivo temporal con permisos 0600
 * 2. Establece FD_CLOEXEC para cerrar en exec
 * 3. Escribe contenido del heredoc
 * 4. Cierra archivo
 * 
 * tmp_file: Ruta del archivo temporal
 * clean_delimiter: Delimitador sin comillas
 * expand: Si debe expandir variables
 * data: Datos del shell
 * 
 * Retorna: 0 si ok, -1 si error
 */
static int	open_and_write_heredoc(char *tmp_file, char *clean_delimiter,
		int expand, t_data *data)
{
	int	fd;

	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(clean_delimiter);
		return (-1);
	}
	fcntl(fd, F_SETFD, FD_CLOEXEC);
	write_heredoc_content(fd, clean_delimiter, expand, data);
	close(fd);
	return (0);
}

/*
 * Función: redirect_heredoc_to_stdin
 * ---------------------------------
 * Redirige el archivo temporal a stdin.
 * 
 * Proceso:
 * 1. Abre archivo temporal en modo lectura
 * 2. Establece FD_CLOEXEC
 * 3. Duplica fd a STDIN_FILENO
 * 4. Cierra fd original
 * 5. Elimina archivo temporal
 * 
 * No se usa en la implementación actual.
 * 
 * tmp_file: Ruta del archivo temporal
 * 
 * Retorna: 0 si ok, -1 si error
 */
static int	redirect_heredoc_to_stdin(char *tmp_file)
{
	int	fd;

	fd = open(tmp_file, O_RDONLY);
	if (fd < 0)
		return (-1);
	fcntl(fd, F_SETFD, FD_CLOEXEC);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	unlink(tmp_file);
	return (0);
}

/*
 * Función: process_heredoc_file
 * ---------------------------
 * Procesa un heredoc completo y maneja señales.
 * 
 * Proceso:
 * 1. Resetea señal recibida
 * 2. Escribe heredoc en archivo temporal
 * 3. Libera delimitador
 * 4. Si recibió señal (Ctrl+C): elimina archivo y retorna -2
 * 5. Si ok: retorna 0
 * 
 * tmp_file: Ruta del archivo temporal
 * clean_delim: Delimitador limpio (se libera aquí)
 * expand: Si debe expandir variables
 * data: Datos del shell
 * 
 * Retorna: 0 si ok, -1 error, -2 interrumpido
 */
int	process_heredoc_file(char *tmp_file, char *clean_delim, int expand,
		t_data *data)
{
	extern volatile sig_atomic_t	g_signal_received;

	g_signal_received = 0;
	if (open_and_write_heredoc(tmp_file, clean_delim, expand, data) == -1)
	{
		free(clean_delim);
		return (-1);
	}
	free(clean_delim);
	if (g_signal_received)
	{
		unlink(tmp_file);
		return (-2);
	}
	return (0);
}

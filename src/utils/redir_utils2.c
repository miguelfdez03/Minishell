/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:32 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:43:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: handle_input_redir
 * --------------------------
 * Redirige entrada desde un archivo a stdin.
 * 
 * Proceso:
 * 1. Abre archivo en modo lectura
 * 2. Si falla: muestra error y retorna -1
 * 3. Duplica fd a STDIN_FILENO (fd 0)
 * 4. Cierra fd original
 * 
 * Ejemplo: < infile redirige infile a stdin
 * 
 * file: Ruta del archivo de entrada
 * 
 * Retorna: 0 si ok, -1 si error
 */
int	handle_input_redir(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

/*
 * Función: handle_output_redir
 * ---------------------------
 * Redirige salida de stdout a un archivo.
 * 
 * Proceso:
 * 1. Si append: abre con O_APPEND (>>)
 *    Si no: abre con O_TRUNC (>)
 * 2. Crea archivo con permisos 0644 si no existe
 * 3. Si falla: muestra error y retorna -1
 * 4. Duplica fd a STDOUT_FILENO (fd 1)
 * 5. Cierra fd original
 * 
 * file: Ruta del archivo de salida
 * append: 1 para >>, 0 para >
 * 
 * Retorna: 0 si ok, -1 si error
 */
int	handle_output_redir(char *file, int append)
{
	int	fd;
	int	flags;

	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

/*
 * Función: apply_redirections
 * -------------------------
 * Aplica todas las redirecciones del comando actual.
 * 
 * Proceso:
 * 1. Obtiene comando actual de data
 * 2. Recorre lista de redirecciones:
 *    - Si HEREDOC: abre archivo temporal y lo redirige
 *    - Si otro tipo: procesa con process_single_redir
 * 3. Si alguna falla: retorna error
 * 
 * Se llama antes de ejecutar el comando.
 * 
 * data: Estructura del shell
 * 
 * Retorna: 0 si ok, -1 si error, -2 si interrumpido
 */
int	apply_redirections(t_data *data)
{
	t_redir	*redir;
	int		result;
	t_cmd	*cmd;

	cmd = data->cmd;
	if (!cmd)
		return (0);
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (handle_input_redir("/tmp/.minishell_heredoc") == -1)
				return (-1);
			unlink("/tmp/.minishell_heredoc");
		}
		else
		{
			result = process_single_redir(redir, data);
			if (result != 0)
				return (result);
		}
		redir = redir->next;
	}
	return (0);
}

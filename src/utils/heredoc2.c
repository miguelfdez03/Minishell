/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:29 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:41:15 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: handle_heredoc
 * ----------------------
 * Maneja la ejecución completa de un heredoc.
 * 
 * Proceso:
 * 1. Define archivo temporal /tmp/.minishell_heredoc
 * 2. Elimina comillas del delimitador
 * 3. Determina si debe expandir variables
 * 4. Procesa el heredoc completo
 * 5. Restaura señales interactivas
 * 6. Si fue interrumpido (Ctrl+C): exit_status = 130
 * 
 * Es la función principal para heredocs.
 * 
 * delimiter: Delimitador del heredoc (puede tener comillas)
 * data: Estructura del shell
 * 
 * Retorna: 0 si ok, -1 error, -2 interrumpido
 */
int	handle_heredoc(char *delimiter, t_data *data)
{
	char	*clean_delimiter;
	int		expand;
	char	*tmp_file;
	int		result;

	tmp_file = "/tmp/.minishell_heredoc";
	clean_delimiter = remove_quotes_from_delimiter(delimiter);
	if (!clean_delimiter)
		return (-1);
	expand = should_expand_heredoc(delimiter);
	result = process_heredoc_file(tmp_file, clean_delimiter, expand, data);
	setup_signals_interactive();
	if (result == -2)
	{
		data->exit_status = 130;
		return (-2);
	}
	return (result);
}

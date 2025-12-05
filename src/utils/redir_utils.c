/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:31 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:43:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: copy_quoted_content
 * ----------------------------
 * Copia contenido entre comillas sin incluir las comillas.
 * 
 * Proceso:
 * 1. Guarda tipo de comilla (' o ")
 * 2. Salta comilla de apertura
 * 3. Copia caracteres hasta comilla de cierre
 * 4. Salta comilla de cierre
 * 
 * Usado para limpiar paths de archivos en redirecciones.
 * 
 * str: String original
 * result: String destino
 * i: Índice en str (se modifica)
 * j: Índice en result (se modifica)
 */
static void	copy_quoted_content(char *str, char *result, int *i, int *j)
{
	char	quote;

	quote = str[(*i)++];
	while (str[*i] && str[*i] != quote)
		result[(*j)++] = str[(*i)++];
	if (str[*i] == quote)
		(*i)++;
}

/*
 * Función: process_quotes_in_path
 * ------------------------------
 * Elimina comillas de un path de redirección.
 * 
 * Proceso:
 * 1. Reserva memoria para resultado
 * 2. Recorre string:
 *    - Si encuentra comilla: copia sin comillas
 *    - Si no: copia normal
 * 3. Libera string original
 * 4. Retorna string limpio
 * 
 * Ejemplo: "file.txt" -> file.txt
 * 
 * str: Path con posibles comillas
 * 
 * Retorna: Path sin comillas
 */
static char	*process_quotes_in_path(char *str)
{
	char	*result;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (str);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			copy_quoted_content(str, result, &i, &j);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	free(str);
	return (result);
}

/*
 * Función: add_redir
 * -----------------
 * Añade una redirección a la lista de redirecciones.
 * 
 * Proceso:
 * 1. Crea nuevo nodo de redirección
 * 2. Asigna tipo de redirección
 * 3. Si es HEREDOC: guarda value tal cual
 *    Si no: procesa comillas del path
 * 4. Si lista vacía: establece como primera
 *    Si no: añade al final de la lista
 * 
 * redir: Puntero a lista de redirecciones
 * type: Tipo (REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC)
 * value: Archivo o delimitador
 */
void	add_redir(t_redir **redir, t_token_type type, char *value)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->type = type;
	if (type == HEREDOC)
		new_redir->file = value;
	else
		new_redir->file = process_quotes_in_path(value);
	new_redir->next = NULL;
	if (!*redir)
	{
		*redir = new_redir;
		return ;
	}
	current = *redir;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

/*
 * Función: free_redirs
 * -------------------
 * Libera toda la lista de redirecciones.
 * 
 * Proceso:
 * 1. Recorre lista de redirecciones
 * 2. Para cada nodo:
 *    - Libera file/delimitador
 *    - Libera nodo
 * 
 * redir: Lista de redirecciones a liberar
 */
void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = tmp;
	}
}

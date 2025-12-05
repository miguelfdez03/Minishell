/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:26 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:47:03 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: free_tokens
 * ------------------
 * Libera toda la lista de tokens.
 * 
 * Proceso:
 * 1. Recorre lista de tokens
 * 2. Para cada token:
 *    - Libera value (string)
 *    - Libera nodo del token
 * 
 * tokens: Lista de tokens a liberar
 */
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

/*
 * Función: free_cmd_content
 * -----------------------
 * Libera el contenido de un comando.
 * 
 * Proceso:
 * 1. Libera name del comando
 * 2. Libera array args:
 *    - Libera cada string
 *    - Libera array
 * 3. Libera lista de redirecciones
 * 
 * cmd: Comando cuyo contenido se libera
 */
static void	free_cmd_content(t_cmd *cmd)
{
	int	i;

	if (cmd->name)
		free(cmd->name);
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->redirections)
		free_redirs(cmd->redirections);
}

/*
 * Función: free_cmd
 * ---------------
 * Libera toda la lista de comandos.
 * 
 * Proceso:
 * 1. Recorre lista de comandos
 * 2. Para cada comando:
 *    - Libera su contenido (name, args, redirs)
 *    - Libera nodo del comando
 * 
 * Se usa para limpiar después de ejecutar un pipeline.
 * 
 * cmd: Lista de comandos a liberar
 */
void	free_cmd(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free_cmd_content(cmd);
		free(cmd);
		cmd = next;
	}
}

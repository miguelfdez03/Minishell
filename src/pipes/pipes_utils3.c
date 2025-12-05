/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:11 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:32:34 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: close_all_fds
 * ---------------------
 * Cierra todos los descriptores de archivo abiertos.
 * 
 * Recorre desde fd=3 hasta 1024 cerrando cada descriptor.
 * 
 * Se usa en procesos hijo antes de execve para evitar
 * que se hereden pipes u otros descriptores innecesarios.
 * 
 * fd 0, 1, 2 (stdin, stdout, stderr) no se cierran porque
 * ya fueron configurados con setup_child_fds().
 */
void	close_all_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
		close(fd++);
}

/*
 * Función: init_next_cmd_name
 * --------------------------
 * Inicializa el nombre del siguiente comando en pipeline.
 * 
 * Proceso:
 * 1. Duplica el valor del token como nombre del comando
 * 2. Si falla malloc: libera next_cmd y retorna error
 * 3. Identifica si el comando es un builtin
 * 
 * next_cmd: Comando a inicializar
 * tmp: Token con el nombre del comando
 * 
 * Retorna: 0 si éxito, -1 si falla malloc
 */
int	init_next_cmd_name(t_cmd *next_cmd, t_token *tmp)
{
	next_cmd->name = ft_strdup(tmp->value);
	if (!next_cmd->name)
	{
		free(next_cmd);
		return (-1);
	}
	next_cmd->builtin_id = identify_builtin(tmp->value);
	return (0);
}

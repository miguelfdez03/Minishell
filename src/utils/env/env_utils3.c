/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:43:17 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:37:58 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Función: decrement_indices
 * --------------------------
 * Decrementa el índice de todos los nodos desde start.
 * 
 * Se usa cuando se elimina un nodo para mantener
 * los índices consecutivos.
 * 
 * start: Primer nodo a actualizar
 */
static void	decrement_indices(t_env *start)
{
	t_env	*current;

	current = start;
	while (current)
	{
		current->index--;
		current = current->next;
	}
}

/*
 * Función: remove_env_node
 * -----------------------
 * Elimina un nodo de la lista de variables.
 * 
 * Proceso:
 * 1. Si es el primer nodo: actualiza cabeza
 * 2. Si no: conecta prev con next
 * 3. Decrementa índices de nodos siguientes
 * 4. Libera memoria del nodo eliminado
 * 
 * env_head: Puntero a la cabeza de la lista
 * prev: Nodo anterior (NULL si es el primero)
 * to_remove: Nodo a eliminar
 */
static void	remove_env_node(t_env **env_head, t_env *prev, t_env *to_remove)
{
	t_env	*temp;

	if (prev)
		prev->next = to_remove->next;
	else
		*env_head = to_remove->next;
	temp = to_remove->next;
	free(to_remove->key);
	if (to_remove->value)
		free(to_remove->value);
	free(to_remove);
	decrement_indices(temp);
}

/*
 * Función: unset_env_var
 * ---------------------
 * Elimina una variable de entorno de la lista.
 * 
 * Proceso:
 * 1. Busca el nodo con la key especificada
 * 2. Guarda referencia al nodo anterior
 * 3. Si encuentra la variable: la elimina
 * 
 * Se usa para el comando unset.
 * 
 * env_head: Puntero a la cabeza de la lista
 * key: Nombre de la variable a eliminar
 */
void	unset_env_var(t_env **env_head, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env_head || !*env_head || !key)
		return ;
	current = *env_head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp2(current->key, (char *)key) == 0)
		{
			remove_env_node(env_head, prev, current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

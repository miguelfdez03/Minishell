/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:43:19 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:36:53 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Función: update_indices
 * ----------------------
 * Incrementa el índice de todos los nodos desde start.
 * 
 * Se usa cuando se inserta un nodo nuevo para mantener
 * los índices consecutivos.
 * 
 * start: Primer nodo a actualizar
 */
static void	update_indices(t_env *start)
{
	t_env	*current;

	current = start;
	while (current)
	{
		current->index++;
		current = current->next;
	}
}

/*
 * Función: create_env_node
 * -----------------------
 * Crea un nuevo nodo de variable de entorno.
 * 
 * Proceso:
 * 1. Reserva memoria para el nodo
 * 2. Asigna key y value
 * 3. Establece next a NULL
 * 4. Si falla malloc: libera key y value
 * 
 * dup_key: Key ya duplicada
 * dup_val: Value ya duplicado (puede ser NULL)
 * 
 * Retorna: Nuevo nodo o NULL si falla malloc
 */
static t_env	*create_env_node(char *dup_key, char *dup_val)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(dup_key);
		if (dup_val)
			free(dup_val);
		return (NULL);
	}
	new_node->key = dup_key;
	new_node->value = dup_val;
	new_node->next = NULL;
	return (new_node);
}

/*
 * Función: insert_at_head
 * ----------------------
 * Inserta un nodo al inicio de la lista.
 * 
 * Proceso:
 * 1. Conecta nuevo nodo con la cabeza actual
 * 2. Establece su índice a 0
 * 3. Actualiza cabeza de la lista
 * 4. Incrementa índices de todos los nodos siguientes
 * 
 * env_head: Puntero a la cabeza de la lista
 * new_node: Nodo a insertar
 */
static void	insert_at_head(t_env **env_head, t_env *new_node)
{
	t_env	*current;

	new_node->next = *env_head;
	new_node->index = 0;
	*env_head = new_node;
	current = new_node->next;
	update_indices(current);
}

/*
 * Función: insert_after_prev
 * -------------------------
 * Inserta un nodo después de otro nodo.
 * 
 * Proceso:
 * 1. Conecta nuevo nodo con el siguiente de prev
 * 2. Conecta prev con el nuevo nodo
 * 3. Establece índice = prev->index + 1
 * 4. Incrementa índices de nodos siguientes
 * 
 * prev: Nodo después del cual insertar
 * new_node: Nodo a insertar
 */
static void	insert_after_prev(t_env *prev, t_env *new_node)
{
	new_node->next = prev->next;
	prev->next = new_node;
	new_node->index = prev->index + 1;
	update_indices(new_node->next);
}

/*
 * Función: set_env_new_node
 * ------------------------
 * Crea e inserta un nuevo nodo de variable en orden alfabético.
 * 
 * Proceso:
 * 1. Si lista vacía: crea primer nodo
 * 2. Si no: crea nodo y busca posición correcta
 * 3. Inserta en orden alfabético por key:
 *    - Al inicio si es el menor
 *    - Después de prev si está en medio
 * 
 * Mantiene la lista ordenada y los índices actualizados.
 * 
 * env_head: Puntero a la cabeza de la lista
 * dup_key: Key ya duplicada
 * dup_val: Value ya duplicado (puede ser NULL)
 */
void	set_env_new_node(t_env **env_head, char *dup_key, char *dup_val)
{
	t_env	*current;
	t_env	*prev;
	t_env	*new_node;

	if (!*env_head)
	{
		*env_head = create_env_node(dup_key, dup_val);
		if (*env_head)
			(*env_head)->index = 0;
		return ;
	}
	new_node = create_env_node(dup_key, dup_val);
	if (!new_node)
		return ;
	current = *env_head;
	prev = NULL;
	while (current && ft_strcmp2(dup_key, current->key) > 0)
	{
		prev = current;
		current = current->next;
	}
	if (!prev)
		insert_at_head(env_head, new_node);
	else
		insert_after_prev(prev, new_node);
}

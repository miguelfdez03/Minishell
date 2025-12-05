/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:43:13 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:36:53 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Función: find_env
 * ----------------
 * Busca una variable de entorno por su nombre.
 * 
 * Recorre la lista de variables comparando keys hasta
 * encontrar coincidencia.
 * 
 * env: Lista de variables de entorno
 * key: Nombre de la variable a buscar
 * 
 * Retorna: Puntero al nodo de la variable o NULL si no existe
 */
t_env	*find_env(t_env *env, const char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp2(current->key, (char *)key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/*
 * Función: get_env_value_from_list
 * --------------------------------
 * Obtiene el valor de una variable de entorno.
 * 
 * Busca la variable por nombre y retorna su valor.
 * 
 * env: Lista de variables de entorno
 * key: Nombre de la variable
 * 
 * Retorna: Valor de la variable o NULL si no existe
 */
char	*get_env_value_from_list(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp2(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/*
 * Función: set_env_value
 * ---------------------
 * Establece o actualiza el valor de una variable de entorno.
 * 
 * Proceso:
 * 1. Duplica el valor (NULL si no hay valor)
 * 2. Busca si la variable ya existe:
 *    - Si existe: actualiza su valor
 *    - Si no existe: crea nuevo nodo
 * 
 * La lista se mantiene ordenada alfabéticamente.
 * 
 * env_head: Puntero a la cabeza de la lista
 * key: Nombre de la variable
 * value: Valor a establecer (puede ser NULL)
 */
void	set_env_value(t_env **env_head, const char *key, const char *value)
{
	t_env	*node;
	char	*dup_key;
	char	*dup_val;

	if (!key)
		return ;
	if (value)
		dup_val = ft_strdup(value);
	else
		dup_val = NULL;
	node = find_env(*env_head, key);
	if (node)
	{
		if (node->value)
			free(node->value);
		node->value = dup_val;
		return ;
	}
	dup_key = ft_strdup(key);
	set_env_new_node(env_head, dup_key, dup_val);
}

/*
 * Función: free_env
 * ----------------
 * Libera toda la lista de variables de entorno.
 * 
 * Recorre la lista liberando:
 * 1. Key de cada nodo
 * 2. Value de cada nodo (si existe)
 * 3. El nodo en sí
 * 
 * env: Lista a liberar
 */
void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

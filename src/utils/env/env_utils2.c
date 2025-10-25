/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:00:00 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/25 23:50:17 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	init_first_env_node(t_env **env_head, char *dup_key, char *dup_val)
{
	*env_head = malloc(sizeof(t_env));
	if (!*env_head)
		return ;
	(*env_head)->key = dup_key;
	(*env_head)->value = dup_val;
	(*env_head)->index = 0;
	(*env_head)->next = NULL;
}

void	add_env_node(t_env *last, char *dup_key, char *dup_val)
{
	last->next = malloc(sizeof(t_env));
	if (!last->next)
	{
		free(dup_key);
		if (dup_val)
			free(dup_val);
		return ;
	}
	last->next->key = dup_key;
	last->next->value = dup_val;
	last->next->index = last->index + 1;
	last->next->next = NULL;
}

void	set_env_new_node(t_env **env_head, char *dup_key, char *dup_val)
{
	t_env	*last;

	if (!*env_head)
	{
		init_first_env_node(env_head, dup_key, dup_val);
		return ;
	}
	last = *env_head;
	while (last->next)
		last = last->next;
	add_env_node(last, dup_key, dup_val);
}

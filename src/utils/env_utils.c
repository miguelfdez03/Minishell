/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 13:07:08 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/20 17:26:41 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

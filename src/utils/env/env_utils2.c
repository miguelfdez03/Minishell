#include "../../minishell.h"

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

static void	insert_at_head(t_env **env_head, t_env *new_node)
{
	t_env	*current;

	new_node->next = *env_head;
	new_node->index = 0;
	*env_head = new_node;
	current = new_node->next;
	update_indices(current);
}

static void	insert_after_prev(t_env *prev, t_env *new_node)
{
	new_node->next = prev->next;
	prev->next = new_node;
	new_node->index = prev->index + 1;
	update_indices(new_node->next);
}

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

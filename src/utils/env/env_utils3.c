#include "../../minishell.h"

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

static void	remove_env_node(t_env **env_head, t_env *prev, t_env *current)
{
	t_env	*temp;

	if (prev)
		prev->next = current->next;
	else
		*env_head = current->next;
	temp = current->next;
	free(current->key);
	if (current->value)
		free(current->value);
	free(current);
	decrement_indices(temp);
}

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

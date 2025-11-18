#include "../../minishell.h"

int	init_first_cmd(t_data **data, t_token **tmp)
{
	t_token	*current;

	current = *tmp;
	while (current && current->type != PIPE)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			current = current->next;
		}
		else if (current->value && (current->type == WORD
				|| current->type == STRING || current->type == ARGS))
		{
			if (create_cmd(current->value, (*data)->cmd) == 0)
				return (EXIT_FAILURE);
			current->type = EMPTY;
			*tmp = current->next;
			return (EXIT_SUCCESS);
		}
		else
			current = current->next;
	}
	return (EXIT_FAILURE);
}

#include "../../minishell.h"

static int	process_cmd_token(t_token *current, t_data **data, t_token **tmp)
{
	if (create_cmd(current->value, (*data)->cmd) == 0)
		return (EXIT_FAILURE);
	current->type = EMPTY;
	*tmp = current->next;
	return (EXIT_SUCCESS);
}

int	init_first_cmd(t_data **data, t_token **tmp)
{
	t_token	*current;
	int		has_redir;

	current = *tmp;
	has_redir = 0;
	while (current && current->type != PIPE)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			has_redir = 1;
			current = current->next;
		}
		else if (current->value && (current->type == WORD
				|| current->type == STRING || current->type == ARGS))
			return (process_cmd_token(current, data, tmp));
		else
			current = current->next;
	}
	if (has_redir)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

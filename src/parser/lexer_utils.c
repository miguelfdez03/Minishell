#include "../minishell.h"

void	process_first_cmd_args(t_token **tmp, t_data **data)
{
	while (*tmp && (*tmp)->type != PIPE)
	{
		if ((*tmp)->type == REDIR_IN || (*tmp)->type == REDIR_OUT
			|| (*tmp)->type == REDIR_APPEND || (*tmp)->type == HEREDOC)
		{
			if ((*tmp)->value)
				add_redir(&((*data)->cmd->redirections), (*tmp)->type,
					ft_strdup((*tmp)->value));
		}
		else if ((*tmp)->value && ((*tmp)->type == WORD
				|| (*tmp)->type == STRING || (*tmp)->type == ARGS
				|| (*tmp)->type == SIMPLE_Q))
			add_cmd_arg((*data)->cmd, (*tmp)->value);
		*tmp = (*tmp)->next;
	}
}

int	check_and_exp(t_data **data)
{
	t_token	*tmp;

	if (!(*data)->tokens)
		return (EXIT_FAILURE);
	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);
	concatenate_tokens(&(*data)->tokens);
	tmp = (*data)->tokens;
	if (init_first_cmd(data, &tmp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	process_first_cmd_args(&tmp, data);
	if (tmp && tmp->type == PIPE)
	{
		if (process_pipes(*data) == -1)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

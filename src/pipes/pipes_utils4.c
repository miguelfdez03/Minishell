#include "../minishell.h"

static int	process_next_cmd_token(t_cmd *current_cmd, t_token *tmp,
		t_token **tokens)
{
	if (init_next_cmd_name(current_cmd->next, tmp) == -1)
	{
		current_cmd->next = NULL;
		return (-1);
	}
	tmp->type = EMPTY;
	*tokens = tmp->next;
	return (0);
}

int	init_next_cmd(t_cmd *current_cmd, t_token **tokens)
{
	t_token	*tmp;

	if (!tokens || !*tokens)
		return (-1);
	current_cmd->next = malloc(sizeof(t_cmd));
	if (!current_cmd->next)
		return (-1);
	init_cmd(current_cmd->next);
	tmp = *tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT
			|| tmp->type == REDIR_APPEND || tmp->type == HEREDOC)
			tmp = tmp->next;
		else if (tmp->value && (tmp->type == WORD || tmp->type == STRING
				|| tmp->type == ARGS))
			return (process_next_cmd_token(current_cmd, tmp, tokens));
		else
			tmp = tmp->next;
	}
	return (-1);
}

void	process_pipeline_cmd(t_data *data, t_cmd **current, int *input_fd,
		pid_t *last_cmd_pid)
{
	if ((*current)->next)
		handle_pipe_cmd(data, *current, input_fd);
	else
	{
		*last_cmd_pid = exec_sig_cmd(data, *current, *input_fd, STDOUT_FILENO);
		if (*input_fd != STDIN_FILENO)
			close(*input_fd);
	}
	*current = (*current)->next;
}

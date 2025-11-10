#include "../minishell.h"

int	execute_single_cmd(t_data *data, t_cmd *cmd, int input_fd, int output_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		setup_child_fds(input_fd, output_fd);
		exec_cmd_in_child(data, cmd);
	}
	return (pid);
}

int	handle_pipe_cmd(t_data *data, t_cmd *current, int *input_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	fcntl(pipe_fd[0], F_SETFD, FD_CLOEXEC);
	fcntl(pipe_fd[1], F_SETFD, FD_CLOEXEC);
	pid = execute_single_cmd(data, current, *input_fd, pipe_fd[1]);
	close(pipe_fd[1]);
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	*input_fd = pipe_fd[0];
	return (pid);
}

int	process_pipes(t_data *data)
{
	t_token	*tmp;
	t_cmd	*current_cmd;

	if (!data || !data->cmd || !data->tokens)
		return (-1);
	tmp = data->tokens;
	current_cmd = data->cmd;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (handle_pipe(current_cmd, &tmp) == -1)
				return (-1);
			current_cmd = current_cmd->next;
		}
		else
			tmp = tmp->next;
	}
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
	if (tmp && tmp->value && (tmp->type == WORD || tmp->type == STRING))
	{
		if (init_next_cmd_name(current_cmd->next, tmp) == -1)
		{
			current_cmd->next = NULL;
			return (-1);
		}
		*tokens = (*tokens)->next;
	}
	return (0);
}

void	process_cmd_args(t_cmd *cmd, t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT
			|| tmp->type == REDIR_APPEND || tmp->type == HEREDOC)
		{
			if (tmp->value)
				add_redir(&(cmd->redirections), tmp->type,
					ft_strdup(tmp->value));
		}
		else if (tmp->value && (tmp->type == WORD || tmp->type == STRING
				|| tmp->type == ARGS || tmp->type == SIMPLE_Q))
			add_cmd_arg(cmd, tmp->value);
		tmp = tmp->next;
		*tokens = tmp;
	}
}
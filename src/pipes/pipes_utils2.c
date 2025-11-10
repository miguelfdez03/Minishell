#include "../minishell.h"

static int	wait_all_processes(int *exit_status)
{
	int		status;
	pid_t	last_pid;

	last_pid = -1;
	while (1)
	{
		last_pid = waitpid(-1, &status, 0);
		if (last_pid <= 0)
			break ;
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
	}
	return (*exit_status);
}

int	execute_pipeline(t_data *data)
{
	t_cmd	*current;
	int		input_fd;
	int		exit_status;

	current = data->cmd;
	input_fd = STDIN_FILENO;
	exit_status = 0;
	while (current)
	{
		if (current->next)
			handle_pipe_cmd(data, current, &input_fd);
		else
		{
			execute_single_cmd(data, current, input_fd, STDOUT_FILENO);
			if (input_fd != STDIN_FILENO)
				close(input_fd);
		}
		current = current->next;
	}
	wait_all_processes(&exit_status);
	data->exit_status = exit_status;
	return (exit_status);
}

void	exec_cmd_in_child(t_data *data, t_cmd *cmd)
{
	int		exit_code;
	t_cmd	*original_cmd;

	original_cmd = data->cmd;
	setup_signals_child();
	close_all_fds();
	if (apply_redirections(cmd) == -1)
		cleanup_and_exit(data, original_cmd, 1);
	data->cmd = cmd;
	if (cmd->builtin_id != BUILTIN_NONE)
	{
		exit_code = execute_builtin_by_id(data);
		cleanup_and_exit(data, original_cmd, exit_code);
	}
	exec_external_cmd(data, cmd, original_cmd);
}

int	init_next_cmd_name(t_cmd *next_cmd, t_token *tmp)
{
	next_cmd->name = ft_strdup(tmp->value);
	if (!next_cmd->name)
	{
		free(next_cmd);
		return (-1);
	}
	next_cmd->builtin_id = identify_builtin(tmp->value);
	return (0);
}

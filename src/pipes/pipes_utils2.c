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

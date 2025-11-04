/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:40:00 by miguel-f          #+#    #+#             */
/*   Updated: 2025/11/04 22:42:25 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_single_cmd(t_data *data, t_cmd *cmd, int input_fd, int output_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd != STDOUT_FILENO)
		{
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}
		data->cmd = cmd;
		if (cmd->builtin_id != BUILTIN_NONE)
			exit(execute_builtin_by_id(data));
		else
			exit(execute_external_command(data));
	}
	return (pid);
}

static void	close_pipe_fds(int input_fd, int pipe_write_fd)
{
	close(pipe_write_fd);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
}

static int	wait_all_processes(int *exit_status)
{
	int	status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
	}
	return (*exit_status);
}

static int	handle_pipe_cmd(t_data *data, t_cmd *current, int *input_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = execute_single_cmd(data, current, *input_fd, pipe_fd[1]);
	close_pipe_fds(*input_fd, pipe_fd[1]);
	*input_fd = pipe_fd[0];
	return (pid);
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

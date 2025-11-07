/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:40:00 by miguel-f          #+#    #+#             */
/*   Updated: 2025/11/07 15:07:20 by miguel-f         ###   ########.fr       */
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
		setup_child_fds(input_fd, output_fd);
		exec_cmd_in_child(data, cmd);
	}
	return (pid);
}

static void	close_pipe_fds(int input_fd, int pipe_write_fd)
{
	close(pipe_write_fd);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
}

int	handle_pipe_cmd(t_data *data, t_cmd *current, int *input_fd)
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

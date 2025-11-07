/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:30:00 by miguel-f          #+#    #+#             */
/*   Updated: 2025/11/07 14:46:00 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_input_redir(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redir(char *file, int append)
{
	int	fd;
	int	flags;

	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_cmd *cmd)
{
	t_redir	*redir;

	if (!cmd)
		return (0);
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (handle_input_redir(redir->file) == -1)
				return (-1);
		}
		else if (redir->type == REDIR_OUT)
		{
			if (handle_output_redir(redir->file, 0) == -1)
				return (-1);
		}
		else if (redir->type == REDIR_APPEND)
		{
			if (handle_output_redir(redir->file, 1) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

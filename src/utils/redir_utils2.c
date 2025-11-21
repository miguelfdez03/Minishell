/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:32 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:42:33 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_input_redir(char *file)
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

int	handle_output_redir(char *file, int append)
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

int	apply_redirections(t_data *data)
{
	t_redir	*redir;
	int		result;
	t_cmd	*cmd;

	cmd = data->cmd;
	if (!cmd)
		return (0);
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (handle_input_redir("/tmp/.minishell_heredoc") == -1)
				return (-1);
			unlink("/tmp/.minishell_heredoc");
		}
		else
		{
			result = process_single_redir(redir, data);
			if (result != 0)
				return (result);
		}
		redir = redir->next;
	}
	return (0);
}

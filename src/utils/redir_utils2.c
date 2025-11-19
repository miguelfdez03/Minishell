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

static int	process_single_redir(t_redir *redir, t_data *data)
{
	int	result;

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
	else if (redir->type == HEREDOC)
	{
		result = handle_heredoc(redir->file, data);
		if (result == -1 || result == -2)
			return (result);
		if (handle_input_redir("/tmp/.minishell_heredoc") == -1)
			return (-1);
		unlink("/tmp/.minishell_heredoc");
	}
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
		result = process_single_redir(redir, data);
		if (result != 0)
			return (result);
		redir = redir->next;
	}
	return (0);
}

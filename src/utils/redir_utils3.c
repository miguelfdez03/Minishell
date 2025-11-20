#include "../minishell.h"

static int	process_heredoc_redir(char *file, t_data *data)
{
	int	result;

	result = handle_heredoc(file, data);
	if (result == -1 || result == -2)
		return (result);
	if (handle_input_redir("/tmp/.minishell_heredoc") == -1)
		return (-1);
	unlink("/tmp/.minishell_heredoc");
	return (0);
}

int	process_single_redir(t_redir *redir, t_data *data)
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
	else if (redir->type == HEREDOC)
		return (process_heredoc_redir(redir->file, data));
	return (0);
}

int	process_all_heredocs(t_data *data)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		result;

	cmd = data->cmd;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				result = handle_heredoc(redir->file, data);
				if (result == -1 || result == -2)
					return (result);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

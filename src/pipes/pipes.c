#include "../minishell.h"

void	setup_child_fds(int input_fd, int output_fd)
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
}

void	exec_cmd_in_child(t_data *data, t_cmd *cmd)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
		close(fd++);
	if (apply_redirections(cmd) == -1)
		exit(1);
	data->cmd = cmd;
	if (cmd->builtin_id != BUILTIN_NONE)
		exit(execute_builtin_by_id(data));
	else
		exit(execute_external_command(data));
}

static int	init_next_cmd(t_cmd *current_cmd, t_token **tokens)
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
		current_cmd->next->name = ft_strdup(tmp->value);
		if (!current_cmd->next->name)
		{
			free(current_cmd->next);
			current_cmd->next = NULL;
			return (-1);
		}
		current_cmd->next->builtin_id = identify_builtin(tmp->value);
		*tokens = (*tokens)->next;
	}
	return (0);
}

static void	process_cmd_args(t_cmd *cmd, t_token **tokens)
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

int	handle_pipe(t_cmd *current_cmd, t_token **tokens)
{
	if (!current_cmd || !tokens || !*tokens)
		return (-1);
	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		ft_printf("syntax error near unexpected token `|'\n");
		return (-1);
	}
	if (init_next_cmd(current_cmd, tokens) == -1)
		return (-1);
	process_cmd_args(current_cmd->next, tokens);
	return (0);
}

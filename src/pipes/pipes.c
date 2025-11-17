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

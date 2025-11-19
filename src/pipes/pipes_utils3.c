#include "../minishell.h"

void	close_all_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
		close(fd++);
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

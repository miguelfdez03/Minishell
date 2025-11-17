#include "../minishell.h"

static int	is_valid_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	if (arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(t_data *data)
{
	int		i;
	int		print_newline;
	int		start;

	if (!data || !data->cmd)
		return (1);
	print_newline = 1;
	i = 0;
	while (data->cmd->args && data->cmd->args[i]
		&& is_valid_n_flag(data->cmd->args[i]))
	{
		print_newline = 0;
		i++;
	}
	start = i;
	while (data->cmd->args && data->cmd->args[i])
	{
		ft_putstr_fd(data->cmd->args[i], 1);
		if (data->cmd->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (print_newline)
		ft_putchar_fd('\n', 1);
	return (0);
}

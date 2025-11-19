#include "../minishell.h"

static int	is_numeric_argument(const char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[0])
		return (0);
	if (arg[0] == '-' || arg[0] == '+')
	{
		i = 1;
		if (!arg[i])
			return (0);
	}
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_exit_args(t_cmd *cmd)
{
	int	arg_count;

	arg_count = 0;
	if (cmd->args)
	{
		while (cmd->args[arg_count])
			arg_count++;
	}
	if (arg_count > 1)
		return (-1);
	if (arg_count == 0 || !cmd->args[0])
		return (0);
	if (!is_numeric_argument(cmd->args[0]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (2);
	}
	return (ft_atoi(cmd->args[0]) & 255);
}

int	builtin_exit(t_data *data)
{
	t_cmd	*cmd;
	int		exit_status;
	int		fd;

	cmd = data->cmd;
	ft_putendl_fd("exit", 2);
	exit_status = check_exit_args(cmd);
	if (exit_status == -1)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (2);
	}
	free_data(data);
	fd = 3;
	while (fd < 1024)
		close(fd++);
	exit(exit_status);
}

#include "../minishell.h"

static int	process_unset_arg(t_data *data, char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("unset: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	unset_env_var(&data->env, arg);
	return (0);
}

int	builtin_unset(t_data *data)
{
	int		i;
	int		status;

	if (!data || !data->cmd)
		return (1);
	if (!data->cmd->args || !data->cmd->args[0])
		return (0);
	status = 0;
	i = 0;
	while (data->cmd->args[i])
	{
		if (process_unset_arg(data, data->cmd->args[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}

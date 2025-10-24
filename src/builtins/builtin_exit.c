/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:07:00 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/24 19:26:21 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_exit_args(t_cmd *cmd);
static int	is_numeric_argument(const char *arg);

int	builtin_exit(t_data *data)
{
	t_cmd	*cmd;
	int		exit_status;

	cmd = data->cmd;
	ft_putendl_fd("exit", 2);
	exit_status = check_exit_args(cmd);
	if (exit_status == -1)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	free_data(data);
	exit(exit_status);
}

static int	is_numeric_argument(const char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i = 1;
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
		ft_printf("exit: %s: numeric argument required\n", cmd->args[0]);
		return (2);
	}
	return (ft_atoi(cmd->args[0]) & 255);
}

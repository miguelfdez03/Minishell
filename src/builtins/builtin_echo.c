/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:51:49 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/25 22:17:09 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO : contemplar las redirs
static int	has_new_line(t_data *data)
{
	int	i;
	t_cmd	*cmd;

	cmd = data->cmd;
	i = 1;
	if (cmd->args[0][0] == '-' && cmd->args[0][1] == 'n')
		while (cmd->args[0][i] == 'n')
			i++;
	if (i == ft_strlen(cmd->args[0]))
		return (0);
	else
		return (1);
}

int	builtin_echo(t_data *data)
{
	t_cmd	*cmd;
	int		i;
	int		newline_flag;

	cmd = data->cmd;
	newline_flag = 1;
	i = 0;
	if (cmd->args && cmd->args[0] && cmd->args[0][0] == '-' && cmd->args[0][1] == 'n')
	{
		newline_flag = has_new_line(data);
		i = 1;
	}
	while (cmd->args && cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}

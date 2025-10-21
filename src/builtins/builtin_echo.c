/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:51:49 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/21 20:31:45 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO : contemplar las redirs
//!! echo -nnnnnnnnnnnnnnnnnnnnnn a
//!! aspidershell> "funciona igual con mil ns que con una"
int	builtin_echo(t_data *data)
{
	t_cmd	*cmd;
	int		i;
	int		newline_flag;

	cmd = data->cmd;
	newline_flag = 1;
	i = 0;
	if (cmd->args && cmd->args[0] && ft_strcmp2(cmd->args[0], "-n") == 0)
	{
		newline_flag = 0;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:51:49 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/03 13:33:52 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int builtin_echo(t_data *data)
{
    t_cmd *cmd;
    int i;
    int newline_flag;

    cmd = (data->cmd);
    newline_flag = 1;
    i = 1;

    if (cmd->args[1] && ft_strcmp2(cmd->args[1], "-n") == 0)
    {
        newline_flag = 0;
        i = 2;
    }

    while(cmd->args[i])
    {
        ft_putstr_fd(cmd->args[i], 1);
        if (cmd->args[i + 1])
            ft_putchar_fd(' ', 1);
        i++;
    }
    
    if (newline_flag)
    {
        ft_putchar_fd('\n', 1);    
    }
    
    return (0);
}

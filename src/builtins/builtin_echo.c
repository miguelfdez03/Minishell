/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:51:49 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/02 13:19:55 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int builtin_echo(t_cmd *cmd)
{
    int i;
    int newline_flag;

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
            ft_putstr_fd(" ", 1);
        i++;
    }
    
    if (newline_flag)
    {
        ft_putstr_fd("\n", 1);    
    }
    
    return (0);
}

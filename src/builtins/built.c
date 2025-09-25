/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel <miguel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:29:57 by miguel            #+#    #+#             */
/*   Updated: 2025/09/25 16:51:59 by miguel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_command(t_cmd *cmd, char **envp)
{
    // Si es comando externo
    if (cmd->type == CMD_EXTERNAL)
         printf("external command \n");
    else
    {
         printf("builtin command \n");
         execute_builtin_by_id(cmd, envp);
    }
}

// Ejecutar built-in usando switch
int	execute_builtin_by_id(t_cmd *cmd, char **envp)
{
    printf("enter here \n");
    switch (cmd->builtin_id)
    {
        case BUILTIN_CD:
            printf("\n builtin cd\n");
            return (0);
        case BUILTIN_PWD:
            printf("\n builtin pwd\n");
            return (0);
        case BUILTIN_EXIT:
            printf("\n builtin exit\n");
            return (0);
        case BUILTIN_ECHO:
            printf("\n builtin echo\n");
            return (0);
        case BUILTIN_ENV:
            printf("\n builtin env\n");
            return (0);
        case BUILTIN_EXPORT:
            printf("\n builtin export\n");
            return (0);
        case BUILTIN_UNSET:
            printf("\n builtin unset\n");
            return (0);
        default:
            printf("\n Unknown builtin\n");
            return (1);
    }
}
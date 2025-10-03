/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:54:11 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/03 11:58:11 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Función para contar argumentos
static int count_args(char **args)
{
    int count;

    count = 0;
    if (!args)
        return (0);
    while (args[count])
        count++;
    return (count);
}

// Función para obtener valor de variable de entorno de la lista
static char *get_env_value_from_list(t_env *env, char *key)
{
    t_env *current;

    current = env;
    while (current)
    {
        if (ft_strcmp2(current->key, key) == 0)
            return (current->value);
        current = current->next;
    }
    return (NULL);
}

int builtin_cd(t_cmd *cmd, t_data *data)
{
    (void)data; // Por ahora no usamos data
    char *path;
    int argc;

    path = NULL;
    argc = count_args(cmd->args);
    if (argc > 2)
    {
        ft_putendl_fd("cd: too many arguments", 2);
        return (1);
    }

    if (argc == 1 || (argc == 2 && ft_strcmp2(cmd->args[1], "~") == 0))
    {
        // Por ahora usar getenv hasta que uses data->env
        path = getenv("HOME");
    }
    else if (argc == 2)
        path = cmd->args[1];
        
    if (!path)
    {
        ft_putendl_fd("cd: HOME not set", 2);
        return (1);
    }
    if (chdir(path) == -1)
    {
        perror("cd");
        return (1);
    }
    
    ft_printf("Changed to: %s\n", path);
    return (0);
}
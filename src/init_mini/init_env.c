/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:31:25 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/02 13:06:59 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_key(char **env, int i, int j)
{
	while(env[i][j] && env[i][j] != '=')
		j++;
	return(ft_substr(env[i], 0, j));	
}

char	*get_env_value_2(char **env, int i, int j)
{
	int start = j;
	
	while(env[i][j] && env[i][j] != '\0')
		j++;
	return(ft_substr(env[i], start, j - start));	
}

int	init_env(char **env, t_env *env_t)
{
	int	i;
	int j;
	
	i = 0;
	while (env[i])
	{
		j = 0;
		env_t->key = get_env_key(env, i, j);
		while (env[i][j] && env[i][j] != '=')
			j++;
		if (env[i][j] == '=')
			j++;
		env_t->value = get_env_value_2(env, i, j);
		env_t->index = i;
		i++;
		if (env[i])
		{
			env_t->next = malloc(sizeof(t_env));
			if (!env_t->next)
				return (-1);
			env_t = env_t->next;
		}
		else
			env_t->next = NULL;
	}
	return (0);
}
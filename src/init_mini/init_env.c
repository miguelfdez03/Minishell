/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:31:25 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/14 20:07:56 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_key(char **env, int i)
{
	char	*key;
	int		j;

	j = 0;
	while (env[i][j] && env[i][j] != '=')
		j++;
	key = ft_substr(env[i], 0, j);
	return (key);
}

char	*get_env_value_2(char **env, int i)
{
	char	*value;
	int		j;
	int		len;

	j = 0;
	while (env[i][j] && env[i][j] != '=')
		j++;
	if (env[i][j] == '=')
	{
		j++;
		len = ft_strlen(env[i]) - j;
		value = ft_substr(env[i], j, len);
	}
	else
		value = NULL;
	return (value);
}

int	init_env(char **env, t_env *env_t)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		env_t->key = get_env_key(env, i);
		env_t->value = get_env_value_2(env, i);
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

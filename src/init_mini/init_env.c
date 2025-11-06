/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:31:25 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/06 11:04:19 by lruiz-to         ###   ########.fr       */
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

static void	no_env_init(t_env *env_t, int i)
{
	env_t->key = ft_strdup("PWD");
	env_t->value = getcwd(NULL, 0);
	env_t->index = i;
	env_t->next = malloc(sizeof(t_env));
	if (!env_t->next)
		return ;
	env_t = env_t->next;
	i++;
	env_t->key = ft_strdup("SHLVL");
	env_t->value = ft_strdup("1");
	env_t->index = i;
	env_t->next = malloc(sizeof(t_env));
	if (!env_t->next)
		return ;
	env_t = env_t->next;
	i++;
	env_t->key = ft_strdup("PATH");
	env_t->value = ft_strdup("/bin:/usr/bin");
	env_t->index = i;
	i++;
	env_t->key = ft_strdup("_");
	env_t->value = ft_strdup("/usr/bin/env");
	env_t->index = i;
	env_t->next = NULL;
}

int	init_env(char **env, t_env *env_t)
{
	int	i;
	int	j;

	i = 0;
	if (!env[0])
		no_env_init(env_t, i);
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

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

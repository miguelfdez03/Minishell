/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:31:25 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/01 19:20:50 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_key(char **env, int i, int j)
{
	while(env[i][j] != 32)
		j++;
	return(ft_substr(env[i], 0, j + 1));	
}

char	*get_env_value_2(char **env, int i, int j)
{
	while(env[i][j] != "\0")
		j++;
	return(ft_substr(env[i], 0, j));	
}
//!!CHECK THIS 
int	init_env(char **env, t_env *env_t)
{
	int	i;
	int j;
	
	i = 0;
	while (env[i])
	{
		env_t->key = get_env_key(env, i, j);
		while (j <= ft_strlen(env_t->key) + 2)
			j++;
		env_t->value = get_env_value_2(env, i, j);		
		i++;
		env_t = env_t->next;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 10:32:09 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/16 12:10:03 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_var_char(char c, int is_first)
{
	if (is_first)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

char	*get_var_value(char *var_name, t_env *env)
{
	t_env	*current;

	if (!var_name || !env)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->key && ft_strcmp2(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char	*extract_var_name(char *str, int start, int *len)
{
	int		i;
	char	*name;

	i = 0;
	*len = 0;
	if (!str || !is_valid_var_char(str[start], 1))
		return (NULL);
	while (str[start + i] && is_valid_var_char(str[start + i], i == 0))
		i++;
	*len = i;
	name = ft_substr(str, start, i);
	return (name);
}

char	*append_to_result(char *result, char *to_add)
{
	char	*new_result;

	if (!to_add)
		return (result);
	if (!result)
		return (ft_strdup(to_add));
	new_result = ft_strjoin(result, to_add);
	free(result);
	return (new_result);
}

char	*append_char(char *result, char c)
{
	char	*new_result;
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	new_result = append_to_result(result, temp);
	return (new_result);
}

char	*expand_exit_status(char *result, int exit_status, int *i)
{
	char	*status_str;
	char	*new_result;

	status_str = ft_itoa(exit_status);
	new_result = append_to_result(result, status_str);
	free(status_str);
	*i += 2;
	return (new_result);
}

char	*expand_pid(char *result, int *i)
{
	char	*pid_str;
	char	*new_result;

	pid_str = ft_itoa(getpid());
	new_result = append_to_result(result, pid_str);
	free(pid_str);
	*i += 2;
	return (new_result);
}


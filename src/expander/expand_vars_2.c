/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 11:15:55 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/26 00:16:19 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_braced_var(char *str, int *i, t_env *env, char *result)
{
	int		j;
	int		len;
	char	*var_name;
	char	*value;
	char	*new_result;

	j = *i + 2;
	len = 0;
	while (str[j + len] && str[j + len] != '}')
		len++;
	if (str[j + len] != '}')
	{
		new_result = append_to_result(result, "${");
		*i += 2;
		return (new_result);
	}
	var_name = ft_substr(str, j, len);
	value = get_var_value(var_name, env);
	free(var_name);
	new_result = append_to_result(result, value);
	*i += len + 3;
	return (new_result);
}

char	*expand_simple_var(char *str, int *i, t_env *env, char *result)
{
	char	*var_name;
	char	*value;
	char	*new_result;
	int		len;

	var_name = extract_var_name(str, *i + 1, &len);
	if (!var_name || len == 0)
	{
		new_result = append_char(result, '$');
		*i += 1;
		return (new_result);
	}
	value = get_var_value(var_name, env);
	free(var_name);
	new_result = append_to_result(result, value);
	*i += len + 1;
	return (new_result);
}

char	*handle_dollar(char *str, int *i, t_env *env, int exit_status)
{
	char	*result;

	result = NULL;
	if (str[*i + 1] == '?')
		return (expand_exit_status(result, exit_status, i));
	else if (str[*i + 1] == '$')
		return (expand_pid(result, i));
	else if (str[*i + 1] == '{')
		return (expand_braced_var(str, i, env, result));
	else if (is_valid_var_char(str[*i + 1], 1))
		return (expand_simple_var(str, i, env, result));
	else
	{
		result = append_char(result, '$');
		*i += 1;
		return (result);
	}
}

char	*expand_string(char *str, t_env *env, int exit_status)
{
	char	*result;
	char	*expansion;
	int		i;

	if (!str)
		return (NULL);
	result = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			expansion = handle_dollar(str, &i, env, exit_status);
			result = append_to_result(result, expansion);
			free(expansion);
		}
		else
		{
			result = append_char(result, str[i]);
			i++;
		}
	}
	return (result);
}

void	expand_variables(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == SIMPLE_Q)
		{
			current = current->next;
			continue ;
		}
		if ((current->type == WORD || current->type == STRING)
			&& current->value)
		{
			expanded = expand_string(current->value, env, exit_status);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:12 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/23 18:45:26 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sort_env_list(t_env *env)
{
	t_env	*current;
	t_env	*next;
	int		swapped;

	if (!env)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = env;
		while (current && current->next)
		{
			next = current->next;
			if (ft_strcmp2(current->key, next->key) > 0)
			{
				swap_env_nodes(current, next);
				swapped = 1;
			}
			current = current->next;
		}
	}
}

static int	export_without_args(t_env *env)
{
	t_env	*current;

	if (!env)
		return (0);
	sort_env_list(env);
	current = env;
	while (current)
	{
		if (current->key)
		{
			if (current->value)
				ft_printf("declare -x %s=\"%s\"\n",
					current->key, current->value);
			else
				ft_printf("declare -x %s\n", current->key);
		}
		current = current->next;
	}
	return (0);
}

static int	export_with_assignment(t_data *data, char *key, char *value)
{
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(key, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	set_env_value(&data->env, key, value);
	return (0);
}

static int	process_export_arg(t_data *data, char *arg)
{
	char	*key;
	char	*value;
	int		status;

	if (!arg || arg[0] == '=')
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	parse_export_arg(arg, &key, &value);
	if (!key)
		return (1);
	status = export_with_assignment(data, key, value);
	free(key);
	if (value)
		free(value);
	return (status);
}

int	builtin_export(t_data *data)
{
	int		i;
	int		status;

	if (!data || !data->cmd)
		return (1);
	if (!data->cmd->args || !data->cmd->args[0])
		return (export_without_args(data->env));
	status = 0;
	i = 0;
	while (data->cmd->args[i])
	{
		if (process_export_arg(data, data->cmd->args[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}

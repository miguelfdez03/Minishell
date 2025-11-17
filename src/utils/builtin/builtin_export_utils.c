#include "../../minishell.h"

void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*extract_key(char *arg, char *equal_pos)
{
	int		len;
	char	*key;

	len = equal_pos - arg;
	key = malloc(sizeof(char) * (len + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, arg, len + 1);
	return (key);
}

static char	*extract_value(char *equal_pos)
{
	char	*value;

	value = ft_strdup(equal_pos + 1);
	return (value);
}

void	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal_pos;

	*key = NULL;
	*value = NULL;
	if (!arg)
		return ;
	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		if (equal_pos == arg)
		{
			*key = ft_strdup("=");
			*value = NULL;
			return ;
		}
		*key = extract_key(arg, equal_pos);
		*value = extract_value(equal_pos);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

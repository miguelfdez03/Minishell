#include "../minishell.h"

int	count_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	**env_list_to_array(t_env *env)
{
	char	**env_array;
	char	*temp;
	int		i;

	env_array = malloc(sizeof(char *) * (count_env_vars(env) + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env)
	{
		temp = ft_strjoin(env->key, "=");
		env_array[i] = ft_strjoin(temp, env->value);
		free(temp);
		env = env->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

static int	count_cmd_args(t_cmd *cmd)
{
	int	count;
	int	i;

	count = 1;
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			count++;
			i++;
		}
	}
	return (count);
}

char	**build_args_array(t_cmd *cmd)
{
	char	**args;
	int		count;
	int		i;

	count = count_cmd_args(cmd);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	args[0] = cmd->name;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		args[i + 1] = cmd->args[i];
		i++;
	}
	args[count] = NULL;
	return (args);
}

char	*check_absolute_path(char *cmd)
{
	struct stat	path_stat;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/') 
		|| ft_strcmp2(cmd, ".") == 0 || ft_strcmp2(cmd, "..") == 0)
	{
		if (stat(cmd, &path_stat) != 0)
			return (NULL);
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Is a directory", 2);
			return ((char *)-1);
		}
		if (access(cmd, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Permission denied", 2);
			return ((char *)-1);
		}
		return (ft_strdup(cmd));
	}
	return (NULL);
}

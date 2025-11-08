#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_env	*env_t;
	t_data	*data;

	if (init_data(&data, env, env_t) == -1)
	{
		ft_printf("Error: Failed to initialize data\n");
		return (-1);
	}
	main_loop(argc, argv, &data);
	free_data(data);
	return (0);
}

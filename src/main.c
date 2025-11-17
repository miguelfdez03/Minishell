#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_env	*env_t;
	t_data	*data;
	int		exit_code;

	setup_signals_interactive();
	if (init_data(&data, env, env_t) == -1)
	{
		ft_printf("Error: Failed to initialize data\n");
		return (-1);
	}
	exit_code = main_loop(argc, argv, &data);
	rl_clear_history();
	free_data(data);
	return (exit_code);
}

#include "../../minishell.h"

int	init_first_cmd(t_data **data, t_token **tmp)
{
	if (*tmp && (*tmp)->value)
	{
		if (create_cmd((*tmp)->value, (*data)->cmd) == 0)
			return (EXIT_FAILURE);
		*tmp = (*tmp)->next;
	}
	return (EXIT_SUCCESS);
}

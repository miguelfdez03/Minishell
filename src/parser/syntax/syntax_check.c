>>>>>>> origin/executor
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_syntax(t_data **data)
{
	t_token	*tokens;

	tokens = (*data)->tokens;
<<<<<<< HEAD
	t_token	*tokens;

	tokens = (*data)->tokens;
	if (tokens->type == PIPE)
		return (EXIT_FAILURE);
}

=======
	if (tokens->type == PIPE)
		return (EXIT_FAILURE);
}
>>>>>>> origin/executor

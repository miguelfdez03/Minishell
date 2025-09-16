/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel <miguel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:27:58 by miguel            #+#    #+#             */
/*   Updated: 2025/09/16 14:31:05 by miguel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Función de prueba para entender fork y execve
int	test_executor(void)
{
	printf("🕷️ Testing Spider-Shell Executor\n");

	// Test 1: Ejecutar un comando simple con fork + execve
	printf("🕸️ Test 1: Executing 'ls' command\n");

	pid_t pid = fork();

	if (pid == 0)
	{
		// Proceso hijo: ejecutar 'ls'
		char *args[] = {"ls", NULL};
		execve("/bin/ls", args, NULL);

		// Si llegamos aquí, execve falló
		perror("execve failed");
		exit(1);
	}
	else if (pid > 0)
	{
		// Proceso padre: esperar al hijo
		int status;
		waitpid(pid, &status, 0);
		printf("🕷️ Command finished with status: %d\n", status);
	}
	else
	{
		perror("fork failed");
		return (1);
	}

	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel <miguel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:27:58 by miguel            #+#    #+#             */
/*   Updated: 2025/09/16 19:07:13 by miguel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Variable global para acceder al entorno
extern char	**environ;

// Función de prueba para entender fork y execve
int	test_executor(void)
{
	printf("🕷️ Spider-Shell Executor Testing Suite\n");
	// Test 1: Comando simple
	printf("\n🕸️ Test 1: Simple command 'ls'\n");
	test_simple_command_with_path("ls", (char *[]){"ls", NULL});
	// Test 2: Comando con argumentos
	printf("\n🕸️ Test 2: Command with arguments 'ls -la'\n");
	test_simple_command_with_path("ls", (char *[]){"ls", "-la", NULL});
	// Test 3: Echo command
	printf("\n🕸️ Test 3: Echo command\n");
	test_simple_command_with_path("echo", (char *[]){"echo",
		"Hello Spider-Shell!", NULL});
	return (0);
}

// Nueva función que usa PATH
int	test_simple_command_with_path(char *cmd, char **args)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	// Buscar comando en PATH
	cmd_path = find_command_path(cmd, environ);
	if (!cmd_path)
	{
		printf("❌ Command not found: %s\n", cmd);
		return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		// Proceso hijo: ejecutar comando
		if (execve(cmd_path, args, environ) == -1)
		{
			perror("execve failed");
			free(cmd_path);
			exit(127);
		}
	}
	else if (pid > 0)
	{
		// Proceso padre: esperar al hijo
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			printf("✅ Command completed with exit code: %d\n",
				WEXITSTATUS(status));
		else
			printf("❌ Command terminated abnormally\n");
		free(cmd_path);
	}
	else
	{
		perror("fork failed");
		free(cmd_path);
		return (1);
	}
	return (0);
}

// Función auxiliar para ejecutar un comando (método antiguo para comparar)
int	test_simple_command(char *cmd_path, char **args)
{
	pid_t pid;
	int status;

	pid = fork();

	if (pid == 0)
	{
		// Proceso hijo: buscar comando en PATH
		char full_path[256];
		snprintf(full_path, sizeof(full_path), "/bin/%s", cmd_path);

		if (execve(full_path, args, NULL) == -1)
		{
			// Si no está en /bin, probar /usr/bin
			snprintf(full_path, sizeof(full_path), "/usr/bin/%s", cmd_path);
			if (execve(full_path, args, NULL) == -1)
			{
				perror("Command not found");
				exit(127);
					// Código de error estándar para comando no encontrado
			}
		}
	}
	else if (pid > 0)
	{
		// Proceso padre: esperar al hijo
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
			printf("✅ Command completed with exit code: %d\n",
				WEXITSTATUS(status));
		else
			printf("❌ Command terminated abnormally\n");
	}
	else
	{
		perror("fork failed");
		return (1);
	}

	return (0);
}
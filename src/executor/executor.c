/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:37 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:18:54 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: handle_exec_failure
 * ---------------------------
 * Maneja el fallo de execve en el proceso hijo.
 * 
 * Cuando execve falla:
 * 1. Muestra el error con perror
 * 2. Libera cmd_path, args y env_array
 * 3. Sale del proceso hijo con código 127 (comando no encontrado)
 * 
 * cmd_path: Ruta del comando
 * args: Array de argumentos
 * env_array: Array de variables de entorno
 */
static void	handle_exec_failure(char *cmd_path, char **args, char **env_array)
{
	perror("minishell: execve");
	free(cmd_path);
	free(args);
	free_string_array(env_array);
	exit(127);
}

/*
 * Función: exec_child_process
 * --------------------------
 * Ejecuta el comando en el proceso hijo.
 * 
 * Pasos:
 * 1. Configura las señales para proceso hijo
 * 2. Aplica las redirecciones (entrada/salida)
 * 3. Si hay error en redirecciones: libera memoria y sale con código 1
 * 4. Ejecuta el comando con execve
 * 5. Si execve falla: maneja el error
 * 
 * data: Estructura con redirecciones
 * cmd_path: Ruta completa del comando
 * args: Array de argumentos
 * env_array: Array de variables de entorno
 */
static void	exec_child_process(t_data *data, char *cmd_path,
		char **args, char **env_array)
{
	setup_signals_child();
	if (apply_redirections(data) == -1)
	{
		free(cmd_path);
		free(args);
		free_string_array(env_array);
		exit(1);
	}
	if (execve(cmd_path, args, env_array) == -1)
		handle_exec_failure(cmd_path, args, env_array);
}

/*
 * Función: prepare_execution
 * -------------------------
 * Prepara todo lo necesario para ejecutar un comando externo.
 * 
 * Pasos:
 * 1. Busca el comando en PATH o verifica ruta absoluta
 *    - Si es directorio: retorna 126
 *    - Si no existe: muestra error y retorna 127
 * 2. Construye array de argumentos
 * 3. Convierte lista de env a array
 * 4. Si falla malloc: libera memoria y retorna 1
 * 
 * data: Estructura con el comando
 * cmd_path: Salida - ruta del comando
 * args: Salida - array de argumentos
 * env_array: Salida - array de variables de entorno
 * 
 * Retorna: 0 si éxito, código de error si falla
 */
static int	prepare_execution(t_data *data, char **cmd_path, char ***args,
		char ***env_array)
{
	*cmd_path = find_cmd_in_path(data->cmd->name, data->env);
	if (*cmd_path == (char *)-1)
		return (126);
	if (!*cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(data->cmd->name, 2);
		return (127);
	}
	*args = build_args_array(data->cmd);
	*env_array = env_list_to_array(data->env);
	if (!*args || !*env_array)
	{
		free(*cmd_path);
		if (*args)
			free(*args);
		if (*env_array)
			free_string_array(*env_array);
		return (1);
	}
	return (0);
}

/*
 * Función: find_cmd_in_path
 * ------------------------
 * Busca el comando en PATH o valida ruta absoluta.
 * 
 * Proceso:
 * 1. Verifica si es ruta absoluta o relativa (/, ./, ., ..)
 * 2. Si no es absoluta: busca en directorios de PATH
 * 3. Convierte lista de env a array para buscar PATH
 * 
 * cmd: Nombre del comando
 * env: Lista de variables de entorno
 * 
 * Retorna: Ruta completa del comando, (char *)-1 si es directorio,
 *          NULL si no se encuentra
 */
char	*find_cmd_in_path(char *cmd, t_env *env)
{
	char	**env_array;
	char	*cmd_path;
	char	*abs_path;

	if (!cmd || !env)
		return (NULL);
	abs_path = check_absolute_path(cmd);
	if (abs_path)
		return (abs_path);
	env_array = env_list_to_array(env);
	if (!env_array)
		return (NULL);
	cmd_path = find_command_path(cmd, env_array);
	free_string_array(env_array);
	return (cmd_path);
}

/*
 * Función: execute_external_command
 * --------------------------------
 * Ejecuta un comando externo (no builtin).
 * 
 * Flujo completo:
 * 1. Procesa heredocs primero
 * 2. Prepara la ejecución (busca comando, construye args y env)
 * 3. Configura señales para modo ejecución
 * 4. Hace fork para crear proceso hijo
 * 5. Proceso hijo: aplica redirecciones y ejecuta con execve
 * 6. Proceso padre: espera al hijo y recoge exit status
 * 7. Restaura señales a modo interactivo
 * 
 * data: Estructura con el comando y redirecciones
 * 
 * Retorna: Código de salida del comando ejecutado
 */
int	execute_external_command(t_data *data)
{
	pid_t	pid;
	char	*cmd_path;
	char	**args;
	char	**env_array;
	int		ret;

	if (process_all_heredocs(data) != 0)
		return (data->exit_status);
	ret = prepare_execution(data, &cmd_path, &args, &env_array);
	if (ret != 0)
		return (ret);
	setup_signals_executing();
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(cmd_path, args, env_array));
	if (pid == 0)
		exec_child_process(data, cmd_path, args, env_array);
	ret = wait_and_cleanup(pid, cmd_path, args, env_array);
	setup_signals_interactive();
	return (ret);
}

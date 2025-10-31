# 📋 TODO - Parser de Minishell (Estado Actualizado)

> **Última actualización:** 31 de octubre de 2025  
> **Estado:** Parser funcional con pipes implementados  
> **Testing:** Completo (66 tests ejecutados, 0 memory leaks)  
> **Siguiente fase:** Implementación de executor y validación de sintaxis



---

## 📊 ESTADO ACTUAL DEL PROYECTO

### ✅ COMPLETAMENTE IMPLEMENTADO

#### 🎯 Parser y Tokenización
- ✅ Tokenización completa (WORD, STRING, SIMPLE_Q, ARGS, PIPE, REDIR_*)
- ✅ Manejo de comillas simples y dobles
- ✅ Expansión de variables ($VAR, $?, $$, ${VAR})
- ✅ Comillas simples NO expanden variables (correcto)
- ✅ Detección de comillas sin cerrar + input multilinea
- ✅ Detección de redirecciones (<, >, <<, >>)
- ✅ Detección de pipes con creación de lista enlazada

#### 🔗 Pipes (NUEVO - Implementado completamente)
- ✅ **Lista enlazada de comandos** (`src/pipes/pipes.c`)
  - `init_next_cmd()` - Crea siguiente comando en la lista
  - `process_cmd_args()` - Añade argumentos hasta encontrar PIPE
  - `handle_pipe()` - Procesa un solo pipe
  - `process_pipes()` - Loop principal para múltiples pipes
- ✅ **Estructura `t_cmd->next`** para linked list
- ✅ **Memory management perfecto** (0 leaks con hasta 6 comandos en pipe)
- ✅ **Bug fix crítico**: `ft_strdup("|")` en checker.c (antes causaba `free(): invalid pointer`)
- ✅ **Documentación completa**: `PIPES_IMPLEMENTATION.md` (1552 líneas)
- ✅ **Testing exhaustivo**: Probado con múltiples comandos en pipeline

#### 🔨 Builtins
- ✅ **EXIT**: 100% funcional (5/5 tests)
- ✅ **CD**: 75% funcional (6/8 tests) - Solo formato de mensajes difiere
  - HOME, OLDPWD, ~, - implementados
  - Detección de permisos funcional
- ✅ **ECHO**: 30% funcional (7/23 tests)
  - Echo básico funciona
  - ⚠️ Falta: flag -n, concatenación de quotes
- ✅ **PWD**: Funcional (rechaza args cuando bash los ignora)
- ✅ **ENV**: Funcional (lista variables correctamente)
- ⚠️ **EXPORT**: 0% funcional - NO exporta al entorno real (setenv faltante)
- ❌ **UNSET**: No implementado (solo stub)

#### 🧠 Gestión de Memoria
- ✅ **0 memory leaks en 66 tests** 🎉
- ✅ `free_tokens()` funciona correctamente
- ✅ `free_cmd()` libera lista enlazada completa
- ✅ `free_data()` limpia toda la estructura
- ✅ Valgrind reports clean en todos los tests

#### 📝 Documentación y Testing
- ✅ `PIPES_IMPLEMENTATION.md` - Guía completa de implementación
- ✅ `ECHO_TEST_REPORT.md` - 23 tests analizados
- ✅ `EXPORT_TEST_REPORT.md` - 20 tests con soluciones
- ✅ `CD_TEST_REPORT.md` - 8 tests detallados
- ✅ `OTHER_BUILTINS_TEST_REPORT.md` - ENV, PWD, UNSET, EXIT
- ✅ `BUILTINS_TEST_REPORT.md` - Resumen general
- ✅ Scripts de test automatizados:
  - `test_echo_simple.sh`
  - `test_export.sh`
  - `test_cd.sh`
  - `test_builtins.sh`

### ⚠️ PARCIALMENTE IMPLEMENTADO

- ⚠️ **Redirecciones**: Detectadas pero no aplicadas en ejecución
- ⚠️ **Heredocs**: Token HEREDOC creado pero sin lectura de contenido
- ⚠️ **Execution de pipes**: Parsing completo pero executor no implementado

### ❌ NO IMPLEMENTADO

- ❌ **Validación de sintaxis** (pipes al inicio/final, redirs mal formadas)
- ❌ **Executor de pipelines** (fork/pipe/dup2)
- ❌ **Lectura funcional de heredocs**
- ❌ **Aplicación completa de redirecciones**
- ❌ **Wildcards** (bonus)
- ❌ **Operadores lógicos && y ||** (bonus)



<details>---```

<summary>📄 Código: syntax_validator.c (Click para expandir)</summary>



```c

#include "../minishell.h"## 🎯 TAREAS PRIORITARIAS (Orden recomendado)**Implementación sugerida:**



static int	is_redir_token(t_token_type type)```

{

	return (type == REDIR_IN || type == REDIR_OUT ### 1. 🔴 VALIDACIÓN DE SINTAXISFUNCIÓN validate_syntax(tokens):

		|| type == REDIR_APPEND || type == HEREDOC);

}**Prioridad: CRÍTICA** (Hazlo primero para evitar crashes)      current = primer_token



static int	validate_redir(t_token *current)**Tiempo estimado: 2-3 horas**      

{

	if (!current->next)**Archivos:**    // 1. Verificar que no empiece con pipe

	{

		ft_printf("syntax error near unexpected token `newline'\n");- `src/parser/syntax_validator.c` (CREAR)    SI current ES PIPE:

		return (0);

	}- `src/parser/lexer.c` (MODIFICAR)        RETORNAR error("syntax error near unexpected token `|'")

	if (is_redir_token(current->next->type) || current->next->type == PIPE)

	{- `src/minishell.h` (AÑADIR prototipo)    

		ft_printf("syntax error near unexpected token `%s'\n", 

			current->next->value);    MIENTRAS current exista:

		return (0);

	}#### Problemas a detectar:        // 2. Verificar redirecciones

	return (1);

}```bash        SI current ES redirección (<, >, <<, >>):



static int	validate_pipe(t_token *current)# Pipes incorrectas            SI NO hay siguiente token O siguiente NO es WORD:

{

	if (!current->next)|                    # Error: comando vacío antes de pipe                RETORNAR error("syntax error near unexpected token")

	{

		ft_printf("syntax error near unexpected token `|'\n");ls |                 # Error: comando vacío después de pipe          

		return (0);

	}ls | | cat           # Error: pipes consecutivas        // 3. Verificar pipes

	if (current->next->type == PIPE)

	{        SI current ES PIPE:

		ft_printf("syntax error near unexpected token `|'\n");

		return (0);# Redirecciones incorrectas            SI NO hay siguiente token:

	}

	return (1);ls >                 # Error: sin archivo después de redirección                RETORNAR error("syntax error near unexpected token `|'")

}

cat < >              # Error: sin archivo            SI siguiente ES PIPE:

int	validate_syntax(t_token *tokens)

{ls > > file          # Error: redirecciones consecutivas sin archivo                RETORNAR error("syntax error near unexpected token `|'")

	t_token	*current;

echo hello > | cat   # Error: pipe justo después de redirección        

	if (!tokens)

		return (1);        current = siguiente_token

	current = tokens;

	if (current->type == PIPE)# Heredocs incorrectos    

	{

		ft_printf("syntax error near unexpected token `|'\n");cat <<               # Error: sin delimitador    RETORNAR éxito

		return (0);

	}cat << | ls          # Error: pipe en lugar de delimitador```

	while (current)

	{

		if (is_redir_token(current->type))

		{# Combinaciones inválidas---

			if (!validate_redir(current))

				return (0);> file | cat         # Error: empieza con redirección

		}

		else if (current->type == PIPE)< file >             # Error: redirección sin archivo### 2. 🔴 PIPES COMPLETOS (Lista enlazada de comandos)

		{

			if (!validate_pipe(current))```**Prioridad: CRÍTICA**  

				return (0);

		}**Archivos a crear/modificar:**

		current = current->next;

	}#### Implementación:- `src/parser/lexer.c` (modificar)

	return (1);

}- `src/executor/pipeline.c` (nuevo)

```

</details>**Archivo: `src/parser/syntax_validator.c`**



<details>```c**Problema actual:**

<summary>✏️ Modificación: lexer.c (Click para expandir)</summary>

/* ************************************************************************** */```c

```c

int	lexer(char *line, t_data **data)/*                                                                            */// Ahora solo tienes UN comando:

{

	int	i;/*                                                        :::      ::::::::   */t_data->cmd = [comando único]



	i = 0;/*   syntax_validator.c                                 :+:      :+:    :+:   */

	while (line[i] && i >= 0)

	{/*                                                    +:+ +:+         +:+     */// Necesitas una LISTA de comandos:

		i = handle_lexer_char(line, i, data);

		if (i == -1)/*   By: tu-usuario <tu-email@student.42.fr>        +#+  +:+       +#+        */t_data->cmd = [cmd1] -> [cmd2] -> [cmd3] -> NULL

			return (EXIT_FAILURE);

		if (i == -2)/*                                                +#+#+#+#+#+   +#+           */```

			break ;

	}/*   Created: YYYY/MM/DD HH:MM:SS by tu-usuario      #+#    #+#             */

	

	// ⭐ AÑADIR ESTA VALIDACIÓN:/*   Updated: YYYY/MM/DD HH:MM:SS by tu-usuario     ###   ########.fr       */**Implementación en el parser:**

	if (!validate_syntax((*data)->tokens))

	{/*                                                                            */```c

		free_tokens((*data)->tokens);

		(*data)->tokens = NULL;/* ************************************************************************** */// En lexer.c - función check_and_exp()

		return (EXIT_FAILURE);

	}// Cuando encuentres un token PIPE:

	

	check_and_exp(data);#include "../minishell.h"

	return (i);

}static int check_and_exp(t_data **data)

```

</details>// Verifica si el tipo de token es una redirección{



**Añadir a minishell.h:**static int	is_redir_token(t_token_type type)    t_token *tmp;

```c

int		validate_syntax(t_token *tokens);{    t_cmd *current_cmd;

```

	return (type == REDIR_IN || type == REDIR_OUT     

---

		|| type == REDIR_APPEND || type == HEREDOC);    if (!(*data)->tokens)

### 2️⃣ Lista Enlazada de Comandos (4-6 horas) 🔴 CRÍTICO

}        return (EXIT_FAILURE);

**¿Por qué?** Sin esto, las pipes no funcionan (30% de la nota)

    

**Problema actual:**

```// Valida que después de una redirección haya un archivo válido    expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);

Input:  "ls | grep test | wc -l"

Ahora:  data->cmd = [solo "ls"]  ❌static int	validate_redir(t_token *current)    tmp = (*data)->tokens;

Necesitas: data->cmd = [ls] -> [grep test] -> [wc -l] -> NULL  ✅

```{    current_cmd = (*data)->cmd;



**Archivos:**	if (!current->next)    

- ✏️ `src/parser/lexer.c` (refactorizar check_and_exp)

- ✏️ `src/utils/cmd_utils.c` (modificar free_cmd, añadir debug)	{    while (tmp)



<details>		ft_printf("syntax error near unexpected token `newline'\n");    {

<summary>📝 Funciones auxiliares para lexer.c (Click para expandir)</summary>

		return (0);        if (tmp->type == PIPE)

```c

// Inicializa comando desde token	}        {

static int	init_cmd_from_token(t_cmd *cmd, t_token **tmp)

{	if (is_redir_token(current->next->type) || current->next->type == PIPE)            // NUEVO: Crear siguiente comando en la lista

	if (!(*tmp) || !(*tmp)->value)

		return (EXIT_FAILURE);	{            current_cmd->next = create_new_cmd();

	cmd->name = ft_strdup((*tmp)->value);

	if (create_cmd(cmd->name, cmd) == 0)		ft_printf("syntax error near unexpected token `%s'\n",             current_cmd = current_cmd->next;

		return (EXIT_FAILURE);

	*tmp = (*tmp)->next;			current->next->value);            tmp = tmp->next;

	return (EXIT_SUCCESS);

}		return (0);            continue;



// Procesa tokens hasta encontrar pipe	}        }

static void	process_cmd_tokens(t_cmd *cmd, t_token **tmp)

{	return (1);        

	while (*tmp && (*tmp)->type != PIPE)

	{}        // Procesar token normal...

		if ((*tmp)->value && ((*tmp)->type == WORD || (*tmp)->type == STRING

				|| (*tmp)->type == ARGS || (*tmp)->type == SIMPLE_Q))        if (tmp->type == WORD && !current_cmd->name)

			add_cmd_arg(cmd, (*tmp)->value);

		*tmp = (*tmp)->next;// Valida que después de un pipe haya un comando            current_cmd->name = ft_strdup(tmp->value);

	}

}static int	validate_pipe(t_token *current)        else if (tmp->value)



// Crea siguiente comando en la lista{            add_cmd_arg(current_cmd, tmp->value);

static int	create_next_cmd(t_cmd **current, t_token **tmp)

{	if (!current->next)            

	if (!*tmp || (*tmp)->type != PIPE)

		return (0);	{        tmp = tmp->next;

	*tmp = (*tmp)->next; // Saltar pipe

	if (!*tmp)		ft_printf("syntax error near unexpected token `|'\n");    }

		return (0);

	(*current)->next = malloc(sizeof(t_cmd));		return (0);    

	if (!(*current)->next)

		return (-1);	}    return (EXIT_SUCCESS);

	*current = (*current)->next;

	init_cmd(*current);	if (current->next->type == PIPE)}

	return (1);

}	{```



// Función principal refactorizada		ft_printf("syntax error near unexpected token `|'\n");

static int	check_and_exp(t_data **data)

{		return (0);**Implementación en el executor:**

	t_token	*tmp;

	t_cmd	*current_cmd;	}Ver archivo `PIPES_IMPLEMENTATION.md` para detalles completos.

	int		status;

	return (1);

	if (!(*data)->tokens)

		return (EXIT_FAILURE);}---

	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);

	tmp = (*data)->tokens;

	current_cmd = (*data)->cmd;

	// Función principal de validación### 3. 🟡 HEREDOCS FUNCIONALES

	if (init_cmd_from_token(current_cmd, &tmp) == EXIT_FAILURE)

		return (EXIT_FAILURE);int	validate_syntax(t_token *tokens)**Prioridad: ALTA**  

	

	while (tmp){**Archivos a crear/modificar:**

	{

		process_cmd_tokens(current_cmd, &tmp);	t_token	*current;- `src/parser/heredoc.c` (nuevo)

		status = create_next_cmd(&current_cmd, &tmp);

		if (status == -1)- `src/parser/checker.c` (modificar)

			return (EXIT_FAILURE);

		if (status == 0)	if (!tokens)

			break;

		if (init_cmd_from_token(current_cmd, &tmp) == EXIT_FAILURE)		return (1);**Problema actual:**

			return (EXIT_FAILURE);

	}	current = tokens;Solo detectas `<<` pero no lees el contenido del heredoc.

	return (EXIT_SUCCESS);

}	// Validar que no empiece con pipe

```

</details>	if (current->type == PIPE)**Cómo debe funcionar:**



<details>	{```bash

<summary>✏️ Modificar free_cmd() en cmd_utils.c (Click para expandir)</summary>

		ft_printf("syntax error near unexpected token `|'\n");cat << EOF

```c

void	free_cmd(t_cmd *cmd)		return (0);line 1

{

	t_cmd	*next;	}line 2

	int		i;

	while (current)line 3

	while (cmd)  // ⭐ Cambio: recorrer TODA la lista

	{	{EOF

		next = cmd->next;

		if (cmd->name)		if (is_redir_token(current->type))```

			free(cmd->name);

		if (cmd->args)		{

		{

			i = 0;			if (!validate_redir(current))**Implementación sugerida:**

			while (cmd->args[i])

			{				return (0);```

				free(cmd->args[i]);

				i++;		}FUNCIÓN read_heredoc(delimiter, env, debe_expandir):

			}

			free(cmd->args);		else if (current->type == PIPE)    contenido = vacío

		}

		if (cmd->redirections)		{    

			free_redirs(cmd->redirections);

		free(cmd);			if (!validate_pipe(current))    BUCLE INFINITO:

		cmd = next;  // ⭐ Continuar con siguiente

	}				return (0);        linea = leer_linea_con_prompt("> ")

}

```		}        

</details>

		current = current->next;        SI linea es NULL (EOF):

<details>

<summary>🐛 Función de debug (opcional pero útil)</summary>	}            IMPRIMIR "warning: here-document delimited by end-of-file"



```c	return (1);            ROMPER

void	print_cmd_list(t_cmd *cmd)

{}        

	int	i;

	int	num;```        SI linea == delimiter:



	num = 1;            LIBERAR linea

	while (cmd)

	{**Modificar `src/parser/lexer.c`:**            ROMPER

		ft_printf("\n🔹 CMD %d: %s\n", num, cmd->name);

		if (cmd->args)```c        

		{

			ft_printf("   Args: ");// Añadir al final de la función lexer(), antes de check_and_exp():        // Expandir variables si debe_expandir = verdadero

			i = 0;

			while (cmd->args[i])int	lexer(char *line, t_data **data)        SI debe_expandir:

				ft_printf("[%s] ", cmd->args[i++]);

			ft_printf("\n");{            linea = expandir_variables(linea, env)

		}

		if (cmd->redirections)	int	i;        

			ft_printf("   ⚠ Tiene redirecciones\n");

		cmd = cmd->next;        contenido = unir(contenido, linea)

		num++;

	}	i = 0;        contenido = unir(contenido, "\n")

}

```	while (line[i] && i >= 0)        LIBERAR linea

</details>

	{    

---

		i = handle_lexer_char(line, i, data);    RETORNAR contenido

### 3️⃣ Executor de Pipelines (6-8 horas) 🔴 CRÍTICO

		if (i == -1)

**¿Por qué?** Sin esto, los comandos con pipe no se ejecutan

			return (EXIT_FAILURE);// Detectar si el delimitador tiene comillas:

**Concepto:**

```		if (i == -2)// << EOF    → expandir variables

Comando: ls | grep test | wc -l

			break ;// << 'EOF'  → NO expandir variables

Proceso:

[ls] --pipe1--> [grep test] --pipe2--> [wc -l] --> pantalla	}// << "EOF"  → expandir variables



Pasos:	// AÑADIR ESTA VALIDACIÓN:```

1. Crear 2 pipes: pipes[0] y pipes[1]

2. Fork 3 procesos (uno por comando)	if (!validate_syntax((*data)->tokens))

3. Proceso 1: stdout → pipe1

4. Proceso 2: stdin ← pipe1, stdout → pipe2	{**Integrar en checker.c:**

5. Proceso 3: stdin ← pipe2

6. Padre: waitpid() de todos		free_tokens((*data)->tokens);```

```

		(*data)->tokens = NULL;FUNCIÓN handle_heredoc(linea, posición, data):

**Archivos:**

- 📝 `src/executor/pipeline.c` (REEMPLAZAR pipelines.c vacío)		return (EXIT_FAILURE);    // Leer delimitador

- ✏️ `src/builtins/built.c` (modificar execute_command)

- ✏️ `src/minishell.h` (añadir prototipos)	}    delimiter = extraer_delimitador(linea, posición, debe_expandir)



<details>	check_and_exp(data);    

<summary>📄 Código completo: pipeline.c (Click para expandir)</summary>

	return (i);    // Leer contenido del heredoc

```c

#include "../minishell.h"}    contenido = read_heredoc(delimiter, data->env, debe_expandir)



// Cuenta comandos en la lista```    

static int	count_commands(t_cmd *cmd)

{    // Guardar en redirections

	int	count;

**Añadir a `src/minishell.h`:**    añadir_redireccion(data->cmd->redirections, HEREDOC, contenido)

	count = 0;

	while (cmd)```c    

	{

		count++;//--SYNTAX VALIDATOR--    LIBERAR delimiter

		cmd = cmd->next;

	}int				validate_syntax(t_token *tokens);    RETORNAR nueva_posición

	return (count);

}``````



// Crea array de pipes

static int	**create_pipes(int num_cmds)

{------

	int	**pipes;

	int	i;



	if (num_cmds <= 1)### 2. 🔴 PIPES - LISTA ENLAZADA DE COMANDOS## 📌 TAREAS SECUNDARIAS (Hacer después)

		return (NULL);

	pipes = malloc(sizeof(int *) * (num_cmds - 1));**Prioridad: CRÍTICA** (30% de la evaluación)  

	if (!pipes)

		return (NULL);**Tiempo estimado: 4-6 horas**  ### 4. 🟢 WILDCARDS (si es necesario para el subject)

	i = 0;

	while (i < num_cmds - 1)**Archivos:****Prioridad: MEDIA (Bonus)**  

	{

		pipes[i] = malloc(sizeof(int) * 2);- `src/parser/lexer.c` (MODIFICAR check_and_exp)**Archivos a crear:**

		if (!pipes[i] || pipe(pipes[i]) == -1)

		{- `src/utils/cmd_utils.c` (AÑADIR funciones)- `src/expander/wildcard.c` (nuevo)

			while (--i >= 0)

				free(pipes[i]);- `src/minishell.h` (AÑADIR prototipos)

			free(pipes);

			return (NULL);**Casos:**

		}

		i++;#### Problema actual:```bash

	}

	return (pipes);```cls *.c           # Expandir a todos los archivos .c

}

// AHORA (INCORRECTO):echo test*       # Expandir test*

// Cierra todos los pipes

static void	close_all_pipes(int **pipes, int num_pipes)t_data->cmd = [un solo comando]cat file?.txt    # ? = un carácter cualquiera

{

	int	i;// Input: "ls | grep test | wc -l"```



	i = 0;// Resultado: Solo ejecuta "ls"

	while (i < num_pipes)

	{**Implementación:**

		close(pipes[i][0]);

		close(pipes[i][1]);// NECESITAS (CORRECTO):```

		i++;

	}t_data->cmd = [cmd1] -> [cmd2] -> [cmd3] -> NULL// Usar readdir() para listar archivos

}

// Input: "ls | grep test | wc -l"// Usar fnmatch() o implementar pattern matching

// Libera memoria de pipes

static void	free_pipes(int **pipes, int num_pipes)// Resultado: cmd1="ls", cmd2="grep test", cmd3="wc -l"

{

	int	i;```FUNCIÓN expand_wildcard(pattern):



	i = 0;    directorio = abrir_directorio(".")

	while (i < num_pipes)

	{#### Implementación:    matches = array_vacío

		free(pipes[i]);

		i++;    

	}

	free(pipes);**Modificar `src/parser/lexer.c` - función `check_and_exp()`:**    MIENTRAS haya entrada en directorio:

}

        SI nombre_archivo coincide con pattern:

// Configura stdin/stdout para cada hijo

static void	setup_pipes_for_child(int **pipes, int cmd_idx, int num_cmds)**IMPORTANTE:** La función será demasiado larga, refactorízala así:            añadir a matches

{

	// Si no es el primer comando, leer del pipe anterior    

	if (cmd_idx > 0)

		dup2(pipes[cmd_idx - 1][0], STDIN_FILENO);```c    cerrar_directorio

	

	// Si no es el último comando, escribir al siguiente pipe// Función auxiliar para inicializar comando desde token    RETORNAR matches

	if (cmd_idx < num_cmds - 1)

		dup2(pipes[cmd_idx][1], STDOUT_FILENO);static int	init_cmd_from_token(t_cmd *cmd, t_token **tmp)```

	

	// Cerrar todos los file descriptors{

	close_all_pipes(pipes, num_cmds - 1);

}	if (!(*tmp) || !(*tmp)->value)---



// Convierte t_env a char** para execve		return (EXIT_FAILURE);

static char	**env_list_to_array(t_env *env)

{	cmd->name = ft_strdup((*tmp)->value);### 5. 🟢 OPERADORES LÓGICOS (Bonus)

	t_env	*current;

	char	**envp;	if (create_cmd(cmd->name, cmd) == 0)**Prioridad: BAJA (Bonus)**  

	char	*temp;

	int		i;		return (EXIT_FAILURE);**Archivos a crear:**

	int		count;

	*tmp = (*tmp)->next;- `src/parser/logical_ops.c` (nuevo)

	count = 0;

	current = env;	return (EXIT_SUCCESS);

	while (current && ++count)

		current = current->next;}**Operadores:**

	envp = malloc(sizeof(char *) * (count + 1));

	if (!envp)```bash

		return (NULL);

	i = 0;// Función para procesar tokens de un comando (hasta pipe)cmd1 && cmd2     # Ejecutar cmd2 solo si cmd1 exitcode=0

	current = env;

	while (current)static void	process_cmd_tokens(t_cmd *cmd, t_token **tmp)cmd1 || cmd2     # Ejecutar cmd2 solo si cmd1 exitcode!=0

	{

		temp = ft_strjoin(current->key, "=");{```

		envp[i] = ft_strjoin(temp, current->value);

		free(temp);	while (*tmp && (*tmp)->type != PIPE)

		current = current->next;

		i++;	{**Implementación:**

	}

	envp[i] = NULL;		if ((*tmp)->value && ((*tmp)->type == WORD || (*tmp)->type == STRINGNecesitas detectar `&&` y `||` en el tokenizer y manejarlos en el executor.

	return (envp);

}				|| (*tmp)->type == ARGS || (*tmp)->type == SIMPLE_Q))



// Ejecuta comando en proceso hijo			add_cmd_arg(cmd, (*tmp)->value);---

static void	execute_in_child(t_cmd *cmd, t_data *data, int **pipes, int idx)

{		*tmp = (*tmp)->next;

	char	*cmd_path;

	char	**envp;	}### 6. 🟢 SUBSHELLS con paréntesis (Bonus avanzado)

	int		num_cmds;

}**Prioridad: MUY BAJA**

	num_cmds = count_commands(data->cmd);

	setup_pipes_for_child(pipes, idx, num_cmds);```bash

	

	// Aplicar redirecciones si las hay// Función para crear siguiente comando en la lista(cd /tmp && ls)  # Ejecutar en proceso hijo sin afectar al padre

	if (cmd->redirections)

		apply_redirections(cmd);static int	create_next_cmd(t_cmd **current, t_token **tmp)```

	

	// Si es builtin{

	if (cmd->builtin_id != BUILTIN_NONE)

	{	if (!*tmp || (*tmp)->type != PIPE)---

		t_cmd *original = data->cmd;

		data->cmd = cmd;		return (0);

		execute_builtin_by_id(data);

		data->cmd = original;	*tmp = (*tmp)->next; // Saltar pipe## 📊 CHECKLIST DE PROGRESO

		exit(data->exit_status);

	}	if (!*tmp)

	

	// Buscar comando externo		return (0);### Parser

	envp = env_list_to_array(data->env);

	cmd_path = find_command_path(cmd->name, envp);	(*current)->next = malloc(sizeof(t_cmd));- [x] Tokenización básica

	if (!cmd_path)

	{	if (!(*current)->next)- [x] Detección de comillas simples/dobles

		ft_printf("minishell: %s: command not found\n", cmd->name);

		free_string_array(envp);		return (-1);- [x] Expansión de variables `$VAR`

		exit(127);

	}	*current = (*current)->next;- [x] Expansión de `$?` y `$$`

	

	if (execve(cmd_path, cmd->args, envp) == -1)	init_cmd(*current);- [x] Detección de redirecciones `<`, `>`, `>>`, `<<`

	{

		perror("execve");	return (1);- [x] Detección de pipes `|`

		free(cmd_path);

		free_string_array(envp);}- [ ] **Validación de sintaxis completa**

		exit(126);

	}- [ ] **Pipes: crear lista enlazada de comandos**

}

static int	check_and_exp(t_data **data)- [ ] **Heredocs funcionales**

// Espera a todos los hijos

static void	wait_all_children(pid_t *pids, int num_cmds, t_data *data){- [ ] Wildcards `*`

{

	int	i;	t_token	*tmp;- [ ] Operadores lógicos `&&`, `||`

	int	status;

	t_cmd	*current_cmd;- [ ] Caracteres escapados `\`

	i = 0;

	while (i < num_cmds)	int		status;

	{

		waitpid(pids[i], &status, 0);### Executor

		if (i == num_cmds - 1)  // Solo el último cuenta

		{	if (!(*data)->tokens)- [x] Ejecutar comandos simples

			if (WIFEXITED(status))

				data->exit_status = WEXITSTATUS(status);		return (EXIT_FAILURE);- [x] Builtins básicos (echo, cd, pwd, env, exit, export, unset)

			else

				data->exit_status = 128 + WTERMSIG(status);	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);- [x] Redirecciones básicas `<`, `>`

		}

		i++;	tmp = (*data)->tokens;- [ ] **Redirecciones `>>` (append)**

	}

}	current_cmd = (*data)->cmd;- [ ] **Heredocs `<<`**



// Función principal	if (init_cmd_from_token(current_cmd, &tmp) == EXIT_FAILURE)- [ ] **Pipeline completo (múltiples comandos con pipes)**

int	execute_pipeline(t_data *data)

{		return (EXIT_FAILURE);- [ ] Manejo de señales (Ctrl+C, Ctrl+D, Ctrl+\)

	int		**pipes;

	pid_t	*pids;	while (tmp)

	t_cmd	*current;

	int		num_cmds;	{---

	int		i;

		process_cmd_tokens(current_cmd, &tmp);

	num_cmds = count_commands(data->cmd);

	if (num_cmds == 0)		status = create_next_cmd(&current_cmd, &tmp);## 🎯 PLAN DE ACCIÓN RECOMENDADO

		return (EXIT_FAILURE);

			if (status == -1)

	pipes = create_pipes(num_cmds);

	if (num_cmds > 1 && !pipes)			return (EXIT_FAILURE);### Semana 1:

		return (EXIT_FAILURE);

			if (status == 0)1. ✅ Implementar validación de sintaxis (1-2 días)

	pids = malloc(sizeof(pid_t) * num_cmds);

	if (!pids)			break;2. ✅ Testear validación exhaustivamente (1 día)

	{

		if (pipes)		if (init_cmd_from_token(current_cmd, &tmp) == EXIT_FAILURE)

			free_pipes(pipes, num_cmds - 1);

		return (EXIT_FAILURE);			return (EXIT_FAILURE);### Semana 2:

	}

		}3. ✅ Implementar pipes completos en parser (2 días)

	current = data->cmd;

	i = 0;	return (EXIT_SUCCESS);4. ✅ Implementar pipeline execution en executor (2-3 días)

	while (current)

	{}5. ✅ Testear pipes (1 día)

		pids[i] = fork();

		if (pids[i] == 0)```

		{

			execute_in_child(current, data, pipes, i);### Semana 3:

			exit(1);

		}**Añadir función de debug en `src/utils/cmd_utils.c`:**6. ✅ Implementar heredocs (2-3 días)

		else if (pids[i] < 0)

		{```c7. ✅ Testear heredocs (1 día)

			perror("fork");

			free(pids);// Función para imprimir lista de comandos (útil para debug)8. ✅ Fix bugs y memory leaks (1-2 días)

			if (pipes)

			{void	print_cmd_list(t_cmd *cmd)

				close_all_pipes(pipes, num_cmds - 1);

				free_pipes(pipes, num_cmds - 1);{### Semana 4 (Bonus opcional):

			}

			return (EXIT_FAILURE);	int	i;9. Wildcards

		}

		current = current->next;	int	cmd_num;10. Operadores lógicos

		i++;

	}11. Refinamientos finales

	

	if (pipes)	cmd_num = 1;

	{

		close_all_pipes(pipes, num_cmds - 1);	while (cmd)---

		free_pipes(pipes, num_cmds - 1);

	}	{

	wait_all_children(pids, num_cmds, data);

	free(pids);		ft_printf("\n🔹 CMD %d: %s\n", cmd_num, cmd->name);## 🧪 CASOS DE PRUEBA IMPORTANTES

	

	return (EXIT_SUCCESS);		if (cmd->args)

}

```		{```bash

</details>

			ft_printf("   Args: ");# Validación de sintaxis

<details>

<summary>✏️ Modificar execute_command() (Click para expandir)</summary>			i = 0;|



```c			while (cmd->args[i])ls |

int	execute_command(t_data *data)

{			{| ls

	int		num_cmds;

	t_cmd	*temp;				ft_printf("[%s] ", cmd->args[i]);ls | | cat



	if (!data || !data->cmd)				i++;ls >

		return (EXIT_FAILURE);

				}ls > > file

	// Contar comandos

	num_cmds = 0;			ft_printf("\n");cat <<

	temp = data->cmd;

	while (temp)		}cat << EOF

	{

		num_cmds++;		if (cmd->redirections)

		temp = temp->next;

	}			ft_printf("   Has redirections: YES\n");# Pipes

	

	// Si hay múltiples comandos o flag de pipe, usar pipeline		cmd = cmd->next;ls | cat

	if (num_cmds > 1 || data->pipe_flag == 1)

		return (execute_pipeline(data));		cmd_num++;ls -l | grep test | wc -l

	

	// Comando único	}cat file.txt | grep hello | wc -l

	if (data->cmd->builtin_id != BUILTIN_NONE)

		return (execute_builtin_by_id(data));}

	

	// Tu código actual para comandos externos...```# Heredocs

}

```cat << EOF

</details>

**Modificar `free_cmd()` para liberar toda la lista:**hello

**Añadir a minishell.h:**

```c```cworld

int		execute_pipeline(t_data *data);

char	**env_list_to_array(t_env *env);void	free_cmd(t_cmd *cmd)EOF

```

{

---

	t_cmd	*next;cat << 'EOF'

### 4️⃣ Heredocs Funcionales (3-4 horas) 🟡 ALTA

	int		i;$USER should not expand

**Concepto:**

```bashEOF

cat << EOF      # Leer líneas hasta "EOF"

hello           # Línea 1	while (cmd)

world           # Línea 2

EOF             # Fin (delimitador encontrado)	{# Redirecciones complejas



cat << "EOF"    # Con comillas → NO expandir variables		next = cmd->next; // Guardar siguiente antes de liberarcat < input.txt | grep test > output.txt

$USER           # Se imprime literal: $USER

EOF		if (cmd->name)ls > file1.txt > file2.txt  # Solo file2 debe tener contenido

```

			free(cmd->name);cat << EOF | grep hello

**Archivos:**

- 📝 `src/parser/heredoc.c` (CREAR)		if (cmd->args)hello world

- ✏️ `src/parser/checker.c` (MODIFICAR handle_redir)

- ✏️ `src/minishell.h` (AÑADIR prototipos)		{test



<details>			i = 0;EOF

<summary>📄 Código: heredoc.c (Click para expandir)</summary>

			while (cmd->args[i])

```c

#include "../minishell.h"			{# Combinaciones



// Verifica si tiene comillas				free(cmd->args[i]);echo "hello" | cat | cat | cat

static int	has_quotes(char *delimiter, char **clean_delimiter)

{				i++;ls | > output.txt  # Error de sintaxis

	int	len;

	int	has_q;			}```



	has_q = 0;			free(cmd->args);

	len = ft_strlen(delimiter);

	if ((delimiter[0] == '"' && delimiter[len - 1] == '"')		}---

		|| (delimiter[0] == '\'' && delimiter[len - 1] == '\''))

	{		if (cmd->redirections)

		has_q = 1;

		*clean_delimiter = ft_substr(delimiter, 1, len - 2);			free_redirs(cmd->redirections);## 📝 NOTAS IMPORTANTES

	}

	else		free(cmd);

		*clean_delimiter = ft_strdup(delimiter);

	return (has_q);		cmd = next; // Continuar con el siguiente1. **Memory leaks**: Usar valgrind constantemente

}

	}2. **Norminette**: Verificar que cumple la norma de 42

// Lee contenido hasta delimitador

static char	*read_heredoc_content(char *delimiter, int expand, t_data *data)}3. **Exit codes**: Guardar y retornar correctamente `$?`

{

	char	*line;```4. **Señales**: Manejar Ctrl+C, Ctrl+D, Ctrl+\ correctamente

	char	*content;

	char	*temp;5. **PATH**: Buscar ejecutables en las rutas del PATH

	char	*expanded;

---6. **Comillas**: Recordar que simples NO expanden, dobles SÍ expanden

	content = ft_strdup("");

	while (1)

	{

		line = readline("> ");### 3. 🔴 EXECUTOR DE PIPELINES---

		if (!line)

		{**Prioridad: CRÍTICA** (Necesario para que funcionen las pipes)  

			ft_printf("warning: here-document delimited by EOF\n");

			break;**Tiempo estimado: 6-8 horas**  ## 🔗 ARCHIVOS DE REFERENCIA

		}

		if (ft_strcmp2(line, delimiter) == 0)**Archivos:**

		{

			free(line);- `src/executor/pipeline.c` (REEMPLAZAR pipelines.c vacío)- `PIPES_IMPLEMENTATION.md` - Guía detallada de pipes

			break;

		}- `src/executor/test_executor.c` (MODIFICAR)- `valgrind.md` - Guía para detectar leaks

		if (expand)

			expanded = expand_string(line, data->env, data->exit_status);- `src/minishell.h` (AÑADIR prototipos)- `BUILTINS_TEST_REPORT.md` - Tests de builtins

		else

			expanded = ft_strdup(line);- `VARIABLE_EXPANSION.md` - Tests de expansión

		temp = ft_strjoin(content, expanded);

		free(content);#### Concepto básico de pipes:

		content = ft_strjoin(temp, "\n");

		free(temp);```---

		free(expanded);

		free(line);Comando: ls | grep test | wc -l

	}

	return (content);**Última actualización:** 25 de octubre de 2025  

}

Proceso:**Estado actual:** Parser básico funcional, faltan pipes, validación y heredocs  

// Crea archivo temporal

static char	*create_heredoc_file(char *content)[ls] --pipe1--> [grep test] --pipe2--> [wc -l] --> stdout**Progreso estimado:** 60% completado

{

	char		*filename;

	int			fd;Implementación:

	static int	heredoc_num = 0;1. Crear arrays de file descriptors: int pipes[n-1][2]

	char		*num_str;2. Para cada comando (excepto el último):

   - fork() crear proceso hijo

	num_str = ft_itoa(heredoc_num++);   - dup2() redirigir stdout al pipe

	filename = ft_strjoin("/tmp/.heredoc_", num_str);   - dup2() redirigir stdin desde pipe anterior

	free(num_str);   - execve() ejecutar comando

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);3. Proceso padre: waitpid() para todos los hijos

	if (fd == -1)```

	{

		free(filename);#### Implementación:

		return (NULL);

	}**Ver archivo adjunto PIPES_IMPLEMENTATION.md para código completo del executor**

	write(fd, content, ft_strlen(content));

	close(fd);Los puntos clave son:

	return (filename);1. `count_commands()` - Cuenta comandos en la lista

}2. `create_pipes()` - Crea array de pipes

3. `setup_pipes_for_child()` - Configura stdin/stdout para cada hijo

// Función principal4. `env_list_to_array()` - Convierte t_env a char** para execve

char	*process_heredoc(char *delimiter, t_data *data)5. `execute_in_child()` - Ejecuta comando en proceso hijo

{6. `wait_all_children()` - Espera a todos los procesos

	char	*clean_delim;7. `execute_pipeline()` - Función principal

	int		should_expand;

	char	*content;**Modificar `src/builtins/built.c` (o donde esté execute_command):**

	char	*filename;```c

int	execute_command(t_data *data)

	should_expand = !has_quotes(delimiter, &clean_delim);{

	content = read_heredoc_content(clean_delim, should_expand, data);	int		num_cmds;

	free(clean_delim);	t_cmd	*temp;

	filename = create_heredoc_file(content);

	free(content);	if (!data || !data->cmd)

	return (filename);		return (EXIT_FAILURE);

}	

```	// Contar cuántos comandos hay

</details>	num_cmds = 0;

	temp = data->cmd;

<details>	while (temp)

<summary>✏️ Modificar handle_redir() en checker.c (Click para expandir)</summary>	{

		num_cmds++;

```c		temp = temp->next;

int	handle_redir(char *line, int i, t_data **data, t_token_type type)	}

{	

	int		start;	// Si hay pipes, usar execute_pipeline

	int		len;	if (num_cmds > 1 || data->pipe_flag == 1)

	char	*file;		return (execute_pipeline(data));

	

	i++;	// Si es comando único, ejecutar normalmente

	while (is_space(line[i]) == EXIT_SUCCESS)	if (data->cmd->builtin_id != BUILTIN_NONE)

		i++;		return (execute_builtin_by_id(data));

	start = i;	

	len = 0;	// Comando externo único (tu código actual)

	while (line[i] && is_space(line[i]) == EXIT_FAILURE	// ... implementación actual ...

		&& is_symbols(line[i]) == EXIT_FAILURE)}

	{```

		len++;

		i++;**Añadir a `src/minishell.h`:**

	}```c

	if (len > 0)//--PIPELINE--

	{int				execute_pipeline(t_data *data);

		if (!(*data)->cmd)char			**env_list_to_array(t_env *env);

			return (i);```

		file = ft_substr(line, start, len);

		---

		// ⭐ AÑADIR: Procesar heredoc

		if (type == HEREDOC)### 4. 🟡 HEREDOCS FUNCIONALES

		{**Prioridad: ALTA** (Necesario para completar el proyecto)  

			char *temp_file = process_heredoc(file, *data);**Tiempo estimado: 3-4 horas**  

			free(file);**Archivos:**

			file = temp_file;- `src/parser/heredoc.c` (CREAR)

			type = REDIR_IN;  // Tratar como input normal- `src/parser/checker.c` (MODIFICAR)

		}- `src/minishell.h` (AÑADIR prototipos)

		

		add_redir(&((*data)->cmd->redirections), type, file);#### Problema actual:

	}```bash

	return (i);# Input:

}cat << EOF

```Hello

</details>World

EOF

**Añadir a minishell.h:**

```c# Ahora: Solo detectas el token HEREDOC con delimitador "EOF"

char	*process_heredoc(char *delimiter, t_data *data);# Necesitas: Leer líneas hasta encontrar "EOF" y guardar el contenido

``````



---#### Concepto de heredoc:

```

### 5️⃣ Aplicar Redirecciones (2-3 horas) 🟡 ALTA1. Usuario escribe: cat << EOF

2. Shell debe leer líneas hasta encontrar "EOF"

**Archivos:**3. Contenido se pasa al comando por stdin

- ✏️ `src/utils/redir_utils.c` (COMPLETAR apply_redirections)4. Si delimitador tiene comillas, no expandir variables

   cat << "EOF"  -> No expandir

<details>   cat << EOF    -> Expandir variables

<summary>✏️ Código: apply_redirections() (Click para expandir)</summary>```



```c#### Implementación resumida:

int	apply_redirections(t_cmd *cmd)

{```c

	t_redir	*current;// En heredoc.c

	int		fd;

// Verifica si el delimitador tiene comillas

	if (!cmd || !cmd->redirections)static int	has_quotes(char *delimiter, char **clean_delimiter);

		return (0);

	current = cmd->redirections;// Lee líneas hasta encontrar el delimitador

	while (current)static char	*read_heredoc_content(char *delimiter, int expand, t_data *data);

	{

		if (current->type == REDIR_IN)// Crea un archivo temporal y escribe el contenido del heredoc

		{static char	*create_heredoc_file(char *content);

			fd = open(current->file, O_RDONLY);

			if (fd == -1)// Función principal - procesa un heredoc

			{char	*process_heredoc(char *delimiter, t_data *data);

				ft_printf("minishell: %s: No such file or directory\n",```

					current->file);

				return (-1);**Modificar `src/parser/checker.c`:**

			}```c

			dup2(fd, STDIN_FILENO);int	handle_redir(char *line, int i, t_data **data, t_token_type type)

			close(fd);{

		}	// ... código existente ...

		else if (current->type == REDIR_OUT)	

		{	if (len > 0)

			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);	{

			if (fd == -1)		if (!(*data)->cmd)

			{			return (i);

				ft_printf("minishell: %s: Permission denied\n", current->file);		file = ft_substr(line, start, len);

				return (-1);		

			}		// Si es heredoc, procesarlo inmediatamente

			dup2(fd, STDOUT_FILENO);		if (type == HEREDOC)

			close(fd);		{

		}			char *temp_file = process_heredoc(file, *data);

		else if (current->type == REDIR_APPEND)			free(file);

		{			file = temp_file;

			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);			// Cambiar tipo a REDIR_IN para que se maneje como input

			if (fd == -1)			type = REDIR_IN;

			{		}

				ft_printf("minishell: %s: Permission denied\n", current->file);		

				return (-1);		add_redir(&((*data)->cmd->redirections), type, file);

			}	}

			dup2(fd, STDOUT_FILENO);	return (i);

			close(fd);}

		}```

		current = current->next;

	}**Añadir a `src/minishell.h`:**

	return (0);```c

}//--HEREDOC--

```char			*process_heredoc(char *delimiter, t_data *data);

</details>```



------



## 📝 RESUMEN DE ARCHIVOS### 5. 🟡 APLICACIÓN DE REDIRECCIONES

**Prioridad: ALTA**  

| Acción | Archivo | Descripción |**Tiempo estimado: 2-3 horas**  

|--------|---------|-------------|**Archivo:**

| ✨ CREAR | `src/parser/syntax_validator.c` | Validación de sintaxis |- `src/utils/redir_utils.c` (MODIFICAR/COMPLETAR)

| ✨ CREAR | `src/executor/pipeline.c` | Ejecución de pipes |

| ✨ CREAR | `src/parser/heredoc.c` | Lectura de heredocs |#### Implementación de `apply_redirections()`:

| ✏️ MODIFICAR | `src/parser/lexer.c` | Validación + lista de comandos |

| ✏️ MODIFICAR | `src/utils/cmd_utils.c` | free_cmd() recursivo |```c

| ✏️ MODIFICAR | `src/parser/checker.c` | Integrar heredocs |// En redir_utils.c

| ✏️ MODIFICAR | `src/utils/redir_utils.c` | Completar apply_redirections |

| ✏️ MODIFICAR | `src/builtins/built.c` | Usar execute_pipeline |int	apply_redirections(t_cmd *cmd)

| ✏️ MODIFICAR | `src/minishell.h` | Todos los prototipos |{

	t_redir	*current;

---	int		fd;



## 🧪 PLAN DE TESTING	if (!cmd || !cmd->redirections)

		return (0);

### Fase 1: Validación ✅	current = cmd->redirections;

```bash	while (current)

|                 # ❌ Error	{

ls |              # ❌ Error		if (current->type == REDIR_IN)

ls | | cat        # ❌ Error		{

ls >              # ❌ Error			fd = open(current->file, O_RDONLY);

cat <<            # ❌ Error			if (fd == -1)

```			{

				ft_printf("minishell: %s: No such file or directory\n",

### Fase 2: Pipes Simples ✅					current->file);

```bash				return (-1);

ls | wc -l			}

echo hello | cat			dup2(fd, STDIN_FILENO);

cat /etc/passwd | grep root			close(fd);

ls | grep test | wc -l		}

```		else if (current->type == REDIR_OUT)

		{

### Fase 3: Pipes + Redirecciones ✅			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

```bash			if (fd == -1)

ls | grep test > output.txt			{

cat < input.txt | wc -l > result.txt				ft_printf("minishell: %s: Permission denied\n", current->file);

```				return (-1);

			}

### Fase 4: Heredocs ✅			dup2(fd, STDOUT_FILENO);

```bash			close(fd);

cat << EOF		}

hello		else if (current->type == REDIR_APPEND)

world		{

EOF			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);

			if (fd == -1)

cat << EOF			{

User: $USER				ft_printf("minishell: %s: Permission denied\n", current->file);

EOF				return (-1);

			}

cat << "EOF"			dup2(fd, STDOUT_FILENO);

$USER no se expande			close(fd);

EOF		}

```		current = current->next;

	}

### Fase 5: Todo Junto ✅	return (0);

```bash}

cat << EOF | grep hello | wc -l > output.txt```

hello world

goodbye world---

hello bash

EOF## 📝 RESUMEN DE ARCHIVOS A CREAR/MODIFICAR

```

### CREAR (nuevos):

---1. ✅ `src/parser/syntax_validator.c` - Validación de sintaxis

2. ✅ `src/executor/pipeline.c` - Ejecución de pipes (reemplazar pipelines.c vacío)

## ⏱️ CRONOGRAMA3. ✅ `src/parser/heredoc.c` - Manejo de heredocs



| Día | Tarea | Horas | Acumulado |### MODIFICAR (existentes):

|-----|-------|-------|-----------|1. ✅ `src/parser/lexer.c` - Añadir validación + lista de comandos

| **1** | Validación de sintaxis | 2-3h | 2-3h |2. ✅ `src/utils/cmd_utils.c` - Modificar free_cmd para lista, añadir debug

| **1** | Parser de pipes (lista) | 4-6h | 6-9h |3. ✅ `src/parser/checker.c` - Integrar process_heredoc

| **2** | Executor de pipelines | 6-8h | 12-17h |4. ✅ `src/utils/redir_utils.c` - Completar apply_redirections

| **2** | Testing de pipes | 2h | 14-19h |5. ✅ `src/builtins/built.c` - Modificar execute_command para usar pipeline

| **3** | Heredocs | 3-4h | 17-23h |6. ✅ `src/minishell.h` - Añadir todos los nuevos prototipos

| **3** | Redirecciones completas | 2-3h | 19-26h |

| **4** | Testing exhaustivo | 2-4h | 21-30h |---



**Total: 21-30 horas (3-4 días intensos)**## 🧪 PLAN DE TESTING



---### Fase 1: Validación de sintaxis

```bash

## ✅ CHECKLIST FINAL# Probar errores:

|

Antes de entregar:ls |

ls | | cat

- [ ] Validación de sintaxis funcionals >

- [ ] Pipes con 2+ comandos funcionancat < >

- [ ] Redirecciones (<, >, >>) funcionancat <<

- [ ] Heredocs (<<) funcionan```

- [ ] Heredocs con comillas no expanden

- [ ] Pipes + redirecciones juntos### Fase 2: Pipes simples

- [ ] Builtins funcionan en pipes```bash

- [ ] Variables expanden correctamente# Probar 2 comandos:

- [ ] Comillas simples no expandenls | wc -l

- [ ] $? retorna exit code correctoecho hello | cat

- [ ] Sin memory leaks (valgrind)cat /etc/passwd | grep root

- [ ] Sin file descriptors abiertos

- [ ] Norminette OK# Probar 3+ comandos:

- [ ] Makefile compila sin warningsls | grep test | wc -l

- [ ] Signals (Ctrl+C, Ctrl+D, Ctrl+\\)echo "hello world" | tr ' ' '\n' | sort | uniq

```

---

### Fase 3: Pipes con redirecciones

## 🐛 PROBLEMAS COMUNES```bash

# Combinar pipes y redirecciones:

| Problema | Causa | Solución |ls | grep test > output.txt

|----------|-------|----------|cat < input.txt | grep word | wc -l > result.txt

| "Broken pipe" | FDs no cerrados | `close_all_pipes()` en padre e hijos |```

| Zombies | Sin waitpid() | Guardar PIDs y esperar todos |

| Heredoc no termina | Delimitador con espacios | `ft_strcmp2()` exacto |### Fase 4: Heredocs

| Variables no expanden | Falta expand_variables() | Llamarla ANTES de crear comandos |```bash

| Leaks en pipes | No liberar lista completa | Modificar `free_cmd()` recursivo |# Heredoc simple:

cat << EOF

---hello

world

## 📚 RECURSOSEOF



### Man Pages# Heredoc con variables:

```bashcat << EOF

man 2 pipe      # Crear pipesUser: $USER

man 2 fork      # Crear procesosHome: $HOME

man 2 dup2      # Duplicar file descriptorsEOF

man 2 execve    # Ejecutar programas

man 2 waitpid   # Esperar procesos hijos# Heredoc sin expansión (comillas):

```cat << "EOF"

$USER should not expand

### Conceptos ClaveEOF

- **File descriptors:** 0=stdin, 1=stdout, 2=stderr```

- **Pipe:** Comunicación unidireccional entre procesos

- **Fork:** Duplica el proceso actual### Fase 5: Combinaciones complejas

- **Execve:** Reemplaza el proceso con nuevo programa```bash

- **Dup2:** Redirige file descriptors# Todo junto:

cat << EOF | grep hello | wc -l > output.txt

---hello world

hello bash

## 🚀 BONUS (Solo si terminas todo)goodbye

EOF

### Wildcards (*) - 6-8 horas```

Expandir `*.c` a lista de archivos con `readdir()`

---

### Operadores lógicos (&&, ||) - 4-6 horas

- `ls && echo success` - ejecutar si exitoso## ⏱️ ESTIMACIÓN TOTAL

- `ls || echo failed` - ejecutar si falla- **Validación de sintaxis:** 2-3 horas

- **Pipes (parser):** 4-6 horas  

---- **Pipes (executor):** 6-8 horas

- **Heredocs:** 3-4 horas

**¡Buena suerte! 🎯**- **Redirecciones:** 2-3 horas

- **Testing y debugging:** 4-6 horas

**TOTAL: 21-30 horas** (3-4 días de trabajo intenso)

---

## 🎯 ORDEN RECOMENDADO DE IMPLEMENTACIÓN

1. **DÍA 1:** Validación de sintaxis (2-3h) + Parser de pipes (4-6h)
2. **DÍA 2:** Executor de pipelines (6-8h) + Testing de pipes simples
3. **DÍA 3:** Heredocs (3-4h) + Redirecciones completas (2-3h)
4. **DÍA 4:** Testing exhaustivo, debugging y casos edge

---

## 🚫 BONUS (Hacer solo SI terminas todo lo anterior)

### Wildcards (*)
- Expansión de `*.c` a lista de archivos
- Implementar con `readdir()` y pattern matching
- **Tiempo estimado:** 6-8 horas

### Operadores lógicos (&& y ||)
- `ls && echo success` - ejecutar segundo si primero exitoso
- `ls || echo failed` - ejecutar segundo si primero falla
- Requiere evaluar exit status entre comandos
- **Tiempo estimado:** 4-6 horas

---

## 📚 RECURSOS ÚTILES

### Man pages importantes:
```bash
man 2 pipe    # Crear pipes
man 2 fork    # Crear procesos
man 2 dup2    # Duplicar file descriptors
man 2 execve  # Ejecutar programas
man 2 waitpid # Esperar a procesos hijos
man 3 readline # Leer input del usuario
```

### Conceptos clave:
- **File descriptors:** 0=stdin, 1=stdout, 2=stderr
- **Pipes:** Comunicación unidireccional entre procesos
- **Fork:** Duplica el proceso actual
- **Execve:** Reemplaza el proceso con nuevo programa
- **Dup2:** Redirige file descriptors

### Bash reference:
```bash
# Para probar comportamiento correcto:
bash -x  # Modo debug de bash
```

---

## ✅ CHECKLIST FINAL

Antes de entregar el proyecto, verifica:

- [ ] Validación de sintaxis funciona para todos los casos
- [ ] Pipes funcionan con 2, 3, 4+ comandos
- [ ] Redirecciones funcionan (<, >, >>)
- [ ] Heredocs funcionan (<<)
- [ ] Heredocs con comillas no expanden variables
- [ ] Pipes + redirecciones funcionan juntos
- [ ] Builtins funcionan en pipes
- [ ] Variables se expanden correctamente
- [ ] Comillas simples no expanden
- [ ] $? retorna exit status correcto
- [ ] Sin leaks de memoria (valgrind)
- [ ] Sin file descriptors abiertos
- [ ] Norminette pasa en todos los archivos
- [ ] Makefile compila sin warnings
- [ ] Signals manejados correctamente (Ctrl+C, Ctrl+D, Ctrl+\\)

---

## 🐛 PROBLEMAS COMUNES Y SOLUCIONES

### "Broken pipe" error:
- **Causa:** No cerrar file descriptors de pipes
- **Solución:** `close_all_pipes()` en padre E hijos

### Procesos zombies:
- **Causa:** No hacer waitpid() de todos los hijos
- **Solución:** Guardar PIDs y esperar a todos

### Heredoc no termina:
- **Causa:** Delimitador con espacios extra
- **Solución:** `ft_strcmp2(line, delimiter)` exacto

### Variables no expanden:
- **Causa:** Olvidar llamar `expand_variables()`
- **Solución:** Llamarla ANTES de crear comandos

### Leaks de memoria en pipes:
- **Causa:** No liberar todos los nodos de t_cmd
- **Solución:** Modificar `free_cmd()` para recorrer toda la lista

---

**¡IMPORTANTE!** Este documento refleja el estado actual de tu parser. Sigue el orden recomendado y ve marcando cada tarea como completada. ¡Buena suerte! 🚀

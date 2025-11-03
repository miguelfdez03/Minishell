# 🔧 IMPLEMENTACIÓN DE PIPES - PARSING COMPLETO# 🔧 IMPLEMENTACIÓN DE PIPES - PARSING COMPLETO# 🔧 IMPLEMENTACIÓN DE PIPES - PARSING COMPLETO



> **Estado actual:** ✅ Parsing 100% implementado | ❌ Ejecución pendiente  

> **Fecha:** 31 de octubre de 2025  

> **Branch:** luis-parse> **Estado actual:** ✅ Parsing 100% implementado | ❌ Ejecución pendiente  > **Estado actual:** ✅ Parsing 100% implementado | ❌ Ejecución pendiente  



---> **Fecha:** 31 de octubre de 2025  > **Fecha:** 31 de octubre de 2025  



## 📋 ÍNDICE> **Branch:** luis-parse> **Branch:** luis-parse



1. [¿Qué se ha implementado?](#1-qué-se-ha-implementado)

2. [¿Cómo funciona el parsing de pipes?](#2-cómo-funciona-el-parsing-de-pipes)

3. [Explicación del código implementado](#3-explicación-del-código-implementado)---## 📚 Tabla de contenidos

4. [Ejemplo paso a paso](#4-ejemplo-paso-a-paso)

5. [¿Qué falta por hacer?](#5-qué-falta-por-hacer)1. [Conceptos básicos](#conceptos-básicos)



---## 📋 ÍNDICE2. [Modificaciones en el Parser](#modificaciones-en-el-parser)



## 1. ¿QUÉ SE HA IMPLEMENTADO?3. [Implementación del Executor](#implementación-del-executor)



### ✅ COMPLETADO: Sistema de parsing de pipes1. [¿Qué se ha implementado?](#1-qué-se-ha-implementado)4. [Funciones auxiliares](#funciones-auxiliares)



Cuando el usuario escribe:2. [¿Cómo funciona el parsing de pipes?](#2-cómo-funciona-el-parsing-de-pipes)5. [Casos especiales](#casos-especiales)

```bash

echo hello world | grep world | cat3. [Explicación del código implementado](#3-explicación-del-código-implementado)6. [Testing](#testing)

```

4. [Ejemplo paso a paso](#4-ejemplo-paso-a-paso)

El parser ahora **crea una lista enlazada** de 3 comandos en memoria:

5. [¿Qué falta por hacer?](#5-qué-falta-por-hacer)---

```

[CMD1: echo] → [CMD2: grep] → [CMD3: cat] → NULL

    ↓              ↓               ↓

[hello, world]   [world]        (sin args)---## 1. Conceptos básicos

```



### ❌ PENDIENTE: Ejecución de pipes

## 1. ¿QUÉ SE HA IMPLEMENTADO?### ¿Qué es una pipe?

Actualmente **NO se ejecutan** los comandos en procesos separados con pipes reales del sistema operativo. Solo se crea la estructura de datos.

Una pipe (`|`) conecta la salida de un comando con la entrada del siguiente:

---

### ✅ COMPLETADO: Sistema de parsing de pipes```bash

## 2. ¿CÓMO FUNCIONA EL PARSING DE PIPES?

ls -l | grep .c | wc -l

### 🎯 El problema que resolvemos

Cuando el usuario escribe:  ↓       ↓         ↓

**ANTES:**

```bash```bash cmd1    cmd2      cmd3

Input: "ls -la | grep test"

       echo hello world | grep world | cat```

Parser creaba:

data->cmd = [cmd: "ls", args: ["-la"]]  ❌```

            

Solo guardaba el PRIMER comando, el resto se perdía### Cómo funciona internamente:

```

El parser ahora **crea una lista enlazada** de 3 comandos en memoria:- Cada comando se ejecuta en un **proceso hijo** separado

**AHORA:**

```bash- Se crean **N-1 pipes** para N comandos

Input: "ls -la | grep test"

```- La pipe tiene dos extremos: `read_end [0]` y `write_end [1]`

Parser crea:

data->cmd = [cmd1: "ls"] → [cmd2: "grep"] → NULL  ✅[CMD1: echo] → [CMD2: grep] → [CMD3: cat] → NULL- Se usa `dup2()` para redirigir stdin/stdout a las pipes

              ↓               ↓

            [-la]          [test]    ↓              ↓               ↓



Lista enlazada con TODOS los comandos[hello, world]   [world]        (sin args)---

```

```

---

## 2. Modificaciones en el Parser

## 3. EXPLICACIÓN DEL CÓDIGO IMPLEMENTADO

### ❌ PENDIENTE: Ejecución de pipes

### 📂 Archivos modificados

### 📊 Estado actual de tu estructura:

1. **`src/parser/checker.c`** - Tokenizar el símbolo `|`

2. **`src/parser/lexer.c`** - Procesar el primer comando y detectar pipesActualmente **NO se ejecutan** los comandos en procesos separados con pipes reales del sistema operativo. Solo se crea la estructura de datos.```c

3. **`src/pipes/pipes.c`** - Crear la lista enlazada (NUEVO ARCHIVO)

4. **`src/utils/cmd_utils.c`** - Liberar toda la lista de comandostypedef struct s_cmd {



------    char *name;              // Nombre del comando



### 📄 **1. checker.c - Crear token PIPE**    char **args;             // Array de argumentos



**Ubicación:** Función `check_redir()` línea 51-55## 2. ¿CÓMO FUNCIONA EL PARSING DE PIPES?    t_builtin_type builtin_id;



**¿Qué hace?**      t_redir *redirections;   // Lista de redirecciones

Cuando el lexer encuentra el símbolo `|`, crea un token de tipo PIPE.

### 🎯 El problema que resolvemos    struct s_cmd *next;      // ✅ Ya tienes esto para enlazar!

```c

else if (line[i] == '|')} t_cmd;

{

    add_to_token(&((*data)->tokens), PIPE, "|");**ANTES:**```

    return (1);

}```bash

```

Input: "ls -la | grep test"### 🎯 El Problema Actual

**Ejemplo:**

```bash       

Input: "echo hello | cat"

                   ↑Parser creaba:**ANTES (lo que tienes ahora):**

                   Aquí se crea token PIPE

data->cmd = [cmd: "ls", args: ["-la"]]  ❌```bash

Tokens resultantes:

WORD("echo") → WORD("hello") → PIPE("|") → WORD("cat") → NULL            Input: "ls -la | grep test | wc -l"

```

Solo guardaba el PRIMER comando, el resto se perdía

---

```Tu parser crea:

### 📄 **2. lexer.c - Procesar primer comando y detectar pipes**

data->cmd = [cmd: "ls", args: ["-la"]]  ❌ Solo un comando

**Ubicación:** Función `check_and_exp()` líneas 40-58

**AHORA:**

**¿Qué hace?**  

1. Crea el **primer comando** de la lista```bashResultado: Solo ejecuta "ls -la", ignora el resto

2. Procesa sus argumentos **hasta encontrar un PIPE**

3. Si hay pipe, llama a `process_pipes()` para crear el restoInput: "ls -la | grep test"```



```c

static int	check_and_exp(t_data **data)

{Parser crea:**DESPUÉS (lo que necesitas):**

	t_token	*tmp;

data->cmd = [cmd1: "ls"] → [cmd2: "grep"] → NULL  ✅```bash

	if (!(*data)->tokens)

		return (EXIT_FAILURE);              ↓               ↓Input: "ls -la | grep test | wc -l"

	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);

	tmp = (*data)->tokens;            [-la]          [test]

	

	// 1️⃣ Crear primer comandoParser debe crear:

	if (init_first_cmd(data, &tmp) == EXIT_FAILURE)

		return (EXIT_FAILURE);Lista enlazada con TODOS los comandosdata->cmd = [cmd1: "ls -la"] → [cmd2: "grep test"] → [cmd3: "wc -l"] → NULL  ✅

	

	// 2️⃣ Procesar argumentos del primer comando hasta PIPE```

	process_first_cmd_args(&tmp, data);

	Resultado: Cadena de 3 comandos conectados

	// 3️⃣ Si hay pipe, crear resto de comandos

	if (tmp && tmp->type == PIPE)---```

	{

		if (process_pipes(*data) == -1)

			return (EXIT_FAILURE);

	}## 3. EXPLICACIÓN DEL CÓDIGO IMPLEMENTADO---

	

	return (EXIT_SUCCESS);

}

```### 📂 Archivos modificados### 🔧 Solución: Crear Lista Enlazada en el Parser



**Ejemplo:**

```bash

Input: "echo hello | cat"1. **`src/parser/checker.c`** - Tokenizar el símbolo `|`#### Opción 1: Función dedicada en `src/pipes/pipes.c`



1. init_first_cmd() crea:2. **`src/parser/lexer.c`** - Procesar el primer comando y detectar pipes

   cmd->name = "echo"

   3. **`src/pipes/pipes.c`** - Crear la lista enlazada (NUEVO ARCHIVO)Esta es la forma más limpia y modular:

2. process_first_cmd_args() añade:

   cmd->args = ["hello"]4. **`src/utils/cmd_utils.c`** - Liberar toda la lista de comandos

   

3. Detecta PIPE → llama a process_pipes()```c

```

---/* ************************************************************************** */

---

/*   pipes.c - Funciones para crear lista enlazada de comandos              */

### 📄 **3. pipes.c - ARCHIVO NUEVO - Crear lista enlazada**

### 📄 **1. checker.c - Crear token PIPE**/* ************************************************************************** */

Este archivo contiene 4 funciones que construyen la lista enlazada:



#### 🔹 Función 1: `init_next_cmd()` (líneas 15-38)

**Ubicación:** Función `check_redir()` línea 51-55#include "../minishell.h"

**¿Qué hace?**  

Crea y enlaza un **nuevo comando** después del pipe.



```c**¿Qué hace?**  // Función para crear el siguiente comando después de un pipe

static int	init_next_cmd(t_cmd *current_cmd, t_token **tokens)

{Cuando el lexer encuentra el símbolo `|`, crea un token de tipo PIPE.int	handle_pipe(t_cmd *current_cmd, t_token **tokens)

	// 1. Reservar memoria para el siguiente comando

	current_cmd->next = malloc(sizeof(t_cmd));{

	if (!current_cmd->next)

		return (-1);```c	t_token	*tmp;

	

	// 2. Inicializar sus campos a NULLelse if (line[i] == '|')

	init_cmd(current_cmd->next);

	{	if (!current_cmd || !tokens || !*tokens)

	// 3. Obtener el nombre del comando desde el token

	tmp = *tokens;    add_to_token(&((*data)->tokens), PIPE, "|");		return (-1);

	if (tmp && tmp->value && (tmp->type == WORD || tmp->type == STRING))

	{    return (1);	

		current_cmd->next->name = ft_strdup(tmp->value);

		current_cmd->next->builtin_id = identify_builtin(tmp->value);}	// Saltar el token PIPE

		*tokens = (*tokens)->next;

	}```	*tokens = (*tokens)->next;

	

	return (0);	

}

```**Ejemplo:**	// Si no hay nada después del pipe, error de sintaxis



**Ejemplo:**```bash	if (!*tokens)

```bash

Estado: [CMD1: echo] → NULLInput: "echo hello | cat"	{

Token actual: "grep"

                   ↑		ft_printf("syntax error near unexpected token `|'\n");

Después de init_next_cmd():

[CMD1: echo] → [CMD2: grep] → NULL                   Aquí se crea token PIPE		return (-1);

                   ↑

              nombre copiado	}

```

Tokens resultantes:	

---

WORD("echo") → WORD("hello") → PIPE("|") → WORD("cat") → NULL	// Crear el siguiente comando

#### 🔹 Función 2: `process_cmd_args()` (líneas 40-53)

```	current_cmd->next = malloc(sizeof(t_cmd));

**¿Qué hace?**  

Añade **argumentos** al comando hasta encontrar otro PIPE.	if (!current_cmd->next)



```c---		return (-1);

static void	process_cmd_args(t_cmd *cmd, t_token **tokens)

{	

	tmp = *tokens;

	### 📄 **2. lexer.c - Procesar primer comando y detectar pipes**	// Inicializar el nuevo comando

	// Recorrer tokens mientras NO sea PIPE

	while (tmp && tmp->type != PIPE)	init_cmd(current_cmd->next);

	{

		// Si es un argumento válido, añadirlo**Ubicación:** Función `check_and_exp()` líneas 40-58	

		if (tmp->value && (tmp->type == WORD || tmp->type == STRING

				|| tmp->type == ARGS || tmp->type == SIMPLE_Q))	// Obtener el nombre del siguiente comando

			add_cmd_arg(cmd, tmp->value);

		**¿Qué hace?**  	tmp = *tokens;

		tmp = tmp->next;

		*tokens = tmp;1. Crea el **primer comando** de la lista	if (tmp && tmp->value && (tmp->type == WORD || tmp->type == STRING))

	}

}2. Procesa sus argumentos **hasta encontrar un PIPE**	{

```

3. Si hay pipe, llama a `process_pipes()` para crear el resto		current_cmd->next->name = ft_strdup(tmp->value);

**Ejemplo:**

```bash		if (create_cmd(current_cmd->next->name, current_cmd->next) == 0)

Tokens: WORD("grep") → WORD("test") → WORD("-i") → PIPE("|") → ...

                         ↑              ↑```c		{

                    arg 1          arg 2

static int	check_and_exp(t_data **data)			free(current_cmd->next);

Después de process_cmd_args():

cmd->args = ["test", "-i", NULL]{			current_cmd->next = NULL;

```

	t_token	*tmp;			return (-1);

---

		}

#### 🔹 Función 3: `handle_pipe()` (líneas 55-70)

	if (!(*data)->tokens)		*tokens = (*tokens)->next;

**¿Qué hace?**  

Maneja **un único pipe**: crea el comando siguiente y procesa sus argumentos.		return (EXIT_FAILURE);	}



```c	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);	

int	handle_pipe(t_cmd *current_cmd, t_token **tokens)

{	tmp = (*data)->tokens;	// Procesar argumentos del nuevo comando hasta encontrar otro PIPE

	// 1. Saltar el token PIPE

	*tokens = (*tokens)->next;		tmp = *tokens;

	

	// 2. Validar: no puede terminar con pipe	// 1️⃣ Crear primer comando	while (tmp && tmp->type != PIPE)

	if (!*tokens)

	{	if (init_first_cmd(data, &tmp) == EXIT_FAILURE)	{

		ft_printf("syntax error near unexpected token `|'\n");

		return (-1);		return (EXIT_FAILURE);		if (tmp->value && (tmp->type == WORD || tmp->type == STRING

	}

						|| tmp->type == ARGS || tmp->type == SIMPLE_Q))

	// 3. Crear el siguiente comando

	if (init_next_cmd(current_cmd, tokens) == -1)	// 2️⃣ Procesar argumentos del primer comando hasta PIPE		{

		return (-1);

		process_first_cmd_args(&tmp, data);			add_cmd_arg(current_cmd->next, tmp->value);

	// 4. Procesar sus argumentos

	process_cmd_args(current_cmd->next, tokens);			}

	

	return (0);	// 3️⃣ Si hay pipe, crear resto de comandos		tmp = tmp->next;

}

```	if (tmp && tmp->type == PIPE)		*tokens = tmp;



**Ejemplo:**	{	}

```bash

Estado: [CMD1: echo hello] → NULL		if (process_pipes(*data) == -1)	

Tokens: PIPE("|") → WORD("grep") → WORD("world") → NULL

         ↑			return (EXIT_FAILURE);	return (0);

     actual

	}}

handle_pipe() hace:

1. Salta el PIPE	

2. Crea CMD2 con nombre "grep"

3. Añade argumento "world"	return (EXIT_SUCCESS);// Función para procesar toda la cadena de pipes



Resultado: [CMD1: echo hello] → [CMD2: grep world] → NULL}int	process_pipes(t_data *data)

```

```{

---

	t_token	*tmp;

#### 🔹 Función 4: `process_pipes()` (líneas 72-94)

**Ejemplo:**	t_cmd	*current_cmd;

**¿Qué hace?**  

Recorre **todos** los tokens y procesa **cada pipe** encontrado.```bash



```cInput: "echo hello | cat"	if (!data || !data->cmd || !data->tokens)

int	process_pipes(t_data *data)

{		return (-1);

	t_token	*tmp;

	t_cmd	*current_cmd;1. init_first_cmd() crea:	



	tmp = data->tokens;   cmd->name = "echo"	tmp = data->tokens;

	current_cmd = data->cmd;

	   	current_cmd = data->cmd;

	// Recorrer toda la lista de tokens

	while (tmp)2. process_first_cmd_args() añade:	

	{

		// Si encontramos un PIPE   cmd->args = ["hello"]	// Recorrer todos los tokens buscando pipes

		if (tmp->type == PIPE)

		{   	while (tmp)

			// Procesar ese pipe

			if (handle_pipe(current_cmd, &tmp) == -1)3. Detecta PIPE → llama a process_pipes()	{

				return (-1);

			```		if (tmp->type == PIPE)

			// Avanzar al comando recién creado

			current_cmd = current_cmd->next;		{

		}

		else---			if (handle_pipe(current_cmd, &tmp) == -1)

		{

			tmp = tmp->next;				return (-1);

		}

	}### 📄 **3. pipes.c - ARCHIVO NUEVO - Crear lista enlazada**			

	

	return (0);			// Avanzar al siguiente comando creado

}

```Este archivo contiene 4 funciones que construyen la lista enlazada:			current_cmd = current_cmd->next;



**Ejemplo:**		}

```bash

Tokens: WORD("echo") → WORD("hi") → PIPE → WORD("cat") → PIPE → WORD("wc") → NULL#### 🔹 Función 1: `init_next_cmd()` (líneas 15-38)		else

                                      ↑                     ↑

                                  pipe 1               pipe 2		{



process_pipes() recorre y encuentra:**¿Qué hace?**  			tmp = tmp->next;

- Pipe 1 → crea CMD2 (cat)

- Pipe 2 → crea CMD3 (wc)Crea y enlaza un **nuevo comando** después del pipe.		}



Resultado: [CMD1] → [CMD2] → [CMD3] → NULL	}

```

```c	

---

static int	init_next_cmd(t_cmd *current_cmd, t_token **tokens)	return (0);

### 📄 **4. cmd_utils.c - Liberar TODA la lista**

{}

**Ubicación:** Función `free_cmd()` líneas 134-148

	// 1. Reservar memoria para el siguiente comando```

**¿Qué hace?**  

Libera **recursivamente** toda la lista enlazada, no solo un comando.	current_cmd->next = malloc(sizeof(t_cmd));



**ANTES (solo liberaba 1 comando):**	if (!current_cmd->next)#### Cómo usar estas funciones en tu `lexer.c`:

```c

void	free_cmd(t_cmd *cmd)		return (-1);

{

	free(cmd->name);	```c

	free(cmd->args);

	free(cmd);  // ❌ cmd->next se pierde (memory leak)	// 2. Inicializar sus campos a NULL// En tu función check_and_exp()

}

```	init_cmd(current_cmd->next);static int	check_and_exp(t_data **data)



**AHORA (libera toda la lista):**	{

```c

void	free_cmd(t_cmd *cmd)	// 3. Obtener el nombre del comando desde el token	t_token	*tmp;

{

	t_cmd	*next;	tmp = *tokens;



	while (cmd)  // ⭐ Bucle para recorrer toda la lista	if (tmp && tmp->value && (tmp->type == WORD || tmp->type == STRING))	if (!(*data)->tokens)

	{

		next = cmd->next;  // ⭐ Guardar siguiente ANTES de liberar	{		return (EXIT_FAILURE);

		

		// Liberar contenido del comando actual		current_cmd->next->name = ft_strdup(tmp->value);	

		free_cmd_content(cmd);

				current_cmd->next->builtin_id = identify_builtin(tmp->value);	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);

		// Liberar el comando

		free(cmd);		*tokens = (*tokens)->next;	tmp = (*data)->tokens;

		

		// Avanzar al siguiente	}	

		cmd = next;

	}		// 1. Crear primer comando (como ya lo haces)

}

```	return (0);	if (tmp && tmp->value)



**Ejemplo:**}	{

```bash

Lista: [CMD1] → [CMD2] → [CMD3] → NULL```		if ((*data)->cmd->name)



Iteración 1: Libera CMD1, avanza a CMD2			free((*data)->cmd->name);

Iteración 2: Libera CMD2, avanza a CMD3

Iteración 3: Libera CMD3, avanza a NULL**Ejemplo:**		(*data)->cmd->name = ft_strdup(tmp->value);

Fin: Todo liberado ✅

``````bash		if (create_cmd((*data)->cmd->name, (*data)->cmd) == 0)



---Estado: [CMD1: echo] → NULL			return (EXIT_FAILURE);



## 4. EJEMPLO PASO A PASOToken actual: "grep"		tmp = tmp->next;



Vamos a ver **todo el proceso** con un ejemplo real:	}



### 📝 Input del usuarioDespués de init_next_cmd():	

```bash

echo hello world | grep world | cat[CMD1: echo] → [CMD2: grep] → NULL	// 2. Procesar argumentos del primer comando hasta el pipe

```

                   ↑	while (tmp && tmp->type != PIPE)

---

              nombre copiado	{

### **PASO 1: Tokenización** (lexer.c)

```		if (tmp->value && (tmp->type == WORD || tmp->type == STRING

El lexer recorre el string y crea tokens:

				|| tmp->type == ARGS || tmp->type == SIMPLE_Q))

```

Tokens creados:---			add_cmd_arg((*data)->cmd, tmp->value);

┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────┐  ┌──────────┐  ┌──────────┐  ┌──────┐  ┌──────┐

│ WORD     │→ │ WORD     │→ │ WORD     │→ │ PIPE │→ │ WORD     │→ │ WORD     │→ │ PIPE │→ │ WORD │		tmp = tmp->next;

│ "echo"   │  │ "hello"  │  │ "world"  │  │ "|"  │  │ "grep"   │  │ "world"  │  │ "|"  │  │ "cat"│

└──────────┘  └──────────┘  └──────────┘  └──────┘  └──────────┘  └──────────┘  └──────┘  └──────┘#### 🔹 Función 2: `process_cmd_args()` (líneas 40-53)	}

```

	

---

**¿Qué hace?**  	// 3. ⭐ NUEVO: Si hay pipes, procesar el resto de comandos

### **PASO 2: Crear primer comando** (lexer.c → check_and_exp)

Añade **argumentos** al comando hasta encontrar otro PIPE.	if (tmp && tmp->type == PIPE)

La función `init_first_cmd()` crea el primer comando:

	{

```c

// Token actual: WORD("echo")```c		if (process_pipes(*data) == -1)

cmd->name = ft_strdup("echo");  // Copiar nombre

cmd->builtin_id = BUILTIN_ECHO;  // Identificar que es builtinstatic void	process_cmd_args(t_cmd *cmd, t_token **tokens)			return (EXIT_FAILURE);

```

{	}

Estado:

```	tmp = *tokens;	

┌────────────────┐

│ CMD1           │		return (EXIT_SUCCESS);

│ name: "echo"   │

│ args: NULL     │	// Recorrer tokens mientras NO sea PIPE}

│ next: NULL     │

└────────────────┘	while (tmp && tmp->type != PIPE)```



Token actual: WORD("hello")	{

```

		// Si es un argumento válido, añadirlo---

---

		if (tmp->value && (tmp->type == WORD || tmp->type == STRING

### **PASO 3: Añadir argumentos del primer comando** (process_first_cmd_args)

				|| tmp->type == ARGS || tmp->type == SIMPLE_Q))#### Opción 2: Función integrada que construye toda la lista

Recorre tokens mientras NO sea PIPE:

			add_cmd_arg(cmd, tmp->value);

```c

// Token: WORD("hello")		Si prefieres una función que procese todo desde cero:

add_cmd_arg(cmd, "hello");  // Añadir arg[0]

		tmp = tmp->next;

// Token: WORD("world")

add_cmd_arg(cmd, "world");  // Añadir arg[1]		*tokens = tmp;```c



// Token: PIPE("|") → STOP	}// Versión alternativa: construir lista completa en un recorrido

```

}int	build_cmd_list(t_data *data)

Estado:

``````{

┌──────────────────────┐

│ CMD1                 │	t_token	*tmp;

│ name: "echo"         │

│ args: ["hello",      │**Ejemplo:**	t_cmd	*current_cmd;

│        "world",      │

│        NULL]         │```bash

│ next: NULL           │

└──────────────────────┘Tokens: WORD("grep") → WORD("test") → WORD("-i") → PIPE("|") → ...	tmp = data->tokens;



Token actual: PIPE("|")                         ↑              ↑	current_cmd = data->cmd;

```

                    arg 1          arg 2	

---

	while (tmp)

### **PASO 4: Detectar pipe y llamar process_pipes()** (check_and_exp)

Después de process_cmd_args():	{

```c

if (tmp && tmp->type == PIPE)  // ✅ Es PIPEcmd->args = ["test", "-i", NULL]		// Si encontramos un PIPE, crear siguiente comando

{

	process_pipes(*data);  // Llamar función de pipes.c```		if (tmp->type == PIPE)

}

```		{



------			tmp = tmp->next; // Saltar el pipe



### **PASO 5: Procesar primer pipe** (process_pipes → handle_pipe)			



```c#### 🔹 Función 3: `handle_pipe()` (líneas 55-70)			// Validar que haya algo después del pipe

// 1. Saltar token PIPE

tokens = tokens->next;  // Ahora apunta a WORD("grep")			if (!tmp)



// 2. Crear CMD2**¿Qué hace?**  			{

init_next_cmd(CMD1, &tokens);

// Resultado: CMD1->next = CMD2 con name="grep"Maneja **un único pipe**: crea el comando siguiente y procesa sus argumentos.				ft_printf("syntax error near unexpected token `|'\n");



// 3. Procesar argumentos de CMD2				return (-1);

process_cmd_args(CMD2, &tokens);

// Añade "world" como argumento```c			}

// Para cuando llega al siguiente PIPE

```int	handle_pipe(t_cmd *current_cmd, t_token **tokens)			



Estado:{			// Crear nuevo comando

```

┌──────────────────────┐      ┌──────────────────────┐	// 1. Saltar el token PIPE			current_cmd->next = malloc(sizeof(t_cmd));

│ CMD1                 │      │ CMD2                 │

│ name: "echo"         │ ───→ │ name: "grep"         │	*tokens = (*tokens)->next;			if (!current_cmd->next)

│ args: ["hello",      │      │ args: ["world",      │

│        "world"]      │      │        NULL]         │					return (-1);

│ next: ──────────────┼──────→ next: NULL           │

└──────────────────────┘      └──────────────────────┘	// 2. Validar: no puede terminar con pipe			init_cmd(current_cmd->next);



Token actual: PIPE("|")	if (!*tokens)			current_cmd = current_cmd->next;

```

	{			

---

		ft_printf("syntax error near unexpected token `|'\n");			// Nombre del nuevo comando

### **PASO 6: Procesar segundo pipe** (process_pipes continúa)

		return (-1);			if (tmp->value && (tmp->type == WORD || tmp->type == STRING))

```c

// current_cmd avanza a CMD2	}			{

current_cmd = current_cmd->next;

					current_cmd->name = ft_strdup(tmp->value);

// Encuentra otro PIPE

if (tmp->type == PIPE)	// 3. Crear el siguiente comando				create_cmd(current_cmd->name, current_cmd);

{

	handle_pipe(current_cmd, &tmp);	if (init_next_cmd(current_cmd, tokens) == -1)				tmp = tmp->next;

	// Crea CMD3 con name="cat"

}		return (-1);			}

```

			}

Estado final:

```	// 4. Procesar sus argumentos		// Si es un valor, añadirlo al comando actual

┌──────────────────────┐      ┌──────────────────────┐      ┌──────────────────────┐

│ CMD1                 │      │ CMD2                 │      │ CMD3                 │	process_cmd_args(current_cmd->next, tokens);		else if (tmp->value && (tmp->type == WORD || tmp->type == STRING

│ name: "echo"         │ ───→ │ name: "grep"         │ ───→ │ name: "cat"          │

│ args: ["hello",      │      │ args: ["world"]      │      │ args: NULL           │					|| tmp->type == ARGS || tmp->type == SIMPLE_Q))

│        "world"]      │      │                      │      │                      │

│ next: ──────────────┼──────→ next: ──────────────┼──────→ next: NULL            │	return (0);		{

└──────────────────────┘      └──────────────────────┘      └──────────────────────┘

```}			// Si no tiene nombre, es el primer token = nombre del comando



---```			if (!current_cmd->name)



### **RESULTADO FINAL**			{



✅ **Lista enlazada de 3 comandos creada correctamente****Ejemplo:**				current_cmd->name = ft_strdup(tmp->value);



``````bash				create_cmd(current_cmd->name, current_cmd);

data->cmd apunta a:

    [echo hello world] → [grep world] → [cat] → NULLEstado: [CMD1: echo hello] → NULL			}

```

Tokens: PIPE("|") → WORD("grep") → WORD("world") → NULL			// Si ya tiene nombre, es un argumento

Esta estructura está **lista para ser ejecutada** por el executor (cuando se implemente).

         ↑			else

---

     actual			{

## 5. ¿QUÉ FALTA POR HACER?

				add_cmd_arg(current_cmd, tmp->value);

### ❌ Implementar el EXECUTOR de pipelines

handle_pipe() hace:			}

Actualmente la lista se crea pero **no se ejecuta**. Falta:

1. Salta el PIPE			tmp = tmp->next;

#### 📂 Crear: `src/executor/pipeline.c`

2. Crea CMD2 con nombre "grep"		}

**Funciones necesarias:**

3. Añade argumento "world"		else

1. **`execute_pipeline(t_cmd *cmd_list)`**

   - Función principal que ejecuta toda la lista		{

   - Llama a las siguientes funciones auxiliares

Resultado: [CMD1: echo hello] → [CMD2: grep world] → NULL			tmp = tmp->next;

2. **`count_commands(t_cmd *cmd)`**

   - Cuenta cuántos comandos hay en la lista```		}

   - Retorna N (número de comandos)

	}

3. **`create_pipes(int num_cmds)`**

   - Crea N-1 pipes del sistema operativo---	

   - Retorna array de pipes: `int pipes[][2]`

	return (0);

4. **`execute_cmd_in_pipeline(t_cmd *cmd, int idx, int pipes[][], int num)`**

   - Hace `fork()` para crear proceso hijo#### 🔹 Función 4: `process_pipes()` (líneas 72-94)}

   - En el hijo:

     - Redirige stdin desde pipe anterior (si idx > 0)```

     - Redirige stdout a pipe siguiente (si idx < N-1)

     - Cierra todos los file descriptors de pipes**¿Qué hace?**  

     - Ejecuta el comando (builtin o `execve()`)

   - En el padre: continúa al siguiente comandoRecorre **todos** los tokens y procesa **cada pipe** encontrado.---



5. **`close_all_pipes(int pipes[][], int num_pipes)`**

   - Cierra todos los pipes en el proceso padre

```c### 📊 Ejemplo Visual del Proceso

6. **`wait_all_children(int num_cmds)`**

   - Hace `waitpid()` para cada proceso hijoint	process_pipes(t_data *data)



---{```bash



#### 🔄 Modificar: `src/executor/test_executor.c`	t_token	*tmp;Input: "ls -la | grep test | wc -l"



```c	t_cmd	*current_cmd;

void execute_command(t_data *data)

{Tokens generados por el lexer:

	if (!data->cmd)

		return;	tmp = data->tokens;┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐

	

	// ⭐ Si hay pipes (lista enlazada), usar pipeline executor	current_cmd = data->cmd;│WORD  │  │ARGS  │  │PIPE  │  │WORD  │  │WORD  │  │PIPE  │  │WORD  │

	if (data->cmd->next)

		execute_pipeline(data->cmd);	│"ls"  │→ │"-la" │→ │ "|"  │→ │"grep"│→ │"test"│→ │ "|"  │→ │"wc"  │→ ...

	else

		// Comando simple, ejecutar como hasta ahora	// Recorrer toda la lista de tokens└──────┘  └──────┘  └──────┘  └──────┘  └──────┘  └──────┘  └──────┘

		execute_single_command(data->cmd);

}	while (tmp)

```

	{Proceso de construcción de la lista:

---

		// Si encontramos un PIPE

### 📊 RESUMEN: Estado actual vs Completo

		if (tmp->type == PIPE)1. Crear cmd1:

| Componente | Estado | Ubicación |

|-----------|--------|-----------|		{   ┌────────────────┐

| Tokenizar `\|` | ✅ HECHO | `checker.c` |

| Crear lista enlazada | ✅ HECHO | `pipes.c` |			// Procesar ese pipe   │ cmd1           │

| Liberar lista completa | ✅ HECHO | `cmd_utils.c` |

| Fork + pipe() + dup2() | ❌ FALTA | `pipeline.c` (crear) |			if (handle_pipe(current_cmd, &tmp) == -1)   │ name: "ls"     │

| Ejecutar en procesos | ❌ FALTA | `pipeline.c` (crear) |

| Esperar procesos hijos | ❌ FALTA | `pipeline.c` (crear) |				return (-1);   │ args: ["-la"]  │



---			   │ next: NULL     │



## 🎯 CONCLUSIÓN			// Avanzar al comando recién creado   └────────────────┘



### ✅ Lo que FUNCIONA:			current_cmd = current_cmd->next;



Cuando escribes:		}2. Encontrar PIPE → Crear cmd2:

```bash

echo uno dos | grep dos | cat		else   ┌────────────────┐      ┌─────────────────┐

```

		{   │ cmd1           │      │ cmd2            │

El parser crea perfectamente esta estructura:

```			tmp = tmp->next;   │ name: "ls"     │ ───→ │ name: "grep"    │

[CMD1: echo] → [CMD2: grep] → [CMD3: cat] → NULL

    ↓              ↓		}   │ args: ["-la"]  │      │ args: ["test"]  │

[uno, dos]      [dos]

```	}   │ next: ─────────┼──┐   │ next: NULL      │



**NO hay memory leaks** (probado con valgrind).	   └────────────────┘  │   └─────────────────┘



---	return (0);                       │



### ⚠️ Lo que NO funciona:}                       └──────→



Los comandos **no se ejecutan** en procesos separados. Solo se ejecuta el primer comando:```



```bash3. Encontrar PIPE → Crear cmd3:

$ echo uno dos | grep dos | cat

uno dos     # ❌ Solo ejecuta "echo", ignora pipe y comandos siguientes**Ejemplo:**   ┌────────────────┐      ┌─────────────────┐      ┌────────────────┐

```

```bash   │ cmd1           │      │ cmd2            │      │ cmd3           │

Para que funcione correctamente, necesitas implementar `execute_pipeline()` con:

- `fork()` para crear N procesosTokens: WORD("echo") → WORD("hi") → PIPE("|") → WORD("cat") → PIPE("|") → WORD("wc") → NULL   │ name: "ls"     │ ───→ │ name: "grep"    │ ───→ │ name: "wc"     │

- `pipe()` para crear N-1 pipes del SO

- `dup2()` para redirigir stdin/stdout                                      ↑                          ↑   │ args: ["-la"]  │      │ args: ["test"]  │      │ args: ["-l"]   │

- `execve()` o ejecutar builtin en cada proceso

                                  pipe 1                    pipe 2   │ next: ─────────┼──────┤ next: ─────────┼──────┤ next: NULL     │

---

   └────────────────┘      └─────────────────┘      └────────────────┘

### 📝 Función de debug

process_pipes() recorre y encuentra:

Puedes usar `print_cmd_list()` en `cmd_utils.c` para visualizar la lista:

- Pipe 1 → crea CMD2 (cat)Resultado final: Lista enlazada de 3 comandos lista para ejecutar

```c

// En mini_init.c después del lexer:- Pipe 2 → crea CMD3 (wc)```

if (lexer(input, data) >= 0)

{

	print_cmd_list((*data)->cmd);  // ⭐ Ver la lista creada

	execute_command(*data);Resultado: [CMD1] → [CMD2] → [CMD3] → NULL---

}

``````



Output esperado:### ⚠️ IMPORTANTE: Modificar `free_cmd()` 

```

=== LISTA DE COMANDOS ===---



🔹 CMD 1: echoTu función actual solo libera UN comando. Necesitas que recorra TODA la lista:

   Args: [uno] [dos]

   ➡️  Siguiente comando### 📄 **4. cmd_utils.c - Liberar TODA la lista**



🔹 CMD 2: grep```c

   Args: [dos]

   ➡️  Siguiente comando**Ubicación:** Función `free_cmd()` líneas 134-148// En src/utils/cmd_utils.c



🔹 CMD 3: catvoid	free_cmd(t_cmd *cmd)

   🏁 Último comando

=========================**¿Qué hace?**  {

```

Libera **recursivamente** toda la lista enlazada, no solo un comando.	t_cmd	*next;

---

	int		i;

**Parsing: ✅ 100% completo**  

**Ejecución: ❌ 0% completo**  **ANTES (solo liberaba 1 comando):**

**Next step: Implementar `src/executor/pipeline.c`**

```c	// ⭐ CAMBIO: Bucle while para recorrer toda la lista

void	free_cmd(t_cmd *cmd)	while (cmd)

{	{

	free(cmd->name);		next = cmd->next; // ⭐ Guardar el siguiente ANTES de liberar

	free(cmd->args);		

	free(cmd);  // ❌ cmd->next se pierde (memory leak)		if (cmd->name)

}			free(cmd->name);

```		

		if (cmd->args)

**AHORA (libera toda la lista):**		{

```c			i = 0;

void	free_cmd(t_cmd *cmd)			while (cmd->args[i])

{			{

	t_cmd	*next;				free(cmd->args[i]);

				i++;

	while (cmd)  // ⭐ Bucle para recorrer toda la lista			}

	{			free(cmd->args);

		next = cmd->next;  // ⭐ Guardar siguiente ANTES de liberar		}

				

		// Liberar contenido del comando actual		if (cmd->redirections)

		free_cmd_content(cmd);			free_redirs(cmd->redirections);

				

		// Liberar el comando		free(cmd);

		free(cmd);		cmd = next; // ⭐ Avanzar al siguiente

			}

		// Avanzar al siguiente}

		cmd = next;```

	}

}---

```

### 🐛 Función de Debug (muy útil)

**Ejemplo:**

```bashAñade esto en `cmd_utils.c` para ver si la lista se creó bien:

Lista: [CMD1] → [CMD2] → [CMD3] → NULL

```c

Iteración 1: Libera CMD1, avanza a CMD2void	print_cmd_list(t_cmd *cmd)

Iteración 2: Libera CMD2, avanza a CMD3{

Iteración 3: Libera CMD3, avanza a NULL	int	i;

Fin: Todo liberado ✅	int	num;

```

	num = 1;

---	ft_printf("\n=== LISTA DE COMANDOS ===\n");

	while (cmd)

## 4. EJEMPLO PASO A PASO	{

		ft_printf("\n🔹 COMANDO %d:\n", num);

Vamos a ver **todo el proceso** con un ejemplo real:		ft_printf("   Nombre: %s\n", cmd->name);

		

### 📝 Input del usuario		if (cmd->args)

```bash		{

echo hello world | grep world | cat			ft_printf("   Args: ");

```			i = 0;

			while (cmd->args[i])

---			{

				ft_printf("[%s] ", cmd->args[i]);

### **PASO 1: Tokenización** (lexer.c)				i++;

			}

El lexer recorre el string y crea tokens:			ft_printf("\n");

		}

```		

Tokens creados:		if (cmd->redirections)

┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────┐  ┌──────────┐  ┌──────────┐  ┌──────┐  ┌──────┐			ft_printf("   ⚠️  Tiene redirecciones\n");

│ WORD     │→ │ WORD     │→ │ WORD     │→ │ PIPE │→ │ WORD     │→ │ WORD     │→ │ PIPE │→ │ WORD │		

│ "echo"   │  │ "hello"  │  │ "world"  │  │ "|"  │  │ "grep"   │  │ "world"  │  │ "|"  │  │ "cat"│		if (cmd->next)

└──────────┘  └──────────┘  └──────────┘  └──────┘  └──────────┘  └──────────┘  └──────┘  └──────┘			ft_printf("   ➡️  Tiene siguiente comando\n");

```		else

			ft_printf("   🏁 Último comando\n");

---		

		cmd = cmd->next;

### **PASO 2: Crear primer comando** (lexer.c → check_and_exp)		num++;

	}

La función `init_first_cmd()` crea el primer comando:	ft_printf("\n=========================\n\n");

}

```c```

// Token actual: WORD("echo")

cmd->name = ft_strdup("echo");  // Copiar nombreLlámala después del lexer para ver si funciona:

cmd->builtin_id = BUILTIN_ECHO;  // Identificar que es builtin

``````c

// En process_input() después de lexer:

Estado:if (lexer(input, data) >= 0)

```{

┌────────────────┐	print_cmd_list((*data)->cmd); // Debug

│ CMD1           │	execute_command(*data);

│ name: "echo"   │}

│ args: NULL     │```

│ next: NULL     │

└────────────────┘---



Token actual: WORD("hello")### 📝 No olvides añadir a `minishell.h`:

```

```c

---//--PIPES--

int		handle_pipe(t_cmd *current_cmd, t_token **tokens);

### **PASO 3: Añadir argumentos del primer comando** (process_first_cmd_args)int		process_pipes(t_data *data);

int		build_cmd_list(t_data *data);

Recorre tokens mientras NO sea PIPE:void	print_cmd_list(t_cmd *cmd);  // Debug

```

```c

// Token: WORD("hello")---

add_cmd_arg(cmd, "hello");  // Añadir arg[0]

## 3. Implementación del Executor

// Token: WORD("world")

add_cmd_arg(cmd, "world");  // Añadir arg[1]### Crear `src/executor/pipeline.c`



// Token: PIPE("|") → STOP```

```// PSEUDOCÓDIGO - Funciones principales para pipeline



Estado:FUNCIÓN count_commands(cmd):

```    contador = 0

┌──────────────────────┐    MIENTRAS cmd exista:

│ CMD1                 │        contador++

│ name: "echo"         │        cmd = cmd->next

│ args: ["hello",      │    RETORNAR contador

│        "world",      │

│        NULL]         │FUNCIÓN create_pipes(num_pipes):

│ next: NULL           │    pipes = array_de_arrays[num_pipes]

└──────────────────────┘    SI pipes es NULL:

        RETORNAR NULL

Token actual: PIPE("|")    

```    PARA i desde 0 hasta num_pipes:

        pipes[i] = malloc(2 enteros)  // [read_end, write_end]

---        SI pipes[i] es NULL:

            RETORNAR NULL

### **PASO 4: Detectar pipe y llamar process_pipes()** (check_and_exp)        SI pipe(pipes[i]) falla:

            imprimir_error("pipe")

```c            RETORNAR NULL

if (tmp && tmp->type == PIPE)  // ✅ Es PIPE    

{    RETORNAR pipes

	process_pipes(*data);  // Llamar función de pipes.c

}FUNCIÓN close_all_pipes(pipes, num_pipes):

```    PARA i desde 0 hasta num_pipes:

        cerrar(pipes[i][0])  // read end

---        cerrar(pipes[i][1])  // write end



### **PASO 5: Procesar primer pipe** (process_pipes → handle_pipe)FUNCIÓN free_pipes(pipes, num_pipes):

    PARA i desde 0 hasta num_pipes:

```c        liberar(pipes[i])

// 1. Saltar token PIPE    liberar(pipes)

tokens = tokens->next;  // Ahora apunta a WORD("grep")

FUNCIÓN setup_pipes_for_child(pipes, cmd_index, num_cmds):

// 2. Crear CMD2    // Redirigir STDIN desde pipe anterior

init_next_cmd(CMD1, &tokens);    SI cmd_index > 0:

// Resultado: CMD1->next = CMD2 con name="grep"        dup2(pipes[cmd_index - 1][0], STDIN_FILENO)

    

// 3. Procesar argumentos de CMD2    // Redirigir STDOUT a pipe siguiente

process_cmd_args(CMD2, &tokens);    SI cmd_index < num_cmds - 1:

// Añade "world" como argumento        dup2(pipes[cmd_index][1], STDOUT_FILENO)

// Para cuando llega al siguiente PIPE    

```    // Cerrar TODAS las pipes en el hijo

    cerrar_todas_pipes(pipes, num_cmds - 1)

Estado:

```FUNCIÓN execute_in_child(cmd, data):

┌──────────────────────┐      ┌──────────────────────┐    // Aplicar redirections si hay

│ CMD1                 │      │ CMD2                 │    SI cmd tiene redirections:

│ name: "echo"         │ ───→ │ name: "grep"         │        aplicar_redirections(cmd)

│ args: ["hello",      │      │ args: ["world",      │    

│        "world"]      │      │        NULL]         │    // Si es builtin, ejecutar y salir

│ next: ──────────────┼──────→ next: NULL           │    SI cmd es builtin:

└──────────────────────┘      └──────────────────────┘        // Nota: algunos builtins no deben ejecutarse en hijo

        // (cd, export, unset, exit afectan al padre)

Token actual: PIPE("|")        ret = ejecutar_builtin(data)

```        exit(ret)

    

---    // Buscar comando en PATH

    cmd_path = buscar_comando_en_path(cmd->name, data->env)

### **PASO 6: Procesar segundo pipe** (process_pipes continúa)    SI NO cmd_path:

        imprimir(cmd->name + ": command not found")

```c        exit(127)

// current_cmd avanza a CMD2    

current_cmd = current_cmd->next;    // Convertir env lista a array

    envp = env_lista_a_array(data->env)

// Encuentra otro PIPE    

if (tmp->type == PIPE)    // Ejecutar

{    execve(cmd_path, cmd->args, envp)

	handle_pipe(current_cmd, &tmp);    

	// Crea CMD3 con name="cat"    // Si llegamos aquí, execve falló

}    perror("execve")

```    liberar(cmd_path)

    liberar_array(envp)

Estado final:    exit(126)

``````

┌──────────────────────┐      ┌──────────────────────┐      ┌──────────────────────┐}

│ CMD1                 │      │ CMD2                 │      │ CMD3                 │

│ name: "echo"         │ ───→ │ name: "grep"         │ ───→ │ name: "cat"          │// Esperar a todos los hijos y retornar exit status del último

│ args: ["hello",      │      │ args: ["world"]      │      │ args: NULL           │static int wait_all_children(pid_t *pids, int num_cmds)

│        "world"]      │      │                      │      │                      │{

│ next: ──────────────┼──────→ next: ──────────────┼──────→ next: NULL            │    int i;

└──────────────────────┘      └──────────────────────┘      └──────────────────────┘    int status;

```    int last_status = 0;

    

---    i = 0;

    while (i < num_cmds)

### **RESULTADO FINAL**    {

        waitpid(pids[i], &status, 0);

✅ **Lista enlazada de 3 comandos creada correctamente**        if (WIFEXITED(status))

            last_status = WEXITSTATUS(status);

```        i++;

data->cmd apunta a:    }

    [echo hello world] → [grep world] → [cat] → NULL    

```    return (last_status);

}

Esta estructura está **lista para ser ejecutada** por el executor (cuando se implemente).

// Función principal para ejecutar pipeline

---int execute_pipeline(t_data *data)

{

## 5. ¿QUÉ FALTA POR HACER?    int num_cmds;

    int **pipes;

### ❌ Implementar el EXECUTOR de pipelines    pid_t *pids;

    t_cmd *current;

Actualmente la lista se crea pero **no se ejecuta**. Falta:    int i;

    int exit_status;

#### 📂 Crear: `src/executor/pipeline.c`    

    num_cmds = count_commands(data->cmd);

**Funciones necesarias:**    

    // Si solo hay un comando, ejecutar normalmente

1. **`execute_pipeline(t_cmd *cmd_list)`**    if (num_cmds == 1)

   - Función principal que ejecuta toda la lista        return (execute_command(data));

   - Llama a las siguientes funciones auxiliares    

    // Crear pipes

2. **`count_commands(t_cmd *cmd)`**    pipes = create_pipes(num_cmds - 1);

   - Cuenta cuántos comandos hay en la lista    if (!pipes)

   - Retorna N (número de comandos)        return (1);

    

3. **`create_pipes(int num_cmds)`**    // Crear array de PIDs

   - Crea N-1 pipes del sistema operativo    pids = malloc(sizeof(pid_t) * num_cmds);

   - Retorna array de pipes: `int pipes[][2]`    if (!pids)

    {

4. **`execute_cmd_in_pipeline(t_cmd *cmd, int idx, int pipes[][], int num)`**        free_pipes(pipes, num_cmds - 1);

   - Hace `fork()` para crear proceso hijo        return (1);

   - En el hijo:    }

     - Redirige stdin desde pipe anterior (si idx > 0)    

     - Redirige stdout a pipe siguiente (si idx < N-1)    // Crear proceso para cada comando

     - Cierra todos los file descriptors de pipes    current = data->cmd;

     - Ejecuta el comando (builtin o `execve()`)    i = 0;

   - En el padre: continúa al siguiente comando    while (i < num_cmds && current)

    {

5. **`close_all_pipes(int pipes[][], int num_pipes)`**        pids[i] = fork();

   - Cierra todos los pipes en el proceso padre        

        if (pids[i] == -1)

6. **`wait_all_children(int num_cmds)`**        {

   - Hace `waitpid()` para cada proceso hijo            perror("fork");

            // TODO: cleanup y exit

---            return (1);

        }

#### 🔄 Modificar: `src/executor/test_executor.c`        

        if (pids[i] == 0)  // Proceso hijo

```c        {

void execute_command(t_data *data)            setup_pipes_for_child(pipes, i, num_cmds);

{            execute_in_child(current, data);

	if (!data->cmd)            exit(1);  // No debería llegar aquí

		return;        }

	        

	// ⭐ Si hay pipes (lista enlazada), usar pipeline executor        current = current->next;

	if (data->cmd->next)        i++;

		execute_pipeline(data->cmd);    }

	else    

		// Comando simple, ejecutar como hasta ahora    // Proceso padre: cerrar todas las pipes

		execute_single_command(data->cmd);    close_all_pipes(pipes, num_cmds - 1);

}    

```    // Esperar a todos los hijos

    exit_status = wait_all_children(pids, num_cmds);

---    

    // Cleanup

### 📊 RESUMEN: Estado actual vs Completo    free_pipes(pipes, num_cmds - 1);

    free(pids);

| Componente | Estado | Ubicación |    

|-----------|--------|-----------|    return (exit_status);

| Tokenizar `\|` | ✅ HECHO | `checker.c` |}

| Crear lista enlazada | ✅ HECHO | `pipes.c` |

| Liberar lista completa | ✅ HECHO | `cmd_utils.c` |// Esperar a todos los hijos y retornar exit status del último

| Fork + pipe() + dup2() | ❌ FALTA | `pipeline.c` (crear) |FUNCIÓN wait_all_children(pids, num_cmds):

| Ejecutar en procesos | ❌ FALTA | `pipeline.c` (crear) |    last_status = 0

| Esperar procesos hijos | ❌ FALTA | `pipeline.c` (crear) |    

    PARA i desde 0 hasta num_cmds:

---        waitpid(pids[i], status, 0)

        SI proceso terminó normalmente:

## 🎯 CONCLUSIÓN            last_status = código_salida(status)

    

### ✅ Lo que FUNCIONA:    RETORNAR last_status



Cuando escribes:// Función principal para ejecutar pipeline

```bashFUNCIÓN execute_pipeline(data):

echo uno dos | grep dos | cat    num_cmds = contar_comandos(data->cmd)

```    

    // Si solo hay un comando, ejecutar normalmente

El parser crea perfectamente esta estructura:    SI num_cmds == 1:

```        RETORNAR execute_command(data)

[CMD1: echo] → [CMD2: grep] → [CMD3: cat] → NULL    

    ↓              ↓    // Crear pipes

[uno, dos]      [dos]    pipes = crear_pipes(num_cmds - 1)

```    SI pipes es NULL:

        RETORNAR 1

**NO hay memory leaks** (probado con valgrind).    

    // Crear array de PIDs

---    pids = malloc(num_cmds PIDs)

    SI pids es NULL:

### ⚠️ Lo que NO funciona:        liberar_pipes(pipes, num_cmds - 1)

        RETORNAR 1

Los comandos **no se ejecutan** en procesos separados. Solo se ejecuta el primer comando:    

    // Crear proceso para cada comando

```bash    current = data->cmd

$ echo uno dos | grep dos | cat    PARA i desde 0 hasta num_cmds:

uno dos     # ❌ Solo ejecuta "echo", ignora pipe y comandos siguientes        pids[i] = fork()

```        

        SI fork falló:

Para que funcione correctamente, necesitas implementar `execute_pipeline()` con:            perror("fork")

- `fork()` para crear N procesos            // TODO: cleanup y exit

- `pipe()` para crear N-1 pipes del SO            RETORNAR 1

- `dup2()` para redirigir stdin/stdout        

- `execve()` o ejecutar builtin en cada proceso        SI pids[i] == 0:  // Proceso hijo

            configurar_pipes_hijo(pipes, i, num_cmds)

---            ejecutar_en_hijo(current, data)

            exit(1)  // No debería llegar aquí

### 📝 Archivo de pruebas        

        current = current->next

Puedes usar la función de debug `print_cmd_list()` en `cmd_utils.c` para visualizar la lista:    

    // Proceso padre: cerrar todas las pipes

```c    cerrar_todas_pipes(pipes, num_cmds - 1)

// En mini_init.c después del lexer:    

if (lexer(input, data) >= 0)    // Esperar a todos los hijos

{    exit_status = esperar_todos_hijos(pids, num_cmds)

	print_cmd_list((*data)->cmd);  // ⭐ Ver la lista creada    

	execute_command(*data);    // Cleanup

}    liberar_pipes(pipes, num_cmds - 1)

```    liberar(pids)

    

Output esperado:    RETORNAR exit_status

``````

=== LISTA DE COMANDOS ===

---

🔹 CMD 1: echo

   Args: [uno] [dos]## 4. Funciones auxiliares

   ➡️  Siguiente comando

### Convertir lista de env a array (para execve)

🔹 CMD 2: grep```

   Args: [dos]// PSEUDOCÓDIGO - Esta función necesitas crearla

   ➡️  Siguiente comando

FUNCIÓN env_list_to_array(env):

🔹 CMD 3: cat    contador = 0

   🏁 Último comando    tmp = env

=========================    

```    // Contar variables

    MIENTRAS tmp exista:

---        contador++

        tmp = tmp->next

**Parsing: ✅ 100% completo**      

**Ejecución: ❌ 0% completo**      // Crear array

**Next step: Implementar `src/executor/pipeline.c`**    envp = malloc(array de (contador + 1) strings)

    SI envp es NULL:
        RETORNAR NULL
    
    // Llenar array con "KEY=VALUE"
    tmp = env
    i = 0
    MIENTRAS tmp exista:
        temp = unir(tmp->key, "=")
        envp[i] = unir(temp, tmp->value)
        liberar(temp)
        tmp = tmp->next
        i++
    envp[i] = NULL
    
    RETORNAR envp
```

---

## 5. Casos especiales

### 5.1. Builtins en pipes
```bash
cd /tmp | pwd  # pwd debe ejecutarse en hijo, no afecta al padre
export VAR=42 | echo $VAR  # export en hijo no afecta al padre
```

**Solución:**
- Builtins que modifican el padre (`cd`, `export`, `unset`, `exit`) se ejecutan en el hijo cuando están en pipe
- El comportamiento es correcto según bash

### 5.2. Redirections + Pipes
```bash
cat < input.txt | grep hello > output.txt
```

**Orden de aplicación:**
1. Configurar pipes primero
2. Aplicar redirections después (pueden sobrescribir stdin/stdout)

```
FUNCIÓN execute_in_child(cmd, data):
    // 1. Pipes ya configuradas
    // 2. Aplicar redirections (pueden sobrescribir)
    SI cmd tiene redirections:
        aplicar_redirections(cmd)
    
    // 3. Ejecutar comando
    // ...
```

### 5.3. Solo un builtin sin pipe
```bash
cd /tmp  # Debe ejecutarse en el padre
```

En `execute_command()`:
```
FUNCIÓN execute_command(data):
    // Si no hay pipe Y es builtin, ejecutar en padre
    SI data->pipe_flag == 0 Y data->cmd es builtin:
        RETORNAR ejecutar_builtin_en_padre(data)
    
    // Si hay pipe, usar pipeline
    SI data->pipe_flag == 1:
        RETORNAR execute_pipeline(data)
    
    // Comando externo simple
    RETORNAR ejecutar_comando_externo(data)
```

---

## 6. Testing

### Test básico
```bash
echo hello | cat
# Output: hello

ls -l | wc -l
# Output: número de líneas
```

### Test múltiples pipes
```bash
cat file.txt | grep test | wc -l
echo "hello\nworld\ntest" | grep test | cat
```

### Test con redirections
```bash
cat < input.txt | grep hello > output.txt
ls | > output.txt  # Error: sintaxis inválida
```

### Test con builtins
```bash
pwd | cat
echo $USER | cat
cd /tmp | pwd  # pwd se ejecuta en hijo, muestra /tmp
```

### Test de errores
```bash
invalidcmd | cat
# Output: invalidcmd: command not found

cat nonexistent | wc -l
# Output: cat: nonexistent: No such file or directory
```

---

## 📋 Checklist de implementación

- [ ] Modificar `check_and_exp()` para crear lista de comandos
- [ ] Crear `execute_pipeline()` en nuevo archivo
- [ ] Implementar `create_pipes()`
- [ ] Implementar `setup_pipes_for_child()`
- [ ] Implementar `execute_in_child()`
- [ ] Implementar `env_list_to_array()`
- [ ] Modificar `execute_command()` para detectar pipes
- [ ] Testear con casos simples (2 comandos)
- [ ] Testear con múltiples pipes (3+ comandos)
- [ ] Testear con redirections + pipes
- [ ] Testear con builtins en pipes
- [ ] Verificar memory leaks con valgrind
- [ ] Verificar exit codes correctos

---

## 🐛 Debugging tips

### Ver qué comandos se están creando:
```
// Función de debug para ver la lista de comandos

FUNCIÓN print_cmd_list(cmd):
    i = 1
    MIENTRAS cmd exista:
        imprimir("CMD " + i + ": " + cmd->name)
        imprimir("  Args: ")
        PARA j desde 0 hasta fin_args:
            imprimir(cmd->args[j] + " ")
        imprimir_salto_linea()
        cmd = cmd->next
        i++
```

### Ver file descriptors:
```bash
ls -la /proc/[PID]/fd/
```

### Strace para ver syscalls:
```bash
strace -f ./minishell
```

---

**Buena suerte con la implementación!** 🚀

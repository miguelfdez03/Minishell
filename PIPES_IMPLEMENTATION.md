# 🔧 GUÍA COMPLETA: Implementación de Pipes en Minishell

## 📚 Tabla de contenidos
1. [Conceptos básicos](#conceptos-básicos)
2. [Modificaciones en el Parser](#modificaciones-en-el-parser)
3. [Implementación del Executor](#implementación-del-executor)
4. [Funciones auxiliares](#funciones-auxiliares)
5. [Casos especiales](#casos-especiales)
6. [Testing](#testing)

---

## 1. Conceptos básicos

### ¿Qué es una pipe?
Una pipe (`|`) conecta la salida de un comando con la entrada del siguiente:
```bash
ls -l | grep .c | wc -l
  ↓       ↓         ↓
 cmd1    cmd2      cmd3
```

### Cómo funciona internamente:
- Cada comando se ejecuta en un **proceso hijo** separado
- Se crean **N-1 pipes** para N comandos
- La pipe tiene dos extremos: `read_end [0]` y `write_end [1]`
- Se usa `dup2()` para redirigir stdin/stdout a las pipes

---

## 2. Modificaciones en el Parser

### Estado actual de tu estructura:
```c
typedef struct s_cmd {
    char *name;
    char **args;
    t_builtin_type builtin_id;
    t_redir *redirections;
    struct s_cmd *next;  // ✅ Ya tienes esto!
} t_cmd;
```

### Modificar `src/parser/lexer.c`

Actualmente tu función `check_and_exp()` procesa todos los tokens para UN solo comando. Necesitas modificarla para crear múltiples comandos cuando encuentres PIPE.

**ANTES (tu código actual):**
```c
// Tu función actual procesa todo en un solo comando
// y no crea nodos adicionales cuando encuentra PIPE
```

**DESPUÉS (pseudocódigo de lo que necesitas):**
```
FUNCIÓN check_and_exp(data):
    tmp = primer_token
    current_cmd = comando_actual
    es_primer_token = verdadero
    
    SI NO hay tokens:
        RETORNAR error
    
    expandir_variables(tokens, env, exit_status)
    tmp = primer_token
    current_cmd = cmd_actual
    es_primer_token = verdadero
    
    MIENTRAS tmp exista:
        // NUEVO: Detectar PIPE y crear nuevo comando
        SI tmp ES PIPE:
            // Crear nuevo comando y enlazarlo
            current_cmd->next = nuevo_comando_malloc()
            SI falla malloc:
                RETORNAR error
            inicializar_cmd(current_cmd->next)
            current_cmd = current_cmd->next
            es_primer_token = verdadero
            tmp = siguiente
            CONTINUAR
        
        // Procesar primer token como nombre del comando
        SI es_primer_token Y tmp tiene valor:
            SI current_cmd ya tiene nombre:
                liberar nombre anterior
            current_cmd->name = copiar(tmp->value)
            crear_cmd(current_cmd->name, current_cmd)
            es_primer_token = falso
        // Resto de tokens son argumentos
        SI NO, SI tmp tiene valor Y (tmp ES WORD O STRING O ARGS O SIMPLE_Q):
            añadir_argumento(current_cmd, tmp->value)
        
        tmp = siguiente
    
    RETORNAR éxito
```
        {
            if (current_cmd->name)
                free(current_cmd->name);
            current_cmd->name = ft_strdup(tmp->value);
            create_cmd(current_cmd->name, current_cmd);
            is_first_token = 0;
        }
        // Resto de tokens son argumentos
        else if (tmp->value && (tmp->type == WORD || tmp->type == STRING
                || tmp->type == ARGS || tmp->type == SIMPLE_Q))
        {
            add_cmd_arg(current_cmd, tmp->value);
        }
        
        tmp = tmp->next;
    }
    
    return (EXIT_SUCCESS);
}
```

---

## 3. Implementación del Executor

### Crear `src/executor/pipeline.c`

```
// PSEUDOCÓDIGO - Funciones principales para pipeline

FUNCIÓN count_commands(cmd):
    contador = 0
    MIENTRAS cmd exista:
        contador++
        cmd = cmd->next
    RETORNAR contador

FUNCIÓN create_pipes(num_pipes):
    pipes = array_de_arrays[num_pipes]
    SI pipes es NULL:
        RETORNAR NULL
    
    PARA i desde 0 hasta num_pipes:
        pipes[i] = malloc(2 enteros)  // [read_end, write_end]
        SI pipes[i] es NULL:
            RETORNAR NULL
        SI pipe(pipes[i]) falla:
            imprimir_error("pipe")
            RETORNAR NULL
    
    RETORNAR pipes

FUNCIÓN close_all_pipes(pipes, num_pipes):
    PARA i desde 0 hasta num_pipes:
        cerrar(pipes[i][0])  // read end
        cerrar(pipes[i][1])  // write end

FUNCIÓN free_pipes(pipes, num_pipes):
    PARA i desde 0 hasta num_pipes:
        liberar(pipes[i])
    liberar(pipes)

FUNCIÓN setup_pipes_for_child(pipes, cmd_index, num_cmds):
    // Redirigir STDIN desde pipe anterior
    SI cmd_index > 0:
        dup2(pipes[cmd_index - 1][0], STDIN_FILENO)
    
    // Redirigir STDOUT a pipe siguiente
    SI cmd_index < num_cmds - 1:
        dup2(pipes[cmd_index][1], STDOUT_FILENO)
    
    // Cerrar TODAS las pipes en el hijo
    cerrar_todas_pipes(pipes, num_cmds - 1)

FUNCIÓN execute_in_child(cmd, data):
    // Aplicar redirections si hay
    SI cmd tiene redirections:
        aplicar_redirections(cmd)
    
    // Si es builtin, ejecutar y salir
    SI cmd es builtin:
        // Nota: algunos builtins no deben ejecutarse en hijo
        // (cd, export, unset, exit afectan al padre)
        ret = ejecutar_builtin(data)
        exit(ret)
    
    // Buscar comando en PATH
    cmd_path = buscar_comando_en_path(cmd->name, data->env)
    SI NO cmd_path:
        imprimir(cmd->name + ": command not found")
        exit(127)
    
    // Convertir env lista a array
    envp = env_lista_a_array(data->env)
    
    // Ejecutar
    execve(cmd_path, cmd->args, envp)
    
    // Si llegamos aquí, execve falló
    perror("execve")
    liberar(cmd_path)
    liberar_array(envp)
    exit(126)
```
}

// Esperar a todos los hijos y retornar exit status del último
static int wait_all_children(pid_t *pids, int num_cmds)
{
    int i;
    int status;
    int last_status = 0;
    
    i = 0;
    while (i < num_cmds)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            last_status = WEXITSTATUS(status);
        i++;
    }
    
    return (last_status);
}

// Función principal para ejecutar pipeline
int execute_pipeline(t_data *data)
{
    int num_cmds;
    int **pipes;
    pid_t *pids;
    t_cmd *current;
    int i;
    int exit_status;
    
    num_cmds = count_commands(data->cmd);
    
    // Si solo hay un comando, ejecutar normalmente
    if (num_cmds == 1)
        return (execute_command(data));
    
    // Crear pipes
    pipes = create_pipes(num_cmds - 1);
    if (!pipes)
        return (1);
    
    // Crear array de PIDs
    pids = malloc(sizeof(pid_t) * num_cmds);
    if (!pids)
    {
        free_pipes(pipes, num_cmds - 1);
        return (1);
    }
    
    // Crear proceso para cada comando
    current = data->cmd;
    i = 0;
    while (i < num_cmds && current)
    {
        pids[i] = fork();
        
        if (pids[i] == -1)
        {
            perror("fork");
            // TODO: cleanup y exit
            return (1);
        }
        
        if (pids[i] == 0)  // Proceso hijo
        {
            setup_pipes_for_child(pipes, i, num_cmds);
            execute_in_child(current, data);
            exit(1);  // No debería llegar aquí
        }
        
        current = current->next;
        i++;
    }
    
    // Proceso padre: cerrar todas las pipes
    close_all_pipes(pipes, num_cmds - 1);
    
    // Esperar a todos los hijos
    exit_status = wait_all_children(pids, num_cmds);
    
    // Cleanup
    free_pipes(pipes, num_cmds - 1);
    free(pids);
    
    return (exit_status);
}

// Esperar a todos los hijos y retornar exit status del último
FUNCIÓN wait_all_children(pids, num_cmds):
    last_status = 0
    
    PARA i desde 0 hasta num_cmds:
        waitpid(pids[i], status, 0)
        SI proceso terminó normalmente:
            last_status = código_salida(status)
    
    RETORNAR last_status

// Función principal para ejecutar pipeline
FUNCIÓN execute_pipeline(data):
    num_cmds = contar_comandos(data->cmd)
    
    // Si solo hay un comando, ejecutar normalmente
    SI num_cmds == 1:
        RETORNAR execute_command(data)
    
    // Crear pipes
    pipes = crear_pipes(num_cmds - 1)
    SI pipes es NULL:
        RETORNAR 1
    
    // Crear array de PIDs
    pids = malloc(num_cmds PIDs)
    SI pids es NULL:
        liberar_pipes(pipes, num_cmds - 1)
        RETORNAR 1
    
    // Crear proceso para cada comando
    current = data->cmd
    PARA i desde 0 hasta num_cmds:
        pids[i] = fork()
        
        SI fork falló:
            perror("fork")
            // TODO: cleanup y exit
            RETORNAR 1
        
        SI pids[i] == 0:  // Proceso hijo
            configurar_pipes_hijo(pipes, i, num_cmds)
            ejecutar_en_hijo(current, data)
            exit(1)  // No debería llegar aquí
        
        current = current->next
    
    // Proceso padre: cerrar todas las pipes
    cerrar_todas_pipes(pipes, num_cmds - 1)
    
    // Esperar a todos los hijos
    exit_status = esperar_todos_hijos(pids, num_cmds)
    
    // Cleanup
    liberar_pipes(pipes, num_cmds - 1)
    liberar(pids)
    
    RETORNAR exit_status
```

---

## 4. Funciones auxiliares

### Convertir lista de env a array (para execve)
```
// PSEUDOCÓDIGO - Esta función necesitas crearla

FUNCIÓN env_list_to_array(env):
    contador = 0
    tmp = env
    
    // Contar variables
    MIENTRAS tmp exista:
        contador++
        tmp = tmp->next
    
    // Crear array
    envp = malloc(array de (contador + 1) strings)
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

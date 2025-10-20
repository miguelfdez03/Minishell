# 📋 FUNCIÓN tokens_to_cmd()

## 🎯 Propósito
Convierte una lista enlazada de tokens (`t_token`) en una estructura de comando (`t_cmd`) ejecutable.

## 📍 Ubicación
- **Archivo**: `src/utils/cmd_utils.c`
- **Declaración**: `src/minishell.h` línea 122

## 📝 Firma
```c
t_cmd *tokens_to_cmd(t_token *tokens);
```

## 🔄 Flujo de Ejecución

```
tokens_to_cmd()
│
├─ 1. Verificar que tokens no es NULL
│
├─ 2. create_cmd(tokens->value) → Crear comando con primer token
│     └─ Inicializa: name, args=NULL, builtin_id, redirections=NULL
│
├─ 3. Iterar por tokens->next (resto de la lista)
│     │
│     ├─ Si es WORD o STRING:
│     │   └─ add_cmd_arg(cmd, token->value) → Añadir como argumento
│     │
│     ├─ Si es REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC:
│     │   └─ process_redir(token, cmd) → Procesar redirección
│     │       ├─ Guarda tipo de redirección
│     │       ├─ Avanza al siguiente token (el archivo)
│     │       └─ add_redir(&cmd->redirections, tipo, archivo)
│     │
│     └─ Si es PIPE:
│         └─ break → Termina (pipes se manejan en otro nivel)
│
└─ 4. return cmd → Devuelve comando completo
```

## 📊 Ejemplos de Conversión

### Ejemplo 1: Comando Simple
```
TOKENS: [echo] [hello] [world]
         WORD   WORD    WORD

RESULTADO:
cmd->name = "echo"
cmd->args = ["hello", "world", NULL]
cmd->builtin_id = BUILTIN_ECHO (3)
cmd->redirections = NULL
```

### Ejemplo 2: Con Redirección Salida
```
TOKENS: [grep] [pattern] [>] [output.txt]
         WORD   WORD      OUT  WORD

RESULTADO:
cmd->name = "grep"
cmd->args = ["pattern", NULL]
cmd->builtin_id = BUILTIN_NONE (-1)
cmd->redirections = [type=REDIR_OUT, file="output.txt"]
```

### Ejemplo 3: Con Múltiples Redirecciones
```
TOKENS: [cat] [<] [input.txt] [>] [output.txt]
         WORD  IN   WORD       OUT  WORD

RESULTADO:
cmd->name = "cat"
cmd->args = NULL
cmd->builtin_id = BUILTIN_NONE (-1)
cmd->redirections = [type=REDIR_IN, file="input.txt"] 
                    → [type=REDIR_OUT, file="output.txt"]
```

### Ejemplo 4: Se Detiene en PIPE
```
TOKENS: [ls] [-l] [|] [grep] [txt]
         WORD WORD PIPE WORD   WORD

RESULTADO (solo procesa hasta el pipe):
cmd->name = "ls"
cmd->args = ["-l", NULL]
cmd->builtin_id = BUILTIN_NONE (-1)
cmd->redirections = NULL

(Los tokens después del pipe se procesan en otra llamada)
```

## 🔧 Funciones Auxiliares Utilizadas

### process_redir()
```c
static t_token *process_redir(t_token *current, t_cmd *cmd)
```
- Guarda el tipo de redirección del token actual
- Avanza al siguiente token (debe ser el nombre del archivo)
- Llama a `add_redir()` con el tipo y el archivo
- Retorna el puntero al token del archivo

**Ejemplo:**
```
current = [>] [output.txt]
         tipo  archivo
         
1. redir_type = REDIR_OUT
2. current = current->next → [output.txt]
3. add_redir(&cmd->redirections, REDIR_OUT, "output.txt")
4. return current (puntero a [output.txt])
```

### create_cmd()
```c
t_cmd *create_cmd(char *cmd_name)
```
- Reserva memoria para `t_cmd`
- Inicializa todos los campos (init_cmd)
- Duplica el nombre del comando
- Identifica si es builtin y asigna `builtin_id`

### add_cmd_arg()
```c
void add_cmd_arg(t_cmd *cmd, char *arg)
```
- Cuenta argumentos actuales
- Reserva memoria para nuevo array (+1 elemento)
- Copia argumentos existentes
- Añade nuevo argumento al final
- Termina con NULL

### add_redir()
```c
void add_redir(t_redir **redir, t_token_type type, char *value)
```
- Crea nuevo nodo de redirección
- Lo añade al final de la lista enlazada
- Mantiene el orden de las redirecciones

## 🔗 Integración en el Lexer

```c
int lexer(char *line, t_data **data)
{
    // ... tokenización ...
    
    // Expandir variables ($VAR, $?, $$, etc.)
    expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);
    
    // Convertir tokens en comando
    (*data)->cmd = tokens_to_cmd((*data)->tokens);
    if (!(*data)->cmd)
        return (EXIT_FAILURE);
        
    return (EXIT_SUCCESS);
}
```

## ⚠️ Notas Importantes

1. **Primer token = nombre del comando**: El primer token SIEMPRE debe ser el nombre del comando
2. **PIPE detiene el proceso**: Cuando encuentra un pipe, termina y retorna (pipes se manejan a nivel superior)
3. **Redirecciones en orden**: Mantiene el orden de aparición de las redirecciones
4. **Duplica strings**: Usa `ft_strdup()` para duplicar los valores, no guarda punteros originales
5. **NULL safety**: Verifica que `tokens` no sea NULL antes de procesar

## 🧪 Tests de Verificación

Ver archivo: `test_tokens_to_cmd.c`

Ejecutar:
```bash
cc -o test_tokens_to_cmd test_tokens_to_cmd.c src/utils/cmd_utils.c \
   src/utils/redir_utils.c src/utils/utils.c src/utils/utils_2.c \
   src/executor/path_utils.c libft/libft.a -I. && ./test_tokens_to_cmd
```

## 📚 Relación con Otras Funciones

```
main.c
  └─ lexer()
       ├─ Tokenización (handle_words, handle_quotes, check_redir)
       ├─ expand_variables() ← Expande $VAR
       └─ tokens_to_cmd() ← ESTA FUNCIÓN
            ├─ create_cmd()
            ├─ add_cmd_arg()
            └─ process_redir()
                 └─ add_redir()
```

## 🎓 Ventajas de Este Diseño

✅ **Separación de responsabilidades**: Lexer tokeniza, esta función estructura
✅ **Manejo de pipes**: Se detiene en pipes para procesar comandos por separado
✅ **Extensible**: Fácil añadir nuevos tipos de tokens
✅ **Memoria limpia**: Duplica strings, no depende de tokens originales
✅ **Orden preservado**: Mantiene orden de argumentos y redirecciones

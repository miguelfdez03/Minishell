# 📋 TODO - Parser de Minishell

## ⚠️ TAREAS PRIORITARIAS (Hacer primero)

### 1. 🔴 VALIDACIÓN DE SINTAXIS
**Prioridad: CRÍTICA**  
**Archivos a crear/modificar:**
- `src/parser/syntax_validator.c` (nuevo)
- `src/parser/lexer.c` (modificar)

**Casos a validar:**
```bash
# Pipes mal ubicadas
|                 # Error: pipe al inicio
ls |              # Error: pipe al final (sin comando después)
ls | | cat        # Error: pipes consecutivas

# Redirecciones mal formadas
ls >              # Error: redirección sin archivo
cat < >           # Error: redirección sin archivo
ls > > file       # Error: redirecciones consecutivas
ls > | cat        # Error: redirección seguida de pipe

# Heredoc incompleto
cat <<            # Error: heredoc sin delimitador
cat << EOF        # Error: heredoc sin cerrar (debe terminar con EOF)

# Comillas sin cerrar (ya lo tienes pero verificar)
echo "hello       # Ya manejado con multilinea
```

**Implementación sugerida:**
```
FUNCIÓN validate_syntax(tokens):
    current = primer_token
    
    // 1. Verificar que no empiece con pipe
    SI current ES PIPE:
        RETORNAR error("syntax error near unexpected token `|'")
    
    MIENTRAS current exista:
        // 2. Verificar redirecciones
        SI current ES redirección (<, >, <<, >>):
            SI NO hay siguiente token O siguiente NO es WORD:
                RETORNAR error("syntax error near unexpected token")
        
        // 3. Verificar pipes
        SI current ES PIPE:
            SI NO hay siguiente token:
                RETORNAR error("syntax error near unexpected token `|'")
            SI siguiente ES PIPE:
                RETORNAR error("syntax error near unexpected token `|'")
        
        current = siguiente_token
    
    RETORNAR éxito
```

---

### 2. 🔴 PIPES COMPLETOS (Lista enlazada de comandos)
**Prioridad: CRÍTICA**  
**Archivos a crear/modificar:**
- `src/parser/lexer.c` (modificar)
- `src/executor/pipeline.c` (nuevo)

**Problema actual:**
```c
// Ahora solo tienes UN comando:
t_data->cmd = [comando único]

// Necesitas una LISTA de comandos:
t_data->cmd = [cmd1] -> [cmd2] -> [cmd3] -> NULL
```

**Implementación en el parser:**
```c
// En lexer.c - función check_and_exp()
// Cuando encuentres un token PIPE:

static int check_and_exp(t_data **data)
{
    t_token *tmp;
    t_cmd *current_cmd;
    
    if (!(*data)->tokens)
        return (EXIT_FAILURE);
    
    expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);
    tmp = (*data)->tokens;
    current_cmd = (*data)->cmd;
    
    while (tmp)
    {
        if (tmp->type == PIPE)
        {
            // NUEVO: Crear siguiente comando en la lista
            current_cmd->next = create_new_cmd();
            current_cmd = current_cmd->next;
            tmp = tmp->next;
            continue;
        }
        
        // Procesar token normal...
        if (tmp->type == WORD && !current_cmd->name)
            current_cmd->name = ft_strdup(tmp->value);
        else if (tmp->value)
            add_cmd_arg(current_cmd, tmp->value);
            
        tmp = tmp->next;
    }
    
    return (EXIT_SUCCESS);
}
```

**Implementación en el executor:**
Ver archivo `PIPES_IMPLEMENTATION.md` para detalles completos.

---

### 3. 🟡 HEREDOCS FUNCIONALES
**Prioridad: ALTA**  
**Archivos a crear/modificar:**
- `src/parser/heredoc.c` (nuevo)
- `src/parser/checker.c` (modificar)

**Problema actual:**
Solo detectas `<<` pero no lees el contenido del heredoc.

**Cómo debe funcionar:**
```bash
cat << EOF
line 1
line 2
line 3
EOF
```

**Implementación sugerida:**
```
FUNCIÓN read_heredoc(delimiter, env, debe_expandir):
    contenido = vacío
    
    BUCLE INFINITO:
        linea = leer_linea_con_prompt("> ")
        
        SI linea es NULL (EOF):
            IMPRIMIR "warning: here-document delimited by end-of-file"
            ROMPER
        
        SI linea == delimiter:
            LIBERAR linea
            ROMPER
        
        // Expandir variables si debe_expandir = verdadero
        SI debe_expandir:
            linea = expandir_variables(linea, env)
        
        contenido = unir(contenido, linea)
        contenido = unir(contenido, "\n")
        LIBERAR linea
    
    RETORNAR contenido

// Detectar si el delimitador tiene comillas:
// << EOF    → expandir variables
// << 'EOF'  → NO expandir variables
// << "EOF"  → expandir variables
```

**Integrar en checker.c:**
```
FUNCIÓN handle_heredoc(linea, posición, data):
    // Leer delimitador
    delimiter = extraer_delimitador(linea, posición, debe_expandir)
    
    // Leer contenido del heredoc
    contenido = read_heredoc(delimiter, data->env, debe_expandir)
    
    // Guardar en redirections
    añadir_redireccion(data->cmd->redirections, HEREDOC, contenido)
    
    LIBERAR delimiter
    RETORNAR nueva_posición
```

---

## 📌 TAREAS SECUNDARIAS (Hacer después)

### 4. 🟢 WILDCARDS (si es necesario para el subject)
**Prioridad: MEDIA (Bonus)**  
**Archivos a crear:**
- `src/expander/wildcard.c` (nuevo)

**Casos:**
```bash
ls *.c           # Expandir a todos los archivos .c
echo test*       # Expandir test*
cat file?.txt    # ? = un carácter cualquiera
```

**Implementación:**
```
// Usar readdir() para listar archivos
// Usar fnmatch() o implementar pattern matching

FUNCIÓN expand_wildcard(pattern):
    directorio = abrir_directorio(".")
    matches = array_vacío
    
    MIENTRAS haya entrada en directorio:
        SI nombre_archivo coincide con pattern:
            añadir a matches
    
    cerrar_directorio
    RETORNAR matches
```

---

### 5. 🟢 OPERADORES LÓGICOS (Bonus)
**Prioridad: BAJA (Bonus)**  
**Archivos a crear:**
- `src/parser/logical_ops.c` (nuevo)

**Operadores:**
```bash
cmd1 && cmd2     # Ejecutar cmd2 solo si cmd1 exitcode=0
cmd1 || cmd2     # Ejecutar cmd2 solo si cmd1 exitcode!=0
```

**Implementación:**
Necesitas detectar `&&` y `||` en el tokenizer y manejarlos en el executor.

---

### 6. 🟢 SUBSHELLS con paréntesis (Bonus avanzado)
**Prioridad: MUY BAJA**
```bash
(cd /tmp && ls)  # Ejecutar en proceso hijo sin afectar al padre
```

---

## 📊 CHECKLIST DE PROGRESO

### Parser
- [x] Tokenización básica
- [x] Detección de comillas simples/dobles
- [x] Expansión de variables `$VAR`
- [x] Expansión de `$?` y `$$`
- [x] Detección de redirecciones `<`, `>`, `>>`, `<<`
- [x] Detección de pipes `|`
- [ ] **Validación de sintaxis completa**
- [ ] **Pipes: crear lista enlazada de comandos**
- [ ] **Heredocs funcionales**
- [ ] Wildcards `*`
- [ ] Operadores lógicos `&&`, `||`
- [ ] Caracteres escapados `\`

### Executor
- [x] Ejecutar comandos simples
- [x] Builtins básicos (echo, cd, pwd, env, exit, export, unset)
- [x] Redirecciones básicas `<`, `>`
- [ ] **Redirecciones `>>` (append)**
- [ ] **Heredocs `<<`**
- [ ] **Pipeline completo (múltiples comandos con pipes)**
- [ ] Manejo de señales (Ctrl+C, Ctrl+D, Ctrl+\)

---

## 🎯 PLAN DE ACCIÓN RECOMENDADO

### Semana 1:
1. ✅ Implementar validación de sintaxis (1-2 días)
2. ✅ Testear validación exhaustivamente (1 día)

### Semana 2:
3. ✅ Implementar pipes completos en parser (2 días)
4. ✅ Implementar pipeline execution en executor (2-3 días)
5. ✅ Testear pipes (1 día)

### Semana 3:
6. ✅ Implementar heredocs (2-3 días)
7. ✅ Testear heredocs (1 día)
8. ✅ Fix bugs y memory leaks (1-2 días)

### Semana 4 (Bonus opcional):
9. Wildcards
10. Operadores lógicos
11. Refinamientos finales

---

## 🧪 CASOS DE PRUEBA IMPORTANTES

```bash
# Validación de sintaxis
|
ls |
| ls
ls | | cat
ls >
ls > > file
cat <<
cat << EOF

# Pipes
ls | cat
ls -l | grep test | wc -l
cat file.txt | grep hello | wc -l

# Heredocs
cat << EOF
hello
world
EOF

cat << 'EOF'
$USER should not expand
EOF

# Redirecciones complejas
cat < input.txt | grep test > output.txt
ls > file1.txt > file2.txt  # Solo file2 debe tener contenido
cat << EOF | grep hello
hello world
test
EOF

# Combinaciones
echo "hello" | cat | cat | cat
ls | > output.txt  # Error de sintaxis
```

---

## 📝 NOTAS IMPORTANTES

1. **Memory leaks**: Usar valgrind constantemente
2. **Norminette**: Verificar que cumple la norma de 42
3. **Exit codes**: Guardar y retornar correctamente `$?`
4. **Señales**: Manejar Ctrl+C, Ctrl+D, Ctrl+\ correctamente
5. **PATH**: Buscar ejecutables en las rutas del PATH
6. **Comillas**: Recordar que simples NO expanden, dobles SÍ expanden

---

## 🔗 ARCHIVOS DE REFERENCIA

- `PIPES_IMPLEMENTATION.md` - Guía detallada de pipes
- `valgrind.md` - Guía para detectar leaks
- `BUILTINS_TEST_REPORT.md` - Tests de builtins
- `VARIABLE_EXPANSION.md` - Tests de expansión

---

**Última actualización:** 25 de octubre de 2025  
**Estado actual:** Parser básico funcional, faltan pipes, validación y heredocs  
**Progreso estimado:** 60% completado

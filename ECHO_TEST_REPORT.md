# 📊 INFORME DE PRUEBAS: BUILTIN ECHO

**Fecha:** 31 de octubre de 2025  
**Total de tests:** 23  
**Tests pasados:** 7 ✅  
**Tests fallados:** 16 ❌  
**Memory leaks:** 0 🎉

---

## ✅ TESTS QUE FUNCIONAN CORRECTAMENTE (7/23)

| # | Comando | Resultado Esperado | Estado |
|---|---------|-------------------|--------|
| 1 | `echo hola` | `hola` | ✅ PASS |
| 2 | `echo ""` | `(línea vacía)` | ✅ PASS |
| 3 | `echo "" hola` | ` hola` | ✅ PASS |
| 11 | `echo "ho   $USER"  'la   $LESS'   $USER$TERM` | Expande variables | ✅ PASS |
| 16 | `"echo" "ho   $USER"` | Ejecuta echo con comillas | ✅ PASS |
| 18 | `       echo "ho   $USER"` | Con espacios al inicio | ✅ PASS |
| 19 | `echo "$HOME '$LESS' $USER"` | Comillas dentro de comillas | ✅ PASS |
| 20 | `echo '$HOME "$LESS" $USER'` | Variables sin expandir en single quotes | ✅ PASS |

---

## ❌ TESTS QUE FALLAN (16/23)

### 🔴 **Categoría 1: Flag `-n` no implementado** (4 tests)

El flag `-n` debe suprimir el newline al final del output.

| # | Comando | Comportamiento Actual | Comportamiento Esperado |
|---|---------|---------------------|------------------------|
| 4 | `echo -n hola` | Imprime con newline | `hola` (sin newline) |
| 5 | `echo -n -n hola` | Imprime con newline | `hola` (sin newline) |
| 6 | `echo -n -nh -n hola` | Imprime todo | `-nh -n hola` (primer -n válido, -nh no válido, para ahí) |
| 7 | `echo -nnnnnnnnn hola` | Imprime con newline | `hola` (sin newline) |

**Problema:** El builtin `echo` no procesa el flag `-n`.

**Solución requerida:**
```c
// En builtin_echo.c
// 1. Detectar si args[0] empieza con '-n'
// 2. Verificar que TODOS los caracteres después del '-' sean 'n'
// 3. Si válido, no imprimir newline al final
// 4. Continuar revisando siguientes args mientras sean válidos -n...
```

---

### 🔴 **Categoría 2: Parsing de comillas concatenadas** (4 tests)

Cuando hay comillas vacías o múltiples comillas concatenadas sin espacios.

| # | Comando | Problema |
|---|---------|----------|
| 8 | `echo $?hcudshfuewrf ew  wr 4ew""""` | No parsea correctamente `4ew""""` |
| 9 | `echo ""uhjkhnkj"hjbmb"'''' defdjhfkjb "iujiouhj"f'f'f'f'f''f'f'f'f'f'f'` | Separa tokens cuando debería concatenar |
| 10 | `echo ""aaaaaaaa"bbbbb"'''' ccccccccc "dddddddd"e'e'e` | Separa tokens cuando debería concatenar |
| 12 | `echo "ho   $USER"'la   $LESS'$USER$TERM` | No concatena comillas adyacentes |

**Ejemplo del problema:**
```bash
# En bash:
$ echo ""hola"mundo"
holamundo         # Todo concatenado

# En minishell actual:
$ echo ""hola"mundo"
 hola mundo       # Los separa con espacios
```

**Causa:** El parser trata cada string entre comillas como un token separado y añade espacios entre ellos.

**Solución requerida:** Cuando dos strings están adyacentes (sin espacio entre ellos), deben concatenarse en un solo argumento.

---

### 🔴 **Categoría 3: Comando sin espacio antes de comillas** (2 tests)

| # | Comando | Problema |
|---|---------|----------|
| 13 | `echo"ho   $USER"` | No reconoce `echo` como comando |
| 14 | `echo"ho   $USE$LESS"` | No reconoce `echo` como comando |

**Comportamiento esperado:** `echo"texto"` debería interpretarse como `echo` + `"texto"` (dos tokens).

**Problema actual:** Se parsea como un solo token `echo"ho   $USER"` que no se reconoce como comando.

**Solución requerida:** El lexer debe separar el nombre del comando de las comillas que vienen inmediatamente después.

---

### 🔴 **Categoría 4: Comando completo entre comillas** (2 tests)

| # | Comando | Problema |
|---|---------|----------|
| 15 | `"echo ho   $USER"` | No ejecuta nada (todo es un string) |
| 17 | `e"cho" "ho   $USER"` | No reconoce `e"cho"` como `echo` |

**Comportamiento esperado:**
- `"echo ho   $USER"` → Expandir `$USER`, luego ejecutar el string resultante como comando
- `e"cho"` → Concatenar a `echo` y usarlo como nombre de comando

**Problema actual:** Las comillas dentro del nombre del comando no se manejan.

**Solución requerida:** Eliminar comillas del nombre del comando antes de ejecutar.

---

### 🔴 **Categoría 5: Comillas mixtas complejas** (4 tests)

Casos con comillas simples y dobles alternadas de forma compleja.

| # | Comando | Problema |
|---|---------|----------|
| 21 | `echo "$HOME '$LESS "pa $TERM" na' $USER"` | No maneja comillas dobles dentro de comillas simples dentro de comillas dobles |
| 22 | `echo '$HOME "$LESS 'pa $TERM' na" $USER'` | Similar, pero al revés |
| 23 | `echo '$'"HOME"` | No concatena correctamente `'$'` + `HOME` |

**Comportamiento esperado en test 23:**
```bash
$ echo '$'"HOME"
$HOME              # '$' (literal) + HOME (literal) = $HOME
```

**Problema:** El parser no maneja correctamente el cambio entre tipos de comillas adyacentes.

**Solución requerida:** Implementar un state machine que alterne entre:
- Estado NONE: normal parsing
- Estado DOUBLE_QUOTE: dentro de `"`, expandir variables
- Estado SINGLE_QUOTE: dentro de `'`, todo literal
- Al cerrar comillas, si viene otra comilla inmediatamente, concatenar

---

## 🎉 MEMORIA: SIN LEAKS

**Todos los tests pasaron la verificación de valgrind:**
- ✅ 0 bytes definitely lost
- ✅ 0 bytes indirectly lost
- ✅ 0 bytes possibly lost

**Leak que se corrigió:**
- **Ubicación:** `init_first_cmd()` en `lexer.c`
- **Causa:** Double `ft_strdup()` del nombre del comando
- **Solución:** Eliminar el primer strdup, solo usar `create_cmd()` que ya hace la copia

---

## 📋 RESUMEN POR CATEGORÍA

| Categoría | Tests | Pasados | Fallados |
|-----------|-------|---------|----------|
| Básico (sin comillas) | 1 | 1 | 0 |
| Comillas vacías | 2 | 2 | 0 |
| Flag -n | 4 | 0 | 4 |
| Comillas concatenadas | 4 | 0 | 4 |
| Comando sin espacio | 2 | 0 | 2 |
| Comando entre comillas | 2 | 1 | 1 |
| Variables de entorno | 4 | 3 | 1 |
| Comillas mixtas complejas | 4 | 0 | 4 |
| **TOTAL** | **23** | **7** | **16** |

---

## 🔧 PRIORIDAD DE CORRECCIONES

### 🔥 **Alta prioridad:**
1. **Implementar flag `-n`** (4 tests) - Relativamente fácil
2. **Concatenar comillas adyacentes** (4 tests) - Medio
3. **Separar comando de comillas** (2 tests) - Medio

### ⚠️ **Media prioridad:**
4. **Comando entre comillas** (2 tests) - Difícil
5. **Comillas mixtas complejas** (4 tests) - Muy difícil

---

## 💡 RECOMENDACIONES

### Para flag `-n`:
```c
int has_n_flag(char **args)
{
    int i = 0;
    int has_flag = 0;
    
    while (args[i] && args[i][0] == '-')
    {
        int j = 1;
        int all_n = 1;
        
        while (args[i][j])
        {
            if (args[i][j] != 'n')
            {
                all_n = 0;
                break;
            }
            j++;
        }
        
        if (all_n && j > 1)  // Al menos -n
        {
            has_flag = 1;
            i++;
        }
        else
            break;
    }
    
    return has_flag ? i : 0;  // Retorna índice donde empiezan los args reales
}
```

### Para concatenar comillas:
- Modificar el lexer para que cuando cierra una comilla y la siguiente char es otra comilla (sin espacio), no cree un nuevo token sino que continúe en el mismo.

### Para comando entre comillas:
- Después de parsear, recorrer el comando y sus args
- Si el nombre del comando contiene comillas, removerlas
- Si un arg es `e"cho"`, convertir a `echo`

---

## ✅ CONCLUSIÓN

**Lo que funciona:**
- ✅ Echo básico
- ✅ Expansión de variables
- ✅ Comillas simples y dobles básicas
- ✅ Gestión de memoria perfecta (0 leaks)

**Lo que falta:**
- ❌ Flag `-n` (crítico para minishell)
- ❌ Concatenación de comillas adyacentes
- ❌ Casos edge de parsing

**Score actual: 30% (7/23)**  
**Memory: 100% (0 leaks)**

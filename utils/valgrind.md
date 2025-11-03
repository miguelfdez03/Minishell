# Valgrind Suppressions File

## ¿Qué es este archivo?

`valgrind.supp` es un archivo de **supresiones para Valgrind** que le indica a la herramienta que ignore ciertos memory leaks que provienen de **librerías externas** que no podemos controlar.

## ¿Por qué es necesario?

Cuando usas funciones de librerías externas como:
- **readline** (para leer input del usuario)
- **ncurses/tinfo** (usadas internamente por readline)
- **add_history** (para guardar el historial de comandos)

Estas librerías pueden tener memory leaks internos o memoria que no liberan hasta que el programa termina completamente. Estos leaks **no son culpa de tu código** y no afectan el funcionamiento del programa.

## Estructura del archivo

Cada bloque de supresión tiene este formato:

```
{
   nombre_de_la_supresion
   Memcheck:Leak
   ...
   fun:*nombre_funcion*
}
```

### Explicación de los bloques:

| Bloque | Descripción |
|--------|-------------|
| `readline_leak_1` y `readline_leak_2` | Suprime leaks de la librería readline |
| `ncurses_leak` | Suprime leaks de ncurses (usada por readline) |
| `tinfo_leak` | Suprime leaks de libtinfo (información de terminal) |
| `history_leak` | Suprime leaks del sistema de historial |
| `termcap_leak` | Suprime leaks de termcap (capacidades de terminal) |
| `libc_leak` | Suprime leaks de la librería estándar de C |
| `dl_leak` | Suprime leaks del dynamic linker |

## ¿Cómo ejecutar Valgrind con este archivo?

### Comando básico:

```bash
valgrind --suppressions=valgrind.supp --leak-check=full ./minishell
```

### Comando completo (recomendado):

```bash
valgrind --suppressions=valgrind.supp \
         --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./minishell
```

### Opciones explicadas:

| Opción | Descripción |
|--------|-------------|
| `--suppressions=valgrind.supp` | Usa el archivo de supresiones |
| `--leak-check=full` | Muestra detalles completos de los leaks |
| `--show-leak-kinds=all` | Muestra todos los tipos de leaks |
| `--track-origins=yes` | Rastrea el origen de valores no inicializados |

## Ejemplo de salida

### Sin archivo de supresiones:

```
HEAP SUMMARY:
    in use at exit: 204,169 bytes in 221 blocks
  total heap usage: 782 allocs, 274 frees, 244,188 bytes allocated

LEAK SUMMARY:
   definitely lost: 48 bytes in 1 blocks
   indirectly lost: 8,771 bytes in 286 blocks
     possibly lost: 0 bytes in 0 blocks
   still reachable: 204,169 bytes in 221 blocks  ← Leaks de readline
         suppressed: 0 bytes in 0 blocks
```

### Con archivo de supresiones:

```
HEAP SUMMARY:
    in use at exit: 204,169 bytes in 221 blocks
  total heap usage: 782 allocs, 274 frees, 244,188 bytes allocated

LEAK SUMMARY:
   definitely lost: 0 bytes in 0 blocks          ← Tu código está limpio
   indirectly lost: 0 bytes in 0 blocks
     possibly lost: 0 bytes in 0 blocks
   still reachable: 0 bytes in 0 blocks
         suppressed: 204,169 bytes in 221 blocks ← Leaks suprimidos
```

## ¿Cuándo usar este archivo?

✅ **SÍ usar** cuando:
- Estás probando tu código con Valgrind
- Quieres ver solo **tus** memory leaks
- Trabajas con readline, ncurses u otras librerías externas

❌ **NO usar** cuando:
- Estás debuggeando leaks específicos de readline (poco probable)
- Quieres ver **absolutamente todos** los leaks del sistema

## Notas importantes

1. Los leaks suprimidos **NO son errores de tu código**
2. Estos leaks son **normales** cuando usas readline
3. El archivo debe estar en el mismo directorio donde ejecutas valgrind
4. Si mueves el archivo, ajusta la ruta en el comando: `--suppressions=ruta/a/valgrind.supp`

## Verificar que funciona correctamente

Después de ejecutar con supresiones, deberías ver:

```bash
LEAK SUMMARY:
   definitely lost: 0 bytes in 0 blocks    ← Esto debe ser 0
   indirectly lost: 0 bytes in 0 blocks    ← Esto debe ser 0
```

Si ves bytes en "definitely lost" o "indirectly lost", son **memory leaks de tu código** que necesitas corregir.

## Makefile target (opcional)

Puedes añadir esto a tu Makefile para facilitar el testing:

```makefile
valgrind: $(NAME)
    valgrind --suppressions=valgrind.supp \
             --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             ./$(NAME)
```

Uso: `make valgrind`
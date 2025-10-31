# 📊 EXPORT BUILTIN - Reporte de Testing Exhaustivo

## 🎯 Resumen Ejecutivo

| Métrica | Valor |
|---------|-------|
| **Tests Totales** | 20 |
| **✅ Tests Pasados** | 0 |
| **❌ Tests Fallados** | 20 |
| **🎉 Sin Memory Leaks** | 20 (100%) |
| **Tasa de Éxito** | 0% |

## 📝 Estado General

El builtin `export` presenta **fallos críticos** en su funcionalidad básica. Aunque no hay memory leaks, la funcionalidad principal está completamente rota:

- ❌ **NO exporta variables al entorno**
- ❌ **NO muestra variables exportadas sin argumentos**
- ✅ **Validación de identificadores funciona**
- ✅ **Parsing de argumentos funciona**
- ✅ **Memory management perfecto**

---

## 🔍 Análisis Detallado de Tests

### 📊 Categoría 1: Asignación Básica de Variables (Tests 1-5)

#### ❌ Test 1: Asignar variable simple (`export a=b`)
```bash
# Comportamiento esperado (bash):
declare -x a="b"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: La variable se almacena internamente pero NO se exporta al entorno

---

#### ❌ Test 2: Variable numérica (`export MY_VAR=123`)
```bash
# Comportamiento esperado (bash):
declare -x MY_VAR="123"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Mismo que Test 1

---

#### ❌ Test 3: Variable con guiones bajos (`export MY_VAR_2=value`)
```bash
# Comportamiento esperado (bash):
declare -x MY_VAR_2="value"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Mismo que Test 1

---

#### ❌ Test 4: Sobrescribir variable (`export a=b` luego `export a=c`)
```bash
# Comportamiento esperado (bash):
declare -x a="c"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Sobrescritura interna funciona pero no se refleja en entorno

---

#### ❌ Test 5: Variable vacía (`export EMPTY=`)
```bash
# Comportamiento esperado (bash):
declare -x EMPTY=""

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Las variables vacías no se exportan correctamente

---

### 📊 Categoría 2: Sintaxis Especial y Edge Cases (Tests 6-10)

#### ❌ Test 6: Múltiples signos = (`export a=b=c=d`)
```bash
# Comportamiento esperado (bash):
declare -x a="b=c=d"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: El parsing correcto pero exportación no funciona

---

#### ❌ Test 7: Sin signo = (`export JUST_VAR`)
```bash
# Comportamiento esperado (bash):
declare -x JUST_VAR

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Declaración sin valor no se exporta

---

#### ❌ Test 8: Múltiples exports (`export VAR1=a VAR2=b VAR3=c`)
```bash
# Comportamiento esperado (bash):
declare -x VAR1="a"
declare -x VAR2="b"
declare -x VAR3="c"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: La iteración sobre múltiples args funciona pero exportación no

---

#### ❌ Test 9: Variable con espacios (`export "VAR WITH SPACES=value"`)
```bash
# Comportamiento esperado (bash):
bash: export: `VAR WITH SPACES=value': not a valid identifier

# Comportamiento actual (minishell):
export: `VAR WITH SPACES': not a valid identifier
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Error message correcto PERO formato diferente

---

#### ❌ Test 10: Comienza con número (`export 123VAR=value`)
```bash
# Comportamiento esperado (bash):
bash: export: `123VAR=value': not a valid identifier

# Comportamiento actual (minishell):
export: `123VAR': not a valid identifier
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Validación correcta pero formato de error diferente

---

### 📊 Categoría 3: Caracteres Especiales (Tests 11-15)

#### ❌ Test 11: Con comillas simples (`export VAR='hello world'`)
```bash
# Comportamiento esperado (bash):
declare -x VAR="hello world"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Parsing correcto pero no exporta

---

#### ❌ Test 12: Con comillas dobles (`export VAR="hello world"`)
```bash
# Comportamiento esperado (bash):
declare -x VAR="hello world"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Mismo que Test 11

---

#### ❌ Test 13: Path con slashes (`export PATH=/usr/bin:/bin`)
```bash
# Comportamiento esperado (bash):
declare -x PATH="/usr/bin:/bin"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Mismo que Test 11

---

#### ❌ Test 14: Con símbolos especiales (`export SPECIAL='!@#$%'`)
```bash
# Comportamiento esperado (bash):
declare -x SPECIAL="!@#$%"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Mismo que Test 11

---

#### ❌ Test 15: Expansión de variable (`export NEW=$OLD`)
```bash
# Comportamiento esperado (bash):
declare -x NEW="previous_value"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Expansión de variables no funciona correctamente

---

### 📊 Categoría 4: Sin Argumentos y Verificación (Tests 16-20)

#### ❌ Test 16: Listar variables sin args (`export`)
```bash
# Comportamiento esperado (bash):
declare -x COLORTERM="truecolor"
declare -x GTK_THEME="Sweet-Dark"
... (todas las variables exportadas)

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: export_without_args() no imprime nada

---

#### ❌ Test 17: Verificar con env (`export TEST=hello && env | grep TEST`)
```bash
# Comportamiento esperado (bash):
TEST=hello

# Comportamiento actual (minishell):
(sin output - variable no encontrada)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Variable no se exporta al entorno real

---

#### ❌ Test 18: Nombre de variable inválido con $ (`export $VAR=value`)
```bash
# Comportamiento esperado (bash):
bash: export: `testvalue=value': not a valid identifier

# Comportamiento actual (minishell):
export: `$VAR': not a valid identifier
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: No expande $VAR antes de validar (debería expandir primero)

---

#### ❌ Test 19: Múltiples assigns, verificar (`export A=1 B=2 && export | grep -E '(A|B)'`)
```bash
# Comportamiento esperado (bash):
declare -x A="1"
declare -x B="2"

# Comportamiento actual (minishell):
(sin output)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: Ni se exportan ni se listan las variables

---

#### ❌ Test 20: Caso combinado con error (`export 1=error VALID=ok`)
```bash
# Comportamiento esperado (bash):
bash: export: `1=error': not a valid identifier
declare -x VALID="ok"

# Comportamiento actual (minishell):
export: `1': not a valid identifier
(VALID no se exporta)
```
**Estado**: ❌ FAIL | ✅ NO LEAKS  
**Problema**: El loop continúa tras error pero no exporta las variables válidas

---

## 🐛 Análisis de Causa Raíz

### Problema Principal: `set_env_value()` no exporta al entorno real

**Localización**: `builtin_export.c:40-48` y utilidades de entorno

```c
static int	export_with_assignment(t_data *data, char *key, char *value)
{
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(key, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	set_env_value(&data->env, key, value);  // ⚠️ PROBLEMA AQUÍ
	return (0);
}
```

**Diagnóstico**:
1. `set_env_value()` solo actualiza la linked list interna (`data->env`)
2. **NO llama a `setenv()` o `putenv()` del sistema**
3. Las variables quedan en memoria pero no en el entorno real del proceso
4. Por eso `env` no las ve y los comandos externos no las heredan

### Problema Secundario: `export_without_args()` no imprime nada

```c
static int	export_without_args(t_env *env)
{
	t_env	*current;

	if (!env)
		return (0);
	current = env;
	while (current)
	{
		if (current->key)
		{
			if (current->value)
				ft_printf("declare -x %s=\"%s\"\n", current->key, current->value);
			else
				ft_printf("declare -x %s\n", current->key);
		}
		current = current->next;
	}
	return (0);
}
```

**Diagnóstico**:
- La función está bien implementada
- Pero si `data->env` no tiene variables exportadas, no imprime nada
- **El problema real**: Cuando minishell inicia, `init_env()` probablemente crea la lista pero no marca variables como "exportadas"

---

## 🔧 Soluciones Recomendadas

### ✅ Solución 1: Implementar exportación real al entorno (CRÍTICO)

**Archivo**: `src/utils/env_utils.c` (o donde esté `set_env_value()`)

```c
void	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_var;

	// Buscar si ya existe
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			current->is_exported = 1;  // Marcar como exportada
			setenv(key, value, 1);     // ⭐ AÑADIR ESTO
			return;
		}
		current = current->next;
	}

	// Si no existe, crear nueva
	new_var = create_env_node(key, value);
	new_var->is_exported = 1;  // Marcar como exportada
	add_env_node(env, new_var);
	setenv(key, value, 1);     // ⭐ AÑADIR ESTO
}
```

**Alternativa con `putenv()`** (si prefieren control manual):
```c
char *env_str = ft_strjoin(ft_strjoin(key, "="), value);
putenv(env_str);  // ⚠️ NO liberar env_str, putenv lo usa directamente
```

---

### ✅ Solución 2: Añadir flag `is_exported` en t_env

**Archivo**: `src/minishell.h`

```c
typedef struct s_env
{
	char		*key;
	char		*value;
	int			is_exported;  // ⭐ AÑADIR ESTO
	struct s_env	*next;
}	t_env;
```

**Propósito**:
- Distinguir entre variables declaradas y exportadas
- `export VAR` (sin =) marca como exportada pero sin valor
- `export VAR=value` exporta con valor

---

### ✅ Solución 3: Inicializar entorno desde `environ` correctamente

**Archivo**: `src/init_mini/init_env.c`

```c
void	init_env(t_data *data, char **envp)
{
	int		i;
	char	*key;
	char	*value;
	t_env	*new_node;

	data->env = NULL;
	i = 0;
	while (envp[i])
	{
		parse_env_string(envp[i], &key, &value);  // Separar KEY=VALUE
		new_node = create_env_node(key, value);
		new_node->is_exported = 1;  // ⭐ TODAS las variables de envp son exportadas
		add_env_node(&data->env, new_node);
		free(key);
		free(value);
		i++;
	}
}
```

---

### ✅ Solución 4: Ordenar variables alfabéticamente en `export` sin args

Bash imprime las variables ordenadas. Para hacerlo:

```c
static int	export_without_args(t_env *env)
{
	t_env	**sorted_array;
	int		count;
	int		i;

	count = count_env_nodes(env);
	sorted_array = malloc(sizeof(t_env *) * count);
	
	// Copiar punteros al array
	copy_env_to_array(env, sorted_array);
	
	// Ordenar alfabéticamente por key
	sort_env_array(sorted_array, count);  // Quicksort o similar
	
	// Imprimir ordenado
	i = 0;
	while (i < count)
	{
		if (sorted_array[i]->is_exported)  // Solo las exportadas
		{
			if (sorted_array[i]->value)
				ft_printf("declare -x %s=\"%s\"\n", sorted_array[i]->key, sorted_array[i]->value);
			else
				ft_printf("declare -x %s\n", sorted_array[i]->key);
		}
		i++;
	}
	free(sorted_array);
	return (0);
}
```

---

### ✅ Solución 5: Manejar `export VAR` sin asignación

```c
static int	process_export_arg(t_data *data, char *arg)
{
	char	*key;
	char	*value;
	int		status;

	if (!ft_strchr(arg, '='))  // ⭐ Caso sin asignación
	{
		if (!is_valid_identifier(arg))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (1);
		}
		mark_as_exported(&data->env, arg);  // Solo marcar, no cambiar valor
		return (0);
	}

	// Caso con asignación (código actual)
	parse_export_arg(arg, &key, &value);
	if (!key)
		return (1);
	status = export_with_assignment(data, key, value);
	free(key);
	if (value)
		free(value);
	return (status);
}
```

---

## 📋 Checklist de Implementación

### Fase 1: Funcionalidad Básica (Crítico)
- [ ] Implementar `setenv()` en `set_env_value()`
- [ ] Añadir campo `is_exported` a `t_env`
- [ ] Modificar `init_env()` para marcar variables como exportadas
- [ ] Verificar que `export VAR=value` funciona con tests 1-5
- [ ] Verificar que `env` muestra las variables exportadas

### Fase 2: Export sin Argumentos
- [ ] Verificar que `export_without_args()` imprime la lista
- [ ] Implementar ordenamiento alfabético (opcional)
- [ ] Filtrar solo variables con `is_exported = 1`

### Fase 3: Edge Cases
- [ ] Manejar `export VAR` sin = (test 7)
- [ ] Manejar múltiples variables en un comando (test 8)
- [ ] Manejar variables con comillas (tests 11-12)
- [ ] Expansión de variables en valores (test 15)

### Fase 4: Validación
- [ ] Mejorar formato de error messages
- [ ] Asegurar que continúa tras error (test 20)
- [ ] Verificar comportamiento con nombres inválidos (tests 9-10, 18)

### Fase 5: Testing Final
- [ ] Re-ejecutar `./test_export.sh`
- [ ] Verificar que los 20 tests pasan
- [ ] Verificar 0 memory leaks con valgrind

---

## 💡 Prioridad de Fixes

| Prioridad | Problema | Tests Afectados | Impacto |
|-----------|----------|-----------------|---------|
| 🔴 **CRÍTICO** | No exporta al entorno real | 1-8, 11-15, 17, 19-20 | 18/20 tests |
| 🟠 **ALTO** | `export` sin args no imprime | 16, 19 | 2/20 tests |
| 🟡 **MEDIO** | Formato de error messages | 9, 10, 18, 20 | 4/20 tests |
| 🟢 **BAJO** | Ordenamiento alfabético | 16 | 1/20 tests |

---

## 🎯 Pasos Inmediatos

1. **PRIMERO**: Implementar `setenv()` en `set_env_value()`
2. **SEGUNDO**: Añadir `is_exported` field a `t_env`
3. **TERCERO**: Probar con `export TEST=hello && env | grep TEST`
4. **CUARTO**: Una vez funcione, re-ejecutar `./test_export.sh`
5. **QUINTO**: Fix de edge cases según resultados

---

## 📊 Estimación de Tiempo

- ✅ Solución 1 (setenv): **30 minutos**
- ✅ Solución 2 (is_exported): **15 minutos**
- ✅ Solución 3 (init_env): **20 minutos**
- ✅ Solución 4 (ordenamiento): **45 minutos** (opcional)
- ✅ Solución 5 (VAR sin =): **20 minutos**

**Total estimado**: 1.5 - 2 horas para tener funcionalidad completa

---

## ✅ Conclusión

El builtin `export` tiene una **implementación parcial correcta**:
- ✅ Parsing de argumentos funciona
- ✅ Validación de identificadores funciona
- ✅ Memory management perfecto
- ❌ **NO exporta variables al entorno real del sistema**

**Fix principal**: Añadir `setenv(key, value, 1)` en `set_env_value()` para que las variables sean accesibles a comandos externos y al builtin `env`.

Con este fix, se espera que **todos los 20 tests pasen** excepto detalles menores de formato.

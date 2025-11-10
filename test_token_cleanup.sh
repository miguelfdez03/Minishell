#!/bin/bash

echo "=== Test: Verificación de limpieza de tokens después de error de sintaxis ==="
echo ""

echo "Test 1: Error de sintaxis seguido de comando válido"
result=$(printf "echo ||\necho test\nexit\n" | ./minishell 2>&1)
if echo "$result" | grep -q "syntax error" && echo "$result" | grep -q "test"; then
    echo "✓ PASSED - El comando válido se ejecutó después del error"
else
    echo "✗ FAILED - El comando válido NO se ejecutó"
fi

echo ""
echo "Test 2: Múltiples errores intercalados con comandos válidos"
result=$(printf "cat >\necho hola\nls |\necho mundo\nexit\n" | ./minishell 2>&1)
error_count=$(echo "$result" | grep -c "syntax error")
valid_count=$(echo "$result" | grep -c "hola\|mundo")

if [ "$error_count" -eq 2 ] && [ "$valid_count" -eq 2 ]; then
    echo "✓ PASSED - 2 errores detectados y 2 comandos válidos ejecutados"
else
    echo "✗ FAILED - Error count: $error_count (esperado 2), Valid count: $valid_count (esperado 2)"
fi

echo ""
echo "Test 3: cd después de error de sintaxis"
result=$(printf "echo ||\ncd ..\npwd\nexit\n" | ./minishell 2>&1)
if echo "$result" | grep -q "syntax error" && ! echo "$result" | tail -n 3 | grep -q "syntax error"; then
    echo "✓ PASSED - cd se ejecutó correctamente después del error"
else
    echo "✗ FAILED - cd heredó el error de sintaxis"
fi

echo ""
echo "=== Tests completados ==="

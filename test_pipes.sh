#!/bin/bash

# Script de pruebas para verificar el sistema de pipes

echo "🧪 ====== TEST DE SISTEMA DE PIPES ======"
echo ""

# Crear archivo temporal para entrada
TEST_FILE="/tmp/minishell_test_input.txt"

echo "📋 Test 1: Comando simple sin pipes"
echo "echo hello" > $TEST_FILE
./minishell < $TEST_FILE
echo ""

echo "📋 Test 2: Pipe con 2 comandos"
echo "ls | grep test" > $TEST_FILE
./minishell < $TEST_FILE
echo ""

echo "📋 Test 3: Pipe con 3 comandos"
echo "ls | grep c | wc -l" > $TEST_FILE
./minishell < $TEST_FILE
echo ""

echo "📋 Test 4: Comando con argumentos antes del pipe"
echo "echo hello world | cat" > $TEST_FILE
./minishell < $TEST_FILE
echo ""

echo "📋 Test 5: Múltiples argumentos en cada comando"
echo "echo uno dos tres | grep dos | cat" > $TEST_FILE
./minishell < $TEST_FILE
echo ""

# Limpiar
rm -f $TEST_FILE

echo "✅ Tests completados"

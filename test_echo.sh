#!/bin/bash

# Script de pruebas exhaustivas para echo
# Compara comportamiento de minishell vs bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

TEST_NUM=1
PASSED=0
FAILED=0

# Función para ejecutar un test
run_test() {
    local cmd="$1"
    local test_name="Test $TEST_NUM: $cmd"
    
    echo -e "\n${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${YELLOW}$test_name${NC}"
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    
    # Ejecutar en bash
    echo -e "${GREEN}BASH:${NC}"
    bash_output=$(echo "$cmd" | bash 2>&1)
    echo "$bash_output"
    
    # Ejecutar en minishell
    echo -e "\n${GREEN}MINISHELL:${NC}"
    mini_output=$(echo "$cmd" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "exit")
    echo "$mini_output"
    
    # Comparar outputs
    if [ "$bash_output" = "$mini_output" ]; then
        echo -e "\n${GREEN}✅ PASS: Output coincide${NC}"
        ((PASSED++))
    else
        echo -e "\n${RED}❌ FAIL: Output diferente${NC}"
        ((FAILED++))
    fi
    
    # Verificar memory leaks con valgrind
    echo -e "\n${YELLOW}Verificando memory leaks...${NC}"
    leak_output=$(echo "$cmd" | valgrind --leak-check=full --suppressions=valgrind.supp ./minishell 2>&1 | grep "definitely lost")
    
    if echo "$leak_output" | grep -q "definitely lost: 0 bytes"; then
        echo -e "${GREEN}✅ NO LEAKS${NC}"
    elif [ -z "$leak_output" ]; then
        echo -e "${GREEN}✅ NO LEAKS${NC}"
    else
        echo -e "${RED}⚠️  MEMORY LEAK DETECTADO:${NC}"
        echo "$leak_output"
    fi
    
    ((TEST_NUM++))
}

echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║           ECHO TEST SUITE - MINISHELL                 ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"

# Tests básicos
run_test "echo hola"
run_test 'echo ""'
run_test 'echo "" hola'

# Tests con flag -n
run_test "echo -n hola"
run_test "echo -n -n hola"
run_test "echo -n -nh -n hola"
run_test "echo -nnnnnnnnn hola"

# Tests con variables y caracteres especiales
run_test 'echo $?hcudshfuewrf ew  wr 4ew""""'
run_test 'echo ""uhjkhnkj"hjbmb"'"'"''"'"''"'"''"'"' defdjhfkjb "iujiouhj"f'"'"'f'"'"'f'"'"'f'"'"'f'"'"''"'"'f'"'"'f'"'"'f'"'"'f'"'"'f'"'"'f'"'"

# Tests con comillas mixtas
run_test 'echo ""uhjkhnkj"hjbmb"'"'"''"'"''"'"''"'"' defdjhfkjb "iujiouhj"f'"'"'f'"'"'a'"'"'j'"'"'o'"'"''"'"'i'"'"'w'"'"'q'"'"''"'"'q'
run_test 'echo ""aaaaaaaa"bbbbb"'"'"''"'"''"'"''"'"' ccccccccc "dddddddd"e'"'"'e'"'"'e'

# Tests con variables de entorno
run_test 'echo "ho   $USER"  '"'"'la   $LESS'"'"'   $USER$TERM'
run_test 'echo "ho   $USER"'"'"'la   $LESS'"'"'$USER$TERM'

# Tests sin espacios
run_test 'echo"ho   $USER"'
run_test 'echo"ho   $USE$LESS"'

# Tests con comando entre comillas
run_test '"echo ho   $USER"'
run_test '"echo" "ho   $USER"'
run_test 'e"cho" "ho   $USER"'

# Tests con espacios al inicio
run_test '       echo "ho   $USER"'

# Tests con comillas mixtas complejas
run_test 'echo "$HOME '"'"'$LESS'"'"' $USER"'
run_test 'echo '"'"'$HOME "$LESS" $USER'"'"
run_test 'echo "$HOME '"'"'$LESS "pa $TERM" na'"'"' $USER"'
run_test 'echo '"'"'$HOME "$LESS '"'"'pa $TERM'"'"' na" $USER'"'"
run_test 'echo '"'"'$'"'"'"HOME"'

# Resumen
echo -e "\n${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║                   RESUMEN FINAL                        ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo -e "${GREEN}Tests pasados: $PASSED${NC}"
echo -e "${RED}Tests fallados: $FAILED${NC}"
echo -e "${YELLOW}Total tests: $((TEST_NUM - 1))${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ¡TODOS LOS TESTS PASARON! 🎉${NC}"
else
    echo -e "\n${RED}⚠️  Hay tests que necesitan corrección${NC}"
fi

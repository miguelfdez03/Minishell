#!/bin/bash

# Script simplificado de pruebas

echo "🧪 TESTS DE ECHO - RESUMEN RÁPIDO"
echo "=================================="
echo ""

test_cmd() {
    local num="$1"
    local cmd="$2"
    
    bash_out=$(echo "$cmd" | bash 2>&1)
    mini_out=$(echo "$cmd" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "^exit$" | grep -v "^$")
    
    if [ "$bash_out" = "$mini_out" ]; then
        result="✅ PASS"
    else
        result="❌ FAIL"
    fi
    
    # Check leaks (solo buscar "definitely lost: 0")
    leak_check=$(echo "$cmd" | valgrind --leak-check=full --suppressions=valgrind.supp ./minishell 2>&1 | grep "definitely lost:")
    if echo "$leak_check" | grep -q "definitely lost: 0 bytes"; then
        leak="✅ NO LEAK"
    else
        leak="⚠️  LEAK"
    fi
    
    printf "%-3s %-50s %s %s\n" "$num" "$cmd" "$result" "$leak"
}

echo "Test | Comando | Resultado | Memory"
echo "-----+----------+-----------+--------"

test_cmd "1"  "echo hola"
test_cmd "2"  "echo \"\""
test_cmd "3"  "echo \"\" hola"
test_cmd "4"  "echo -n hola"
test_cmd "5"  "echo -n -n hola"
test_cmd "6"  "echo -n -nh -n hola"
test_cmd "7"  "echo -nnnnnnnnn hola"
test_cmd "8"  "echo \$?hcudshfuewrf ew  wr 4ew\"\"\"\""
test_cmd "9"  "echo \"\"uhjkhnkj\"hjbmb\"'''' defdjhfkjb \"iujiouhj\"f'f'f'f'f''f'f'f'f'f'f'"
test_cmd "10" "echo \"\"aaaaaaaa\"bbbbb\"'''' ccccccccc \"dddddddd\"e'e'e"
test_cmd "11" "echo \"ho   \$USER\"  'la   \$LESS'   \$USER\$TERM"
test_cmd "12" "echo \"ho   \$USER\"'la   \$LESS'\$USER\$TERM"
test_cmd "13" "echo\"ho   \$USER\""
test_cmd "14" "echo\"ho   \$USE\$LESS\""
test_cmd "15" "\"echo ho   \$USER\""
test_cmd "16" "\"echo\" \"ho   \$USER\""
test_cmd "17" "e\"cho\" \"ho   \$USER\""
test_cmd "18" "       echo \"ho   \$USER\""
test_cmd "19" "echo \"\$HOME '\$LESS' \$USER\""
test_cmd "20" "echo '\$HOME \"\$LESS\" \$USER'"
test_cmd "21" "echo \"\$HOME '\$LESS \"pa \$TERM\" na' \$USER\""
test_cmd "22" "echo '\$HOME \"\$LESS 'pa \$TERM' na\" \$USER'"
test_cmd "23" "echo '\$'\"HOME\""

echo ""
echo "=================================="

#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

MINISHELL="./minishell"
TEST_COUNT=0

print_section() {
    echo -e "\n${BLUE}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║  $1${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════════════════════════╝${NC}\n"
}

test_cmd() {
    ((TEST_COUNT++))
    echo -e "${CYAN}[$TEST_COUNT]${NC} ${YELLOW}Testing:${NC} ${GREEN}$1${NC}"
    echo -e "$2\nexit" | $MINISHELL 2>&1 | grep -v "spidershell>" | grep -v "^exit$" | head -n 20
    echo ""
}

# ==================== ECHO TESTS ====================
print_section "ECHO TESTS"

test_cmd "echo" "echo"
test_cmd "echo hola" "echo hola"
test_cmd 'echo ""' 'echo ""'
test_cmd 'echo "" hola' 'echo "" hola'
test_cmd "echo -n hola" "echo -n hola"
test_cmd "echo -n -n hola" "echo -n -n hola"
test_cmd "echo -n -nh -n hola" "echo -n -nh -n hola"
test_cmd "echo -nnnnnnnnn hola" "echo -nnnnnnnnn hola"
test_cmd 'echo complex' 'echo ""aaaaaaaa"bbbbb"'"'"''"'"''"'"''"'"' ccccccccc "dddddddd"e'"'"'e'"'"'e'
test_cmd 'echo with vars' 'echo "ho   $USER"  '"'"'la   $LESS'"'"'   $USER$TERM'
test_cmd 'echo"no space"' 'echo"ho   $USER"'
test_cmd 'quoted echo 1' '"echo" "ho   $USER"'
test_cmd 'quoted echo 2' 'e"cho" "ho   $USER"'
test_cmd 'echo spaces' '       echo "ho   $USER"'

# ==================== EXPORT TESTS ====================
print_section "EXPORT TESTS"

test_cmd "export a=b" "export a=b\nenv | grep ^a="
test_cmd "export override" "export b=wawa\nexport b=lala\nenv | grep ^b="
test_cmd "export c=lala=kaka" "export c=lala=kaka\nenv | grep ^c="
test_cmd "export a (no value)" "export a\nenv | grep ^a="
test_cmd "export a= (empty)" "export a=\nenv | grep ^a="
test_cmd "export a===" "export a===\nenv | grep ^a="
test_cmd "export a3=l" "export a3=l\nenv | grep ^a3="
test_cmd "export 2a=lala (invalid)" "export 2a=lala"
test_cmd "export !a=ma (invalid)" "export !a=ma"
test_cmd "export (list all)" "export | head -n 5"

# ==================== CD TESTS ====================
print_section "CD TESTS"

test_cmd "cd to /tmp" "cd /tmp\npwd"
test_cmd "cd (go HOME)" "cd\npwd"
test_cmd "cd ~ (HOME)" "cd ~\npwd"
test_cmd "cd nonexistent" "cd /nonexistent_dir_test_123"
test_cmd "cd -" "cd /tmp\ncd /\ncd -\npwd"

# ==================== ENV TESTS ====================
print_section "ENV TESTS"

test_cmd "env" "env | head -n 5"
test_cmd "env with args (fail)" "env lalal"

# ==================== PWD TESTS ====================
print_section "PWD TESTS"

test_cmd "pwd" "pwd"
test_cmd "pwd with args" "pwd arg1 arg2"

# ==================== UNSET TESTS ====================
print_section "UNSET TESTS"

test_cmd "unset (no args)" "unset"
test_cmd "unset TEST" "export TEST=valor123\nenv | grep ^TEST=\nunset TEST\nenv | grep ^TEST="
test_cmd "unset multiple" "export A=1 B=2 C=3\nunset A B C\nenv | grep -E '^(A=|B=|C=)'"

# ==================== EXIT TESTS ====================
print_section "EXIT TESTS"

echo -e "${CYAN}[$(( ++TEST_COUNT ))]${NC} ${YELLOW}Testing:${NC} ${GREEN}exit${NC}"
echo "exit" | $MINISHELL
echo "Exit code: $?"
echo ""

echo -e "${CYAN}[$(( ++TEST_COUNT ))]${NC} ${YELLOW}Testing:${NC} ${GREEN}exit 0${NC}"
echo "exit 0" | $MINISHELL
echo "Exit code: $?"
echo ""

echo -e "${CYAN}[$(( ++TEST_COUNT ))]${NC} ${YELLOW}Testing:${NC} ${GREEN}exit 1${NC}"
echo "exit 1" | $MINISHELL
echo "Exit code: $?"
echo ""

echo -e "${CYAN}[$(( ++TEST_COUNT ))]${NC} ${YELLOW}Testing:${NC} ${GREEN}exit 42${NC}"
echo "exit 42" | $MINISHELL
echo "Exit code: $?"
echo ""

echo -e "${CYAN}[$(( ++TEST_COUNT ))]${NC} ${YELLOW}Testing:${NC} ${GREEN}exit lalal${NC}"
echo "exit lalal" | $MINISHELL
echo "Exit code: $?"
echo ""

# ==================== PIPE TESTS ====================
print_section "PIPE TESTS"

test_cmd "ls | wc" "ls | wc"
test_cmd "ls|wc (no space)" "ls|wc"
test_cmd "echo | cat | wc" "echo hola | cat | wc"
test_cmd "ls || wc (syntax)" "ls || wc"
test_cmd "ls |   | wc (empty)" "ls |   | wc"
test_cmd "ls | wc | (trailing)" "ls | wc |"
test_cmd "| (alone)" "|"
test_cmd "| ls | wc (leading)" "| ls | wc"

# ==================== REDIRECTION TESTS ====================
print_section "REDIRECTION TESTS"

# Create temp dir
TMP_DIR="test_redir_$$"
mkdir -p "$TMP_DIR"
echo -e "red\nblue\ngreen" > "$TMP_DIR/colors.txt"

test_cmd "echo > file" "echo hola > $TMP_DIR/out1.txt\ncat $TMP_DIR/out1.txt"
test_cmd "echo >> file" "echo hola >> $TMP_DIR/out2.txt\necho mundo >> $TMP_DIR/out2.txt\ncat $TMP_DIR/out2.txt"
test_cmd "echo >>>>>>> (multi)" "echo hola >>>>>>> $TMP_DIR/out3.txt"
test_cmd "echo hola>file (nospace)" "echo hola>$TMP_DIR/out4.txt\ncat $TMP_DIR/out4.txt"
test_cmd "wc < file" "wc < $TMP_DIR/colors.txt"
test_cmd "wc < file > out" "wc < $TMP_DIR/colors.txt > $TMP_DIR/out5.txt\ncat $TMP_DIR/out5.txt"
test_cmd "cat | wc" "cat $TMP_DIR/colors.txt | wc"
test_cmd "heredoc <<EOF" "cat << EOF\nhola\nmundo\nEOF"
test_cmd "multiple cat pipes" "echo hola | cat | cat | cat"

rm -rf "$TMP_DIR"

# ==================== GENERAL TESTS ====================
print_section "GENERAL TESTS"

echo -e "${CYAN}[$(( ++TEST_COUNT ))]${NC} ${YELLOW}Testing:${NC} ${GREEN}empty input${NC}"
echo -e "\n\nexit" | $MINISHELL 2>&1 | head -n 5
echo ""

echo -e "${CYAN}[$(( ++TEST_COUNT ))]${NC} ${YELLOW}Testing:${NC} ${GREEN}only spaces${NC}"
echo -e "     \n     \nexit" | $MINISHELL 2>&1 | head -n 5
echo ""

echo -e "${CYAN}[$(( ++TEST_COUNT ))]${NC} ${YELLOW}Testing:${NC} ${GREEN}unclosed quotes${NC}"
echo -e '"hola\nexit' | $MINISHELL 2>&1 | head -n 5
echo ""

test_cmd "echo \$?" "ls\necho \$?"
test_cmd "unset PATH; ls" "unset PATH\nls"
test_cmd "unset PATH; /bin/ls" "unset PATH\n/bin/ls | head -n 3"
test_cmd "unset HOME; cd" "unset HOME\ncd"

# ==================== EXTERNAL COMMANDS ====================
print_section "EXTERNAL COMMANDS"

test_cmd "ls" "ls | head -n 3"
test_cmd "ls -la" "ls -la | head -n 3"
test_cmd "/bin/echo" "/bin/echo hola mundo"
test_cmd "cat /etc/hostname" "cat /etc/hostname"
test_cmd "whoami" "whoami"
test_cmd "date" "date"
test_cmd "command not found" "comando_que_no_existe_123"

# ==================== SUMMARY ====================
echo -e "\n${GREEN}╔══════════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║  TESTS COMPLETED: $TEST_COUNT tests executed${NC}"
echo -e "${GREEN}╚══════════════════════════════════════════════════════════════╝${NC}\n"

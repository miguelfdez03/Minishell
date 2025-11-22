# Manual Test Cases for Minishell

## 📋 Table of Contents
1. [Compilation Tests](#compilation-tests)
2. [Echo Tests](#echo-tests)
3. [Export Tests](#export-tests)
4. [CD Tests](#cd-tests)
5. [Environment Tests](#environment-tests)
6. [PWD Tests](#pwd-tests)
7. [Unset Tests](#unset-tests)
8. [Exit Tests](#exit-tests)
9. [Return Value Tests](#return-value-tests)
10. [Signal Tests](#signal-tests)
11. [Pipe Tests](#pipe-tests)
12. [Redirection Tests](#redirection-tests)
13. [Quote Tests](#quote-tests)
14. [Path Tests](#path-tests)
15. [Edge Cases](#edge-cases)

---

## Compilation Tests

### Test 1: Compilation Flags
```bash
make -n | grep -E "Wall|Wextra|Werror"
```
**Expected:** Should show all three flags

### Test 2: No Relink
```bash
make
make
```
**Expected:** Second make should say "Nothing to be done" or "up to date"

### Test 3: Clean Compilation
```bash
make fclean
make
```
**Expected:** No errors, creates minishell executable

---

## Echo Tests

### Basic Echo
```bash
echo
echo hola
echo ""
echo "" hola
```

### Echo with -n flag
```bash
echo -n hola
echo -n -n hola
echo -n -nh -n hola
echo -nnnnnnnnn hola
```

### Echo with quotes and variables
```bash
echo "ho   $USER"  'la   $LESS'   $USER$TERM
echo "ho   $USER"'la   $LESS'$USER$TERM
echo "$HOME '$LESS' $USER"
echo '$HOME "$LESS" $USER'
echo '$'"HOME"
```

### Complex echo cases
```bash
echo ""uhjkhnkj"hjbmb"'''' defdjhfkjb "iujiouhj"f'f'f'f'f''f'f'f'f'f'f'
echo ""aaaaaaaa"bbbbb"'''' ccccccccc "dddddddd"e'e'e
```

### Echo command variations
```bash
echo"ho   $USER"
"echo" "ho   $USER"
e"cho" "ho   $USER"
       echo "ho   $USER"
```

---

## Export Tests

### Basic Export
```bash
export a=b
export
export b=wawa
export b=lala
export
```

### Export with special values
```bash
export c=lala=kaka
export a
export a=
export a= b=o
export a===
```

### Export with invalid identifiers
```bash
export a3=l
export 2a=lala
export !a=ma
```

### Export with variables
```bash
export $USER=la
export $USER=$TERM
export $USER=$TERM$HOME
export $USER=$TERM$HOM
export $HOME=$USER
export $USER$SHLVL
```

### Export edge cases
```bash
export
           export
export ""
export a b=$LESS $USER $HOME=la $COLORTERM=pa c=d
```

---

## CD Tests

### Basic CD
```bash
mkdir /tmp/test_dir
cd /tmp/test_dir
pwd
cd /tmp/nonexistent
cd
           cd
```

### CD with special paths
```bash
cd -         # Should go to OLDPWD
cd ~         # Should go to HOME
cd ..
cd .
cd ../..
```

### CD without HOME
```bash
unset HOME
cd
```

### CD with permissions
```bash
mkdir /tmp/no_exec
chmod 000 /tmp/no_exec
cd /tmp/no_exec
chmod 755 /tmp/no_exec
rmdir /tmp/no_exec
```

---

## Environment Tests

### ENV command
```bash
env
env lalal
env /tmp
```

### ENV after modifications
```bash
export NEW_VAR=test
env | grep NEW_VAR
unset NEW_VAR
env | grep NEW_VAR
```

---

## PWD Tests

```bash
pwd
pwd arg1 arg2
pwd -L
cd /tmp
pwd
cd ..
pwd
```

---

## Unset Tests

### Basic unset
```bash
export TEST=hello
echo $TEST
unset TEST
echo $TEST
```

### Multiple unset
```bash
export A=1 B=2 C=3
unset A B C
env | grep -E "^A=|^B=|^C="
```

### Unset without arguments
```bash
unset
```

---

## Exit Tests

```bash
exit
exit 0
exit 1
exit 42
exit 255
exit 256
exit -1
exit lalal
exit 1 2 3
```

---

## Return Value Tests

### Test $?
```bash
/bin/ls
echo $?
/bin/ls /nonexistent
echo $?
true
echo $?
false
echo $?
```

### Complex $? usage
```bash
echo $?
echo $?$?
echo $?hola
```

---

## Signal Tests

### CTRL+C (SIGINT)
1. Launch minishell
2. Press `Ctrl+C` → Should show new prompt
3. Type something, then press `Ctrl+C` → Should clear line and show new prompt
4. Run `cat` (blocking), then press `Ctrl+C` → Should interrupt and show new prompt

### CTRL+D (EOF)
1. Launch minishell
2. Press `Ctrl+D` → Should exit minishell
3. Launch minishell
4. Type something, then press `Ctrl+D` → Should not do anything
5. Run `cat` (blocking), then press `Ctrl+D` → Should send EOF to cat

### CTRL+\ (SIGQUIT)
1. Launch minishell
2. Press `Ctrl+\` → Should not do anything
3. Type something, then press `Ctrl+\` → Should not do anything
4. Run `cat` (blocking), then press `Ctrl+\` → Should quit with core dump message

---

## Pipe Tests

### Simple pipes
```bash
ls | wc
ls|wc
echo hola | cat
cat /etc/passwd | grep root
```

### Multiple pipes
```bash
ls | wc | cat
cat /etc/passwd | grep root | wc -l
echo "a b c" | tr ' ' '\n' | sort | uniq
```

### Pipe with redirections
```bash
echo hola | cat > file
cat file | wc > file2
cat < file | cat > file2
```

### Syntax errors with pipes
```bash
ls ||
ls |   | wc
ls |
|
| ls
```

### Many pipes
```bash
echo hola | cat | cat | cat | cat | cat | cat | cat
```

---

## Redirection Tests

### Output redirections
```bash
echo hola > file
echo mundo >> file
cat file
echo test >>>>>>> file
```

### Input redirections
```bash
cat < /etc/passwd
wc < /etc/passwd
wc -l < /etc/passwd
```

### Multiple redirections
```bash
echo hola > file1 > file2 > file3
cat < file1 > file2
echo test > file < /etc/passwd
```

### Heredoc tests
```bash
cat << EOF
hello
world
EOF

cat << EOF > file
test
content
EOF

<< EOF

cat << hola << que << tal
```

### Heredoc with signals
```bash
cat << EOF
# Press Ctrl+C here
# Press Ctrl+D here
```

### Heredoc with variables
```bash
cat << EOF
$USER
$HOME
EOF

cat << "EOF"
$USER
$HOME
EOF
```

### Syntax errors
```bash
echo hola </<</>/>> file
echo hola > | file
< < file
> > file
```

---

## Quote Tests

### Single quotes
```bash
echo 'hello world'
echo '$USER'
echo 'cat file | grep test'
echo '< > | << >>'
```

### Double quotes
```bash
echo "hello world"
echo "$USER"
echo "ho   $USER   la"
echo "test | pipe"
```

### Mixed quotes
```bash
echo "ho   $USER"'la   $LESS'
echo '$HOME '"$USER"' $PWD'
echo '"$USER"'
echo "'$USER'"
```

### Unclosed quotes (syntax error)
```bash
echo "unclosed
echo 'unclosed
echo "mix'
```

---

## Path Tests

### Absolute paths
```bash
/bin/ls
/bin/echo hola
/usr/bin/wc /etc/passwd
```

### Relative paths
```bash
./minishell
../minishell
../../bin/ls
```

### Commands from PATH
```bash
ls
echo hola
cat /etc/passwd
wc -l /etc/passwd
```

### PATH modification
```bash
export PATH=/bin:/usr/bin
ls
export PATH=/tmp
ls
/bin/ls
```

### Unset PATH
```bash
unset PATH
ls               # Should fail
/bin/ls          # Should work
echo $PATH       # Should be empty
```

---

## Edge Cases

### Empty input
```bash
# Just press Enter

```

### Only spaces/tabs
```bash
    
		
   	   
```

### Non-existent commands
```bash
dsbksdgbksdghsd
invalidcommand123
./nonexistent
```

### Long commands
```bash
echo a b c d e f g h i j k l m n o p q r s t u v w x y z 1 2 3 4 5 6 7 8 9 0
```

### Special characters
```bash
echo ';'
echo '\'
echo '&&'
echo '*'
echo '?'
```

### Stress tests
```bash
echo $EMPTY echo hi
export EMPTY=
echo $EMPTY echo hi
$EMPTY echo hi
```

### Command history
```bash
# Type: echo first
# Press Up arrow
# Type: echo second
# Press Up arrow twice
# Should navigate through history
```

---

## 🎯 Expected Behaviors Summary

### Signals
- **Ctrl+C**: Show new prompt, clear current line
- **Ctrl+D**: Exit shell (when line is empty)
- **Ctrl+\**: Do nothing (in interactive shell)

### Quotes
- **Single quotes**: No expansion, preserve everything literally
- **Double quotes**: Expand variables, preserve spaces
- **Mixed quotes**: Each section respects its own rules

### Pipes
- Connect stdout of left command to stdin of right command
- Multiple pipes create pipeline
- Syntax errors: `||`, `|` at start/end, `| |`

### Redirections
- `>`: Truncate and write
- `>>`: Append
- `<`: Read from file
- `<<`: Read until delimiter (heredoc)
- Multiple redirections: Last one wins for output

### Exit codes
- 0: Success
- 1-255: Various errors
- 127: Command not found
- 126: Permission denied
- 130: Terminated by Ctrl+C

### Builtins
- `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Must work without PATH
- Should be executed in current shell (no fork)

---

## 📝 Notes

- Always compare behavior with bash when in doubt
- Test both success and failure cases
- Check exit codes with `echo $?`
- Test with and without PATH
- Test with various combinations of quotes
- Test signal handling in different states
- Test memory leaks with valgrind

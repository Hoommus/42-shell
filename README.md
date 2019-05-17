_Just gonna pretend somebody will ever need this..._

## Overview

This is an educational project for School 42, the goal of which is to
implement a real Unix shell. In my shell specifically I'm doing my best
to comply with
[The Open Group Base Specification](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
for Shell Language. When I finish this project, it should be fully
compatible with Bash Script both in syntax and behaviour.

I develop this project on macOS in pure C with my own implementation of
standard library (school requirements). Currenly, the program will build
successfully via __clang 9.0.0__ at least on macOS.

But keep in mind that work is still in progress and many convenient features have either limited implementation, either none at all.

At the time of writing (May 5th, 2019) shell consists of:
* some **simple builtins**: cd, echo, env, export, exit, history, help,
  set, setenv, unsetenv, where;
* **environment variables management** with (seem like) all needed
  scopes: shell local, command local, exportable;
* **history** in zsh-like storage format and simple history builtin
* almost full-blown binary **Abstract Syntax Tree** for POSIX-compliant
  shell script execution. At the moment, it supports both logical
  operators (`||` and `&&`), pipelines, `;` and `\n`-separated commands,
  `()`-subshell execution in current shell environment (but still not
  inside pipelines because of wrong architecture assumptions);
* **expansions** for `$`-variables, single and double quotes and tilde
* management of **special variables**, specifically `0` (shell binary
  name), `$` (shell pid), `?` (last pipeline);
* all **redirects** - `>|`, `>`, `>>`, `<`, `<<`, `<<-`, `<>`, custom
  `<<<` "hereword" redirect and `>&`, `<&` file descriptor duplicators
  supporting closing specific fds;
* **non-interactive** script execution either via command-line argument
  or via opening a file at stdin file descriptor (returning correct exit
  code in case of any error, of course);
* framework for button press listening
* somewhat buggy line editing
* and many other esoteric things under the hood

## Builtins
**cd** — change the working directory, doesn't support `-L` and `-P`
options.
> `cd [directory]` <br> `cd -`

**export** — print exportable variables. Supports `-p` option.
> `export` <br> `export -p`

**history** — print history. Supports `-c` option to clear shell
history. 
> `history`<br> `history -c`<br> `history -s [entry to
insert]`

**where** — print path to command and information if this command is a
builtin. 
> `where [command name]`

**set** — print _all_ variables - both exportable and local. Does not support
any shell options config.

**env** — same as **export** without arguments.

**setenv** — same as **export** with `KEY=VALUE` arguments.
> `setenv [KEY=VALUE ...]`

**unsetenv** — unset variable of local and exportable scopes with given
name (without variable name validation).
> `unsetenv [KEY ...]`

**echo** — echo some text, will work just as you'd expect.

**exit** — exit shell. Supports numeric code argument. If the string is not numeric,
prints diagnostic message and shell exits with error code. If `exit` is executed in a
subshell environment, it does not invoke system call with the same name, but rather just
return control to AST executor.

**help** — prints supported builtins. 

## Hotkeys
Line editing is not that good, because we are forced to use old
Termcap library, instead of something better like ncurses. Currently,
hotkeys are:

| Key      | Function                    |
|----------|-----------------------------|
| Up       | Cycle history back          |
| Down     | Cycle history forward       |
| Alt+Left | Move cursor one word left   |
| Alt+Right| Move cursor one word right  |
| Alt+Up   | Move cursor vertically up   |
| Alt+Down | Move cursor vertically down |
| Ctrl+U   | Kill line                   |
| Ctrl+Y   | Paste killed line           |
| Home     | Move cursor to the beginning of the command|
| End      | Move cursor to the end of the command |

## The differences between Bash and my shell
#### Syntax differences
Most notable and, possibly, critical to some users difference is in redirects syntax.
In command of form `cat 123 > foobar` redirect will use 'WORD' on its left 
as a filedes number and will not pass it as an argument for `cat`. To avoid this
kind of behaviour, user must enclose `123` in quotes, either double or single. 

So if you want to `cat` file with numeric name as `123` and redirect output to file
with name `foobar`, the command must look like `cat "123" > foobar`

#### Script execution differences
The main and only difference is that this shell tries to validate syntax of whole file
before executing it as a one big syntax tree. So it will take some time to validate 
and emit error message, if there are syntax errors.

## Implementation specifics
I personally highlight a LL(0) parser for AST in my shell. Syntax analysis and binary AST
creation are built on simple recursive descent (see `src/ast/parser.c`) based on 
rules declared in `include/shell_script_parser.c` and defined in `src/ast/syntax_rules.c`.
The main problem with this kind of parser is its dependence on stack size and number
of recursive functions calls. As far as I tested, my parser can take up to 100 000 tokens
in one run. And it takes about 7 seconds (without compile-time optimisations) 
to parse and build a tree for a test file of 260 KB (about 6600 lines) with some simple commands.
 
So probably, finite automata can work better in some cases, providing more optimisations and 
checks on the fly to determine best fitting syntax rule for given token stream. 
But in my case, I chose recursive descent because it looked like it's easier to implement — 
the algorithm itself takes up about 60 lines of code.
Moreover, to alter parser's behaviour and ASTs it returns I don't have to change that much code.
For example, to make any redirects invalid, I can just alter corresponding rule 
in `src/ast/syntax_rules.c` file and commands with redirects will momentarily become invalid.

Also, answering the question why I didn't use Bison or YACC, that's because here
at School 42 students aren't allowed to use such libraries, unless task states otherwise.

And hey, that was really a great journey. While you write your own parses, you learn many
interesting things and have to read tons of theory on the matter.
## Compilation
To build a project, just clone it somewhere and `make` it. But have in
mind that it was developed on macOS and compiled using __clang__, so
probably, it will not compile via __gcc__ on Linux. Also, this shell
depends on `termcap` library for terminal interactions.

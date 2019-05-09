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

**set** — print _all_ variables - both exportable and local, but without
any shell options config.

**env** — same as **export** without arguments.

**setenv** — same as **export** with `KEY=VALUE` arguments.
> `setenv [KEY=VALUE ...]`

**unsetenv** — unset variable of local and exportable scopes with given
name (without variable name validation).
> `unsetenv [KEY ...]`

**echo** — echo some text, will work just as you'd expect. Does not have
any arguments yet.

**exit** — exit shell. Does not support return code specification yet. 

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


## Compilation
To build a project, just clone it somewhere and `make` it. But have in
mind that it was developed on macOS and compiled using __clang__, so
probably, it will not compile via __gcc__ on Linux. Also, this shell
depends on `termcap` library for terminal interactions.

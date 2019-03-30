_Just gonna pretend somebody will ever need this..._

## About

Currenly, the program will build successfully via __clang 9.0.0__ at least on macOS.

But keep in mind that work is still in progress and many convenient features have either limited implementation, either none at all.

At the time of writing (March 30th, 2019) shell consists of:
* some simple builtins like __echo__, __env__, __cd__, __export__, __set__, etc.
* basic environment variables management without any local / global variables shenanigans.
* somewhat buggy, but consistent input mechanisms and buffer management
* framework for button press listening
* simple history in zsh-like storage format and simple history builtin
* tokenizer for normal command parsing
* fully described syntax rules for syntax analysis
* working AST building and execution
* expansions for $-variables, quotes and tilde
* environment variables management with different scopes
* and many other esoteric things under the hood

## Compilation
Either way, to build a project, just clone it somewhere and `make` it. But have in mind that it was developed on macOS and compiled using __clang__, so probably, it won't compile via __gcc__ on Linux.

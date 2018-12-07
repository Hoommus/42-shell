_Just gonna pretend somebody will ever need this shiet..._

## About

Currenly, the program will build successfully via __clang 9.0.0__ at least on macOS. At the time of writing (December 7th, 2018) shell consists of:
* some simple builtins like __echo__, __env__, __cd__, etc.
* basic environment variables management without any local / global variables shenanigans.
* somewhat buggy, but consistent input mechanisms and buffer management
* framework for button press listening
* simple history in zsh-like storage format and simple history builtin
* tokenizer for normal command parsing
* fully described syntax rules for syntax analysis and
* necessary base for AST implementation

## Compilation
Either way, to build a project, just clone it somewhere and `make` it. But have in mind that it was developed on macOS and compiled using __clang__, so probably, it won't compile via __gcc__ on Linux.

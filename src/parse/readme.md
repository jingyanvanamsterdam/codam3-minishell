# ABOUT PARSE BRANCH

## BASE
This branch is based on feature/lexing

## Updated util function file
_error.c
_free.c

Delete from lexing directory
lex.h deleted
Makefile changed

## TO DO
Need to change makefile:
1. one target for clean up
2. one target for check valgrind 

## Parsing test conditions:
1. echo, "$? + 3 ="3, c'1 + $? "$?abc" 2 ', arg"a 'b'c" << heredoc
2. echo arg1 $?asc << heredoc$ade
3. <<abc echo $?.acd
4. echo arg$x.c"$x.c"abc << here'doc'
5. echo arg$ arg2 arg3$ << here$
6. echo arg1$| grep arg<< here$
7. echo << here"$doc"
8. echo << here"$doc"abc here"$doc"abc


## Memo for todo
1. how to handle signal for heredoc
2. how to handle non interactive and interactive minishell, signal differnece.
3. infile and outfile not check access? infile not exist how to pass to next pipeline?

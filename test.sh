#!/usr/bin/env bash

print_usage() {
    echo "
Description: Run tests on this library.

Usage: ./test.sh [OPTION]

OPTIONs
    arena
        test arena
    pool
        test pool
    mem
        test memory
    help
        show this help page and exit"
}

arg1="$1"
arg2="$2"

if [[ $arg1 == "help" ]]; then
    print_usage
    exit
fi

echo "Running tests..."
if [[ $arg1 == "arena" ]]; then
    gcc tests/test_main.c tests/tests_arena.c src/sk_arena.c -DARENA -o test
elif [[ $arg1 == "pool" ]]; then
    gcc tests/test_main.c tests/tests_pool.c src/sk_pool.c -DPOOL -o test
else
    gcc tests/*.c src/*.c -DARENA -DPOOL -o test
fi

if [[ $arg1 == "mem" ]]; then
    valgrind ./test
else
    ./test
fi

echo "Done"
rm test


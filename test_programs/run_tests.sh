#!/bin/bash

# Colors
COL_RED='\033[1;31m'
COL_GREEN='\033[1;32m'
COL_NC='\033[0m'

test_cnt=0
mem_err_cnt=0
compile_err_cnt=0

# Check valid programs
for file in valid/*
do
    ((test_cnt++))
    printf "TESTING \"$file\"... "
    ./../jcc $file > /dev/null 2>&1

    if [ $? -ne 0 ] # If the comiler exits with and error
    then
        ((compile_err_cnt++))
        printf "${COL_RED}FAIL.${COL_NC} Unexpected compilation error.\n"
    else
        # Check for memory leaks
        valgrind --leak-check=full --show-leak-kinds=all ./../jcc $file > /dev/null 2>&1
        if [ $? -ne 0 ] # Memory leak
        then
            ((mem_err_cnt++))
            printf "${COL_RED}FAIL.${COL_NC} Memory leak/error found.\n"
        else
            printf "${COL_GREEN}PASS.${COL_NC}\n"
        fi
    fi
done

# Check invalid programs.
for file in invalid/*
do
    ((test_cnt++))
    printf "TESTING \"$file\"... "
    ./../jcc $file > /dev/null 2>&1

    if [ $? -eq 0 ] # If the comiler exits without and error
    then
        ((compile_err_cnt++))
        printf "${COL_RED}FAIL.${COL_NC} Expected compilation to result in an error.\n"
    else
        printf "${COL_GREEN}PASS.${COL_NC}\n"
    fi
done

echo ""
echo "Test Summary"
echo "------------"
echo "$((mem_err_cnt+compile_err_cnt))/$test_cnt test(s) failed."
echo "$mem_err_cnt test(s) resulted in a memory leak/error."
echo "$compile_err_cnt test(s) failed to return an error message."

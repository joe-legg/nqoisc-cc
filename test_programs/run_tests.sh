#!/bin/bash

# Check valid programs
for file in valid/*
do
    ./../jcc $file;
    if [ $? -ne 1 ] # If the comiler exits with and error
    then
        echo "Test Failed: The compiler exited with an error when given the valid program \"$file\"."
        exit 1
    fi
done

for file in invalid/*
do
    ./../jcc $file;
    if [ $? -ne 0 ] # If the comiler exits without and error
    then
        echo "Test Failed: The compiler exited WITHOUT an error when given the invalid program \"$file\"."
    fi
done

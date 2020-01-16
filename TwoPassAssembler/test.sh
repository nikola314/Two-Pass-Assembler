#!/bin/bash
for ((i=1; i<=10; i++))
do
    ./assembler -o output/test"$i".txt tests/test"$i".txt
done

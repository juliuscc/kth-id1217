#!/bin/bash
printf "\nBenchmarking:$1\n"
printf "1 worker:\n" | tee /dev/tty >> $2
for i in `seq 1 10`;
do
    eval $1 0.000001 1 >> $2
done

printf "\n2 workers:\n" | tee /dev/tty >> $2
for i in `seq 1 10`;
do
    eval $1 0.000001 2 >> $2
done

printf "\n4 workers:\n" | tee /dev/tty >> $2
for i in `seq 1 10`;
do
    eval $1 0.000001 4 >> $2
done

printf "\n8 workers:\n" | tee /dev/tty >> $2
for i in `seq 1 10`;
do
    eval $1 0.000001 8 >> $2
done
#!/bin/bash
echo "1 worker:" | tee /dev/tty >> $2
for i in `seq 1 10`;
do
    eval $1 10000 1 >> $2
done

echo "2 workers:" | tee /dev/tty >> $2
for i in `seq 1 10`;
do
    eval $1 10000 2 >> $2
done

echo "4 workers:" | tee /dev/tty >> $2
for i in `seq 1 10`;
do
    eval $1 10000 4 >> $2
done

echo "8 workers:" | tee /dev/tty >> $2
for i in `seq 1 10`;
do
    eval $1 10000 8 >> $2
done
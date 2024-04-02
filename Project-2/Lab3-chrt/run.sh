#!/bin/bash
res_loc=./results
sched_policy="RR"
# sched_policy="FIFO"

echo > $res_loc/result.$sched_policy.txt
for i in {0..10}; do
    ./partner.out&
done

for i in {8..0}; do
    ((i=($i+1)*10))
    echo $i
    (sudo chrt -r $i ./lab.out $i > $res_loc/$i.$sched_policy.txt) &
    # (sudo chrt -f $i ./lab.out $i > $res_loc/$i.$sched_policy.txt) &
done
# sar -P ALL 1 1
# sudo chrt -f 99 ./lab
# sudo chrt -o -p $$
./lab.out 0 > $res_loc/normal.$sched_policy.txt 

sleep 2
sudo pkill -f partner.out
sudo pkill -f lab.out
for i in {0..8}; do
    ((i=($i+1)*10))
    cat $res_loc/$i.$sched_policy.txt >> $res_loc/result.$sched_policy.txt
done
cat $res_loc/normal.$sched_policy.txt >> $res_loc/result.$sched_policy.txt




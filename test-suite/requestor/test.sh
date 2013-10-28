#!/bin/bash

rm -f file*

echo '*** 100K ***' | tee tempi.out
(time nc -vv local3 3000 > file100K) 2>&1 | tee -a tempi.out
echo '*** 500K ***' | tee -a tempi.out
(time nc -vv local3 3001 > file500K) 2>&1 | tee -a tempi.out
echo '*** 1M ***' | tee -a tempi.out
(time nc -vv local3 3002 > file1M) 2>&1 | tee -a tempi.out
echo '*** 2M ***' | tee -a tempi.out
(time nc -vv local3 3003 > file2M) 2>&1 | tee -a tempi.out
echo '*** 10M ***' | tee -a tempi.out
(time nc -vv local3 3004 > file10M) 2>&1 | tee -a tempi.out

ls -lh file*

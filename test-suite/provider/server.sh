#!/bin/bash

bash -c 'cat file100K | nc -vv -q 0 -l -p 3000' &
bash -c 'cat file500K | nc -vv -q 0 -l -p 3001' &
bash -c 'cat file1M | nc -vv -q 0 -l -p 3002' &
bash -c 'cat file2M | nc -vv -q 0 -l -p 3003' &
bash -c 'cat file10M | nc -vv -q 0 -l -p 3004' &

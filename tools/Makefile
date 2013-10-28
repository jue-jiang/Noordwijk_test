#
# Makefile for nf-queue.c
# Compiler: gcc
# Linker: ld
#

all: nf-queue

nf-queue: nf-queue.o
	gcc nf-queue.o -lnfnetlink -lnetfilter_queue -lmnl -o nf-queue

nf-queue.o: nf-queue.c
	gcc -Wall -c nf-queue.c

clean:
	rm -f nf-queue.o nf-queue

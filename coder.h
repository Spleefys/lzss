#ifndef CODER_H
#define CODER_H
#include <stdio.h>
#include <stdlib.h>

#define EI 12
#define EJ  4
#define P   1
#define N (1 << EI)
#define F ((1 << EJ) + P)

int bit_buffer = 0, bit_mask = 128;
unsigned long codecount = 0, textcount = 0;
unsigned char buffer[N * 2];
FILE *infile, *outfile;

void error();

void putbit1();

void putbit0();

void flush_bit_buffer();

void output1(int c);

void output2(int x, int y);

void encode();

int getbit(int n);

void decode();

#endif

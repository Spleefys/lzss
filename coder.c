#include "coder.h"

void error()
{
    printf("Output error\n");
    exit(1);
}

void putbit1()
{
    bit_buffer |= bit_mask;
    if ((bit_mask >>= 1) == 0) {
        if (fputc(bit_buffer, outfile) == EOF) 
            error();
        bit_buffer = 0;
        bit_mask = 128;
        codecount++;
    }
}

void putbit0()
{
    if ((bit_mask >>= 1) == 0) {
        if (fputc(bit_buffer, outfile) == EOF)
            error();
        bit_buffer = 0;
        bit_mask = 128;
        codecount++;
    }
}

void flush_bit_buffer()
{
    if (bit_mask != 128) {
        if (fputc(bit_buffer, outfile) == EOF)
            error();
        codecount++;
    }
}

void output1(int c)
{
    int mask;

    putbit1();
    mask = 256;
    while (mask >>= 1) {
        if (c & mask)
            putbit1();
        else
            putbit0();
    }
}

void output2(int x, int y)
{
    int mask;

    putbit0();
    mask = N;
    while (mask >>= 1) {
        if (x & mask)
            putbit1();
        else
            putbit0();
    }
    mask = (1 << EJ);
    while (mask >>= 1) {
        if (y & mask)
            putbit1();
        else
            putbit0();
    }
}

void encode()
{
    int i, j, f1, x, y, r, s, bufferend, c;

    for (i = 0; i < N - F; i++)
        buffer[i] = ' ';
    for (i = N - F; i < N * 2; i++) {
        if ((c = fgetc(infile)) == EOF)
            break;
        buffer[i] = c;
        textcount++;
    }
    bufferend = i;
    r = N - F;
    s = 0;
    while (r < bufferend) {
        f1 = (F <= bufferend - r) ? F : bufferend - r;
        x = 0;
        y = 1;
        c = buffer[r];
        for (i = r - 1; i >= s; i--)
            if (buffer[i] == c) {
                for (j = 1; j < f1; j++)
                    if (buffer[i + j] != buffer[r + j])
                        break;
                if (j > y) {
                    x = i;
                    y = j;
                }
            }
        if (y <= P)
            output1(c);
        else
            output2(x & (N - 1), y - 2);
        r += y;
        s += y;
        if (r >= N * 2 - F) {
            for (i = 0; i < N; i++)
                buffer[i] = buffer[i + N];
            bufferend -= N;
            r -= N;
            s -= N;
            while (bufferend < N * 2) {
                if ((c = fgetc(infile)) == EOF)
                    break;
                buffer[bufferend++] = c;
                textcount++;
            }
        }
    }
    flush_bit_buffer();
    printf("Входные данные:  %ld байт\n", textcount);
    printf("Сжато до:  %ld байт (Коэффицент сжатия: %ld%%)\n", codecount, (codecount * 100) / textcount);
}

int getbit(int n)
{
    int i, x;
    static int buf, mask = 0;

    x = 0;
    for (i = 0; i < n; i++) {
        if (mask == 0) {
            if ((buf = fgetc(infile)) == EOF)
                return EOF;
            mask = 128;
        }
        x <<= 1;
        if (buf & mask)
            x++;
        mask >>= 1;
    }
    return x;
}

void decode()
{
    int i, j, k, r, c;

    for (i = 0; i < N - F; i++)
        buffer[i] = ' ';
    r = N - F;
    while ((c = getbit(1)) != EOF) {
        if (c) {
            if ((c = getbit(8)) == EOF)
                break;
            fputc(c, outfile);
            buffer[r++] = c;
            r &= (N - 1);
        } else {
            if ((i = getbit(EI)) == EOF)
                break;
            if ((j = getbit(EJ)) == EOF)
                break;
            for (k = 0; k <= j + 1; k++) {
                c = buffer[(i + k) & (N - 1)];
                fputc(c, outfile);
                buffer[r++] = c;
                r &= (N - 1);
            }
        }
    }
}

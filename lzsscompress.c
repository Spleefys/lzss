#include "lzss.h"

int main(int argc, char *argv[])
{
    int enc;
    char *s, *k1, *k2;

    if (argc != 5) {
        printf("Usage: lzsscompress c/d o outfile infile\n\tlzsscompress c/d infile o outfile\nc = compress\td = decode\to - output file\n");
        return 1;
    }
    s = argv[1];
    if (s[1] == 0 && (*s == 'd' || *s == 'c'))
        enc = (*s == 'c');
    else {
        printf("Неверный аргумент %s\n", s);  return 1;
    }
    k1 = argv[2];
    k2 = argv[3];
    if (*k1 == 'o'){
        if ((infile  = fopen(argv[4], "rb")) == NULL) {
            printf("Неверный файл %s\n", argv[4]);  return 1;
        }
        if ((outfile = fopen(argv[3], "wb")) == NULL) {
            printf("Неверный файл %s\n", argv[3]);  return 1;
        }
    }
    else if (*k2 == 'o'){
      if ((infile  = fopen(argv[2], "rb")) == NULL) {
          printf("Неверный файл %s\n", argv[3]);  return 1;
      }
      if ((outfile = fopen(argv[4], "wb")) == NULL) {
          printf("Неверный файл %s\n", argv[4]);  return 1;
      }
    }
    if (enc){
        encode();
    }
    else{
        decode();
    }
    fclose(infile);
    fclose(outfile);
    return 0;
}

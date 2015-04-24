
#include <openssl/des.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define ENC 1
#define DEC 0

/**
 * Read the file to be encrypted
 */
int DesFile(const char* rf, const char* wf, int ed)
{
  int rv = 0;

  FILE *i = fopen(rf, "rb"),
       *o = fopen(wf, "wb");

  char b[8];

  if(i && o)
  {
    static unsigned char cbc_key[8] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
    des_key_schedule k;
    int kr = des_set_key_checked(&cbc_key, k);
    if(kr)
    {
      printf("\nkey error\n");
      rv = 1;
    }
    else
    {
      while( (rv = fread(b, 1, 8, i)) > 0)
      {
        des_encrypt1(b, k, ed);
        fwrite(b, 1, 8, o);
      }
    }
    fclose(i);
    fclose(o);
  }
  else
  {
    rv = 2;
    printf("I/O error\n");
  }
  return rv;
}

int main(int argc, char** argv)
{
  if(argc == 4)
  {
    int enc = atoi(argv[3]);
    if(enc == 1)   printf("DES encoding ASCII file: %s to %s\n", argv[1], argv[2]);
    else           printf("Decoding DES file: %s to %s\n", argv[1], argv[2]);
    DesFile(argv[1], argv[2], enc);
  }

  return 0;
}


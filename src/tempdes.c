#include <openssl/des.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define ENC 1
#define DEC 0

/**
 * Read the file to be encrypted/decrypted and write to output
 * 
 * @param rf input file
 * @param wf output file
 * @param ed encode 0/1
 */
int DesFile(const char* rf, const char* wf, int ed)
{
  struct timeval st, ft;
  gettimeofday(&st, NULL);

  int rv = 0;

  FILE *i  = fopen(rf, "rb"),
       *o  = fopen(wf, "wb"),
       *tf = 0;

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
    gettimeofday(&ft, NULL);
    double ms = ((ft.tv_sec - st.tv_sec) * 1000.0) + ((double)(ft.tv_usec - st.tv_usec)* .001);
    tf = fopen("../doc/des_ms.txt", "a");
    if(ed) fprintf(tf, "encode: %f ms\n", ms);
    else   fprintf(tf, "decode: %f ms\n", ms);
    fclose(tf);
  }
  else
  {
    rv = 2;
    printf("I/O error\n");
  }
  return rv;
}

/**
 * Main encodes the specified input file and saves it to the specified output file,
 * then decodes it
 * 
 * @param argc 
 * @param argv infile name [1] outfile name[2]
 **/
int main(int argc, char** argv)
{
  if(argc == 3)
  {
    printf("DES encoding ASCII file: %s to %s\n", argv[1], argv[2]);
    if(DesFile(argv[1], argv[2], ENC))
    {
      printf("error encrypting\n");
    }
    else
    {
      char decout[strlen(argv[2]) + 3];
      memset(decout, 0, strlen(argv[2]) + 3);
      strcat(decout, argv[2]);
      strcat(decout, ".d");
      printf("Decoding DES file: %s to %s\n", argv[2], decout);
      if(DesFile(argv[2], decout, DEC))
      {
        printf("error decrypting\n");
      }
    }
  }

  return 0;
}


#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <string.h>

/**
 * Decodes rf with private key and writes output to wf
 */
int RsaDec(const char* rf, const char* wf, RSA** key, int sz)
{
  int r   = 0,
      v   = 0,
      len = 0;
  unsigned char* rb;
  unsigned char* wb;
  FILE *i = fopen(rf, "rb"),
       *o = fopen(wf, "wb");

  if(i && o)
  {
    fseek(i, 0, SEEK_END);
    len = ftell(i);
    fseek(i, 0, SEEK_SET);
    rb = (unsigned char*) malloc(256);
    wb = (unsigned char*) malloc(256);
    int rbs = 0, wbs = 0, len = 0;
    printf("RSA decrypting in file %s to out file %s\n", rf, wf);
    while(feof(i) == 0)
    {
      memset(rb, 0, 256);
      memset(wb, 0, 256);
      len = fread(rb, 1, 256, i);
      rbs += len;
      v = RSA_private_decrypt(len, rb, wb, *key, RSA_PKCS1_PADDING);
      if(v != -1) wbs += fwrite(wb, 1, v, o);
      else
      {
        ERR_print_errors_fp(stderr);
      }
    }
    fclose(i);
    fclose(o);
  }
  else
  {
    printf("I/O error\n");
    if(!i) printf("open %s failed\n", rf);
    if(!o) printf("open %s failed\n", wf);
    r = 1;
  }

  return r;
}

/**
 * Encodes file rf with key and writes output to wf
 */
int RsaEnc(const char* rf, const char* wf, RSA** key, int* sz)
{
  int r   = 0,
      v   = 0;
  unsigned char* rb;
  unsigned char* wb;
  FILE *i = fopen(rf, "r"),
       *o = fopen(wf, "w");

  if(i && o)
  {
    fseek(i, 0, SEEK_END);
    *sz = ftell(i);
    fseek(i, 0, SEEK_SET);
    rb = (unsigned char*) malloc(256);
    wb = (unsigned char*) malloc(256);
    printf("RSA encrypting in file %s to out file %s\n", rf, wf);
    int rbs = 0, wbs = 0, len = 0;
    while(feof(i) == 0)
    {
      memset(rb, 0, 256);
      memset(wb, 0, 256);
      fread(rb, 1, 128, i);
      rbs += strlen(rb);
      len = RSA_public_encrypt(strlen(rb), rb, wb, *key, RSA_PKCS1_PADDING);
      if(len != -1) wbs += fwrite(wb, 1, 256, o);
      else
      {
        ERR_print_errors_fp(stderr);
      }
    }

    free(rb);
    free(wb);
    fclose(i);
    fclose(o);
  }
  else
  {
    printf("I/O error\n");
    r = 1;
  }

  return r;
}

/**
 * Encodes file argv[1] to outfile argv[2] and decodes argv[2]
 */
int main(int argc, char** argv)
{
  RSA *key = 0;
  int r = 0;

  static const char rnd_seed[] =
    "string to make the random number generator think it has entropy";

  if(argc == 3)
  {
    RAND_seed(rnd_seed, sizeof rnd_seed);

    if( (key = RSA_generate_key(2048,3,NULL,NULL)) == NULL)
    {
      printf("\nerror generating key\n");
    }
    else
    {
      int sz = 0;
      r = RsaEnc(argv[1], argv[2], &key, &sz);
      if(r)
      {
        printf("RsaEnc error\n");
      }
      char decout[strlen(argv[2]) + 3];
      memset(decout, 0, strlen(argv[2]) + 3);
      strcat(decout, argv[2]);
      strcat(decout, ".d");
      r = RsaDec(argv[2], decout, &key, sz);
      if(r)
      {
        printf("RsaDec error\n");
      }
      RSA_free(key);
    }
  }
  else
  {
    printf("Usage: prog, encfile, encoutfile, decoutfile\n");
  }

  return 0;
}


#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

/**
 * Decodes rf with private key and writes output to wf
 *
 * @param const char* rf, is the read file for decoding
 * @param const char* wf, is the write file for deconding
 * @param RSA** key, is the private key for decrypting
 * @param int ed, 0 indicates decrypt, all others indicate encrypt
 */
int RsaEncDec(const char* rf, const char* wf, RSA** key, int ed)
{
  struct timeval st, ft;
  gettimeofday(&st, NULL);

  int r   = 0,               //return code
      v   = 0,               //decrypt value
      len = 0,               //length read/write
      rps = 0;
  unsigned char* rb;         //read buf
  unsigned char* wb;         //write buf
  FILE *i = fopen(rf, "rb"), //read file
       *o = fopen(wf, "wb"), //write file
       *t = 0;               //time file

  if(i && o)
  {
    fseek(i, 0, SEEK_END);
    len = ftell(i);
    fseek(i, 0, SEEK_SET);
    rb = (unsigned char*) malloc(len);
    fread(rb, 1, len, i);
    fclose(i);
    wb = (unsigned char*) malloc(256);
    if(ed == 0)
      printf("RSA decrypting in file %s to out file %s\n", rf, wf);
    else
      printf("RSA encrypting in file %s to out file %s\n", rf, wf);
    while(rps < len)
    {
      memset(wb, 0, 256);
      if(ed == 0)
      {
        v = RSA_private_decrypt(256, rb + rps, wb, *key, RSA_PKCS1_PADDING);
        rps += 256;
      }
      else
      {
        v = RSA_public_encrypt(128, rb + rps, wb, *key, RSA_PKCS1_PADDING);
        rps += 128;
      }
      if(v != -1) fwrite(wb, 1, v, o);
      else if(r + rps < len)
      {
        ERR_print_errors_fp(stderr);
        r = 2;
      }
    }
    free(rb);
    free(wb);
    fclose(o);
    gettimeofday(&ft, NULL);
    double us = ((ft.tv_sec - st.tv_sec) * 1000000.0) + ((double)(ft.tv_usec - st.tv_usec));
    t = fopen("../doc/rsa_ms.txt", "a");
    if(ed) fprintf(t, "encode: %f us\n", us);
    else   fprintf(t, "decode: %f us\n", us);
    fclose(t);
  }
  else
  {
    printf("I/O error\n");
    if(!i && rf) printf("open %s failed\n", rf);
    if(!o && wf) printf("open %s failed\n", wf);
    r = 1;
  }

  return r;
}

/**
 * Encodes and decodes text file using RSA pki where
 *
 * @param int argc is the number of arguments and
 * @param argv contains infile argv[1] outfile argv[2]
 */
int main(int argc, char** argv)
{
  RSA *key = 0;
  int    r = 0;

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
      if(RsaEncDec(argv[1], argv[2], &key, 1))
      {
        printf("RsaEnc error\n");
      }
      else
      {
        char decout[strlen(argv[2]) + 3];
        memset(decout, 0, strlen(argv[2]) + 3);
        strcat(decout, argv[2]);
        strcat(decout, ".d");
        if(RsaEncDec(argv[2], decout, &key, 0))
        {
          printf("RsaDec error\n");
        }
      }
      RSA_free(key);
    }
  }
  else
  {
    printf("Usage: prog name, encode file name, outfile name\n");
  }

  return 0;
}

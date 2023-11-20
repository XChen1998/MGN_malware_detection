
#include <stdio.h>
#include <stdlib.h>

#define KEYLEN 1024

int main (int argc, char *argv[])
{
   static unsigned char key[KEYLEN];
   static unsigned char buf[KEYLEN];
   size_t len;
   FILE *kfp, *ifp, *ofp;
   unsigned i;

   if (argc != 4)
      abort();

   kfp = fopen (argv[1], "rb");
   if (!kfp)
      abort();
   len = fread (key, 1, KEYLEN, kfp);
   if (len != KEYLEN)
      abort();
   fclose (kfp);

   ifp = fopen (argv[2], "rb");
   if (!ifp)
      abort();
   ofp = fopen (argv[3], "wb");
   if (!ofp)
      abort();
   len = fread (buf, 1, KEYLEN, ifp);
   for (i = 0; i < len; ++i)
      buf[i] ^= key[i];
   fwrite (buf, 1, len, ofp);

   while ((len = fread (buf, 1, KEYLEN, ifp)) != 0)
      fwrite (buf, 1, len, ofp);
   fclose (ofp);
   fclose (ifp);
   return 0;
}

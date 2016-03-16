#include <mysql.h>
#include <my_global.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#define SZ 128

char* fgets_alloc(FILE *fp)
{
  char* buf = NULL;
  size_t size = 0;
  size_t used = 0;
  do {
    size += SZ;
    char *buf_new = (char *) realloc(buf, size);
    if (buf_new == NULL) {
      // Out-of-memory
      free(buf);
      return NULL;
    }
    buf = buf_new;
    if (fgets(&buf[used], (int) (size - used), fp) == NULL) {
      // feof or ferror
      if (used == 0 || ferror(fp)) {
        free(buf);
        buf = NULL;
      }
      return buf;
    }
    size_t length = strlen(&buf[used]);
    if (length + 1 != size - used) break;
    used += length;
  } while (buf[used - 1] != '\n');
  return buf;
}

int main(int argc, char* argv[])
{

  FILE *fp = fopen("../tips/general.tips","r");

  int count = -1;

  std::string tip = "";

  while(count < 2)
    {
      char * buf = fgets_alloc(fp);
      if (strcmp(buf,"##----\n") == 0)
        {
          if (count > -1)
            printf("%s",tip.c_str());
          tip = "";
          count ++;
          continue;
        }
      tip += buf;
    }
}

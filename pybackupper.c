#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zipbackup.h>

int main(int argc, char *argv[])
{
  printf("PyBackupper: 1.0.0.0h\n");
  if (argc > 3)
    {
      zipbackup zip;
      zipbackup_constructor(&zip, argv[2], argv[3], argv[1]);
      char *s = zipbackup_bbackup(&zip);
      printf("%s\n", s);
      free(s);
      zipbackup_destructor(&zip);
    }
}

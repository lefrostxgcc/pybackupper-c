#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zipbackup.h>

int main(int argc, char *argv[])
{
  printf("PyBackupper: 1.0.0.0e\n");
  if (argc > 3)
    {
      if (strcmp(argv[1], "-a") == 0)
        {
          zipbackup zip;
          zipbackup_constructor(&zip, argv[2], argv[3], "w");
          char *s = zipbackup_bzipfile(&zip);
          printf("%s\n", s);
          free(s);
          zipbackup_destructor(&zip);
        }
      else if (strcmp(argv[1], "-e") == 0)
        {
          zipbackup zip;
          zipbackup_constructor(&zip, argv[2], argv[3], "r");
          char *s = zipbackup_bextract(&zip);
          printf("%s\n", s);
          free(s);
          zipbackup_destructor(&zip);
        }
    }
}

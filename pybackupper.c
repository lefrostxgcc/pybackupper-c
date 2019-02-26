#include <stdio.h>
#include <stdlib.h>
#include <zipbackup.h>

int main(void)
{
  printf("PyBackupper: 1.0.0.0b\n");
  zipbackup zip;
  zipbackup_constructor(&zip, "/home/chip/python/test/zip/backup_2c.zip", "/home/chip/python/", "r");
  /* char *bzip_str = zipbackup_bzipfile(&zip, zip.filename, zip.path); */
  char *bext_str = zipbackup_bextract(&zip);
  /* printf("%s\n", bzip_str); */
  printf("%s\n", bext_str);
  /* free(bzip_str); */
  free(bext_str);
  zipbackup_destructor(&zip);
}

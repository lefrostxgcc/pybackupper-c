#include <zipbackup.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MSG1 "Файлы по пути "
#define MSG2 " архивированы в архив "
#define MSG3 "Архив "
#define MSG4 " распакован в каталог "

enum {MSG1_LEN = (sizeof MSG1) - 1, MSG2_LEN = (sizeof MSG2) - 1,
      MSG3_LEN = (sizeof MSG3) - 1, MSG4_LEN = (sizeof MSG4) - 1};

void zipbackup_constructor(zipbackup * const this)
{
  this->filename = "";
  this->path = "";
}

static char *concat(const char *s1, const char *s2,
                    const char *s3, const char *s4)
{
  size_t s1_len = strlen(s1);
  size_t s2_len = strlen(s2);
  size_t s3_len = strlen(s3);
  size_t s4_len = strlen(s4);
  size_t s_len = s1_len + s2_len + s3_len + s4_len + 1;
  char *s = (char *) malloc(s_len);
  memcpy(s, s1, s1_len);
  memcpy(s + s1_len, s2, s2_len);
  memcpy(s + s1_len + s2_len, s3, s3_len);
  memcpy(s + s1_len + s2_len + s3_len, s4, s4_len);
  s[s1_len + s2_len + s3_len + s4_len] = '\0';
  return s;
}

char *zipbackup_bzipfile(zipbackup *const this, const char *filename, const char *path)
{
  return concat("Файлы по пути ", path, " архивированы в архив ", filename);
}

char *zipbackup_bextract(zipbackup * const this, const char *filename, const char *path)
{
    return concat("Архив ", filename, " распакован в каталог ", path);
}

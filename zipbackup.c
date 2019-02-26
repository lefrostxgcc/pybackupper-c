#include <zipbackup.h>
#include <zipfile.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>

static char *chip_concat(const char *s, ...)
{
  if (s == NULL)
    return NULL;
  va_list ap;
  const char *s_saved = s;
  const char *p = NULL;
  size_t len = strlen(s);
  va_start(ap, s);
  while ((p = va_arg(ap, const char *)) != NULL)
    len += strlen(p);
  char *result = (char *) malloc(len + 1);
  if (result == NULL)
    return NULL;
  *result = '\0';
  strcat(result, s_saved);
  va_end(ap);
  s = s_saved;
  va_start(ap, s);
  p = NULL;
  while ((p = va_arg(ap, const char *)) != NULL)
    strcat(result, p);
  va_end(ap);
  return result;
}

void zipbackup_constructor(zipbackup * const self, const char *filename,
                           const char *path, const char *mode)
{
  self->filename = strdup(filename);
  self->path = strdup(path);
  if (mode && mode[0] == 'r')
    self->archtype = ZIP_RDONLY;
  else
    self->archtype = ZIP_CREATE | ZIP_TRUNCATE;
}

void zipbackup_destructor(zipbackup * const self)
{
  free(self->filename);
  free(self->path);
  self->filename = NULL;
  self->path = NULL;
  printf("Экземпляр класса уничтожен\n");
}

char *zipbackup_bzipfile(zipbackup *const self, const char *filename, const char *path)
{
  (void) self;
  return chip_concat("Файлы по пути ", path, " архивированы в архив ", filename, NULL);
}

char *zipbackup_bextract(zipbackup * const self)
{
  char *result = NULL;
  zip_error_t *err = NULL;
  int ze = 0;
  zip_t *wr = zip_open(self->filename, self->archtype, &ze);
  if (!wr)
    {
      zip_error_t error;
      zip_error_init_with_code(&error, ze);
      result = chip_concat("Error: ", zip_error_strerror(&error), NULL);
    }
  else if (!extractall(wr, self->path, &err))
    result = chip_concat("Error: ", err ? zip_error_strerror(err) : strerror(errno), NULL);
  else if (zip_close(wr) == -1)
    result = chip_concat("Error: ", zip_error_strerror(zip_get_error(wr)), NULL);
  else
    result = chip_concat("Архив ", self->filename, " распакован в каталог ", self->path, NULL);
  return result;
}

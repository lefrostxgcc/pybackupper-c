#include <zipbackup.h>
#include <zipfile.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>

static char *zipbackup_addarchive(zipbackup * const self, zip_t *wr);
static char *zipbackup_extract(zipbackup * const self, zip_t *wr);
static char *zipbackup_view(zipbackup * const self, zip_t *wr);

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
                           const char *path, const char *command)
{
  self->filename = strdup(filename);
  self->path = strdup(path);
  self->archtype = ZIP_RDONLY;
  if (command)
    {
      if (strcmp(command, "-a") == 0)
        self->archtype = ZIP_CREATE | ZIP_TRUNCATE;
      else if (strcmp(command, "-e") == 0)
        self->archtype = ZIP_RDONLY;
      else if (strcmp(command, "-v") == 0)
        self->archtype = ZIP_RDONLY;
    }
  self->command = strdup(command);
}

void zipbackup_destructor(zipbackup * const self)
{
  free(self->filename);
  free(self->path);
  free(self->command);
  self->filename = NULL;
  self->path = NULL;
  self->command = NULL;
  printf("Экземпляр класса уничтожен\n");
}

char *zipbackup_bbackup(zipbackup *const self)
{
  char *result = NULL;
  int ze = 0;
  zip_t *wr = zip_open(self->filename, self->archtype, &ze);
  if (!wr)
    {
      zip_error_t error;
      zip_error_init_with_code(&error, ze);
      result = chip_concat("Error: ", zip_error_strerror(&error), NULL);
      return result;
    }
  if (strcmp(self->command, "-a") == 0)
    result = zipbackup_addarchive(self, wr);
  else if (strcmp(self->command, "-e") == 0)
    result = zipbackup_extract(self, wr);
  else if (strcmp(self->command, "-v") == 0)
    result = zipbackup_view(self, wr);
  else
    result = strdup("unknown option");
  zip_close(wr);
  return result;
}

static char *zipbackup_addarchive(zipbackup * const self, zip_t *wr)
{
  char *result = NULL;
  zip_error_t *err = NULL;
  if (!compress(wr, self->path, &err))
    result = chip_concat("Error: ", err ? zip_error_strerror(err) : strerror(errno), NULL);
  else
    result = chip_concat("Файлы по пути ", self->path, " архивированы в архив ", self->filename, NULL);
  return result;
}

static char *zipbackup_extract(zipbackup * const self, zip_t *wr)
{
  char *result = NULL;
  zip_error_t *err = NULL;
  if (!extractall(wr, self->path, &err))
    result = chip_concat("Error: ", err ? zip_error_strerror(err) : strerror(errno), NULL);
  else
    result = chip_concat("Архив ", self->filename, " распакован в каталог ", self->path, NULL);
  return result;
}

static char *zipbackup_view(zipbackup * const self, zip_t *wr)
{
  (void) self;
  char *result = NULL;
  zip_error_t *err = NULL;
  if (!printdir(wr, &err))
    result = chip_concat("Error: ", err ? zip_error_strerror(err) : strerror(errno), NULL);
  else
    result = strdup("");
  return result;
}

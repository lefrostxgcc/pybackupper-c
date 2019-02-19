#ifndef ZIPBACKUP_H
#define ZIPBACKUP_H

typedef struct {
  const char *filename;
  const char *path;
} zipbackup;

void zipbackup_constructor(zipbackup * const this);
char *zipbackup_bzipfile(zipbackup * const this,
                         const char *filename,
                         const char *path);
char *zipbackup_bextract(zipbackup * const this,
                         const char *filename,
                         const char *path);

#endif

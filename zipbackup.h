#ifndef ZIPBACKUP_H
#define ZIPBACKUP_H

typedef struct {
  char *filename;
  char *path;
  int archtype;
} zipbackup;

void zipbackup_constructor(zipbackup * const self,
                           const char *filename,
                           const char *path,
                           const char *archtype);
void zipbackup_destructor(zipbackup * const self);
char *zipbackup_bzipfile(zipbackup * const self);
char *zipbackup_bextract(zipbackup * const self);

#endif

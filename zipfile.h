#ifndef ZIPFILE_H
#define ZIPFILE_H

#include <zip.h>

void zip_error_exit(zip_error_t *error);
void ze_error_exit(int ze);
int printdir(zip_t *z, zip_error_t **err);
int extractall(zip_t *z, const char *path, zip_error_t **err);
int compress(zip_t *z, const char *dir, zip_error_t **err);

#endif

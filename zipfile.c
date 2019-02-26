#include <zipfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <zip.h>

static int create_dir(const char *dir)
{
  if (mkdir(dir, 0755) < 0)
      if (errno != EEXIST)
        return 0;
  return 1;
}

static char *strcat_new(const char *s1, const char *s2)
{
  size_t s1_len = strlen(s1);
  size_t s2_len = strlen(s2);
  char *s = (char *) malloc(s1_len + s2_len + 1);
  memcpy(s, s1, s1_len);
  memcpy(s + s1_len, s2, s2_len);
  s[s1_len + s2_len] = '\0';
  return s;
}

void zip_error_exit(zip_error_t *error)
{
  fprintf(stderr, "%s\n", zip_error_strerror(error));
  exit(1);
}

void ze_error_exit(int ze)
{
  zip_error_t error;
  zip_error_init_with_code(&error, ze);
  zip_error_exit(&error);
}

int printdir(zip_t *z, zip_error_t **err)
{
  char buf_time[32];
  zip_int64_t num = zip_get_num_entries(z, ZIP_FL_UNCHANGED);
  zip_stat_t sb;
  zip_stat_init(&sb);
  *err = NULL;
  printf("%-54s%s%17s\n", "File Name", "Modified", "Size");
  for (zip_int64_t i = 0; i < num; i++)
    {
      if (zip_stat_index(z, i, 0, &sb) == -1)
        {
          *err = zip_get_error(z);
          return 0;
        }
      strftime(buf_time, sizeof buf_time, "%Y-%m-%d %H:%M:%S",
               localtime(&sb.mtime));
      printf("%-47s%s%13"PRIiMAX"\n", sb.name, buf_time, sb.size);
    }
  return 1;
}

int extractall(zip_t *z, const char *path, zip_error_t **err)
{
  static char buf[4096];
  zip_int64_t num = zip_get_num_entries(z, ZIP_FL_UNCHANGED);
  zip_stat_t sb;
  zip_stat_init(&sb);
  *err = NULL;
  if (!create_dir(path))
    return 0;
  for (zip_int64_t i = 0; i < num; i++)
    {
      if (zip_stat_index(z, i, 0, &sb) == -1)
        {
          *err = zip_get_error(z);
          return 0;
        }
      char *filename = strcat_new(path, sb.name);
      if (sb.name[strlen(sb.name) - 1] == '/')
        {

          if (!create_dir(filename))
            return 0;
        }
      else
        {
          struct zip_file *zf = zip_fopen_index(z, i, 0);
          if (!zf)
            {
              *err = zip_file_get_error(zf);
              return 0;
            }
          FILE *f = fopen(filename, "w");
          if (!f)
            return 0;
          zip_uint64_t pos = 0;
          while (pos != sb.size)
            {
              zip_int64_t bytes = zip_fread(zf, buf, 100);
              if (bytes == -1)
                {
                  *err = zip_file_get_error(zf);
                  return 0;
                }
              if ((zip_int64_t )fwrite(buf, sizeof buf[0], bytes, f) != bytes)
                return 0;
              pos += bytes;
            }
          if (fclose(f) == EOF)
            return 0;
          if (zip_fclose(zf) != 0)
            {
              *err = zip_file_get_error(zf);
              return 0;
            }

        }
      free(filename);
    }
  return 1;
}

static char *concat_dir_filename(const char *dir, const char *filename)
{
  size_t dir_len = strlen(dir);
  size_t filename_len = strlen(filename);
  char *s = (char *) malloc(dir_len + filename_len + 2);
  strcpy(s, dir);
  strcpy(s + dir_len + 1, filename);
  s[dir_len] = '/';
  return s;
}

static int add_file(zip_t *z,
                    const char *dir,
                    const char *path,
                    const char *filename, zip_error_t **err)
{
  char *dir_s = concat_dir_filename(dir, filename);
  char *path_s = concat_dir_filename(path, filename);
  zip_source_t *source = zip_source_file_create(dir_s, 0, -1, NULL);
  if (source == NULL)
    return 0;
  zip_int64_t ind = zip_file_add(z, path_s, source,
                                 ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);
  if (ind == -1)
    {
      free(dir_s);
      free(path_s);
      zip_source_free(source);
      *err = zip_get_error(z);
      return 0;
    }
  free(dir_s);
  free(path_s);
  return 1;
}

static int compress_dir(zip_t *z, const char *dir, const char *path, zip_error_t **err)
{
  *err = NULL;
  DIR *d = opendir(dir);
  if (d == NULL)
    return 0;
  struct dirent *fd;
  errno = 0;
  while ((fd = readdir(d)) != NULL)
    {
      if (fd->d_type == DT_DIR)
        {
          if (strcmp(fd->d_name, ".") == 0 ||
              strcmp(fd->d_name, "..") == 0)
            continue;
          char *sub_dir = concat_dir_filename(dir, fd->d_name);
          char *sub_path = concat_dir_filename(path, fd->d_name);
          if (zip_dir_add(z, sub_path, ZIP_FL_ENC_UTF_8) == -1)
            {
              free(sub_dir);
              free(sub_path);
              *err = zip_get_error(z);
              return 0;
            }
          if (!compress_dir(z, sub_dir, sub_path, err))
            {
              free(sub_dir);
              free(sub_path);
              return 0;
            }
          free(sub_dir);
          free(sub_path);
        }
      else
        if (!add_file(z, dir, path, fd->d_name, err))
          return 0;
    }
  if (errno != 0)
    return 0;
  if (closedir(d) == -1)
    return 0;
  return 1;
}

int compress(zip_t *z, const char *dir, zip_error_t **err)
{
  const char *path = dir;
  while (path != NULL && *path == '/')
    path++;
  *err = NULL;
  if (!compress_dir(z, dir, path, err))
    return 0;
  else
    return 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zipbackup.h>
#include <json-glib/json-glib.h>

int main(int argc, char *argv[])
{
  printf("PyBackupper: 1.0.0.0i\n");
  if (argc == 2 && strcmp(argv[1], "af") == 0)
    {
      JsonParser *parser = json_parser_new();
      json_parser_load_from_file(parser, "b.json", NULL);
      JsonNode *root_node = json_parser_get_root(parser);
      JsonObject *root_object = json_node_get_object(root_node);
      const char *p = json_object_get_string_member(root_object, "arch");
      printf("%s\n", p);
      g_object_unref(parser);
    }
  else if (argc > 3)
    {
      zipbackup zip;
      zipbackup_constructor(&zip, argv[2], argv[3], argv[1]);
      char *s = zipbackup_bbackup(&zip);
      printf("%s\n", s);
      free(s);
      zipbackup_destructor(&zip);
    }
}

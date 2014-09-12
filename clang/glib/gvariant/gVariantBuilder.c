#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>
GVariant* make_aay(){
  GVariantBuilder builder;
  int i, j;
  g_variant_builder_init (&builder, G_VARIANT_TYPE ("aay"));
  for (i = 0; i < 16; i++)
    {
      g_variant_builder_open (&builder, G_VARIANT_TYPE("ay"));
      guchar buf[4] = {'I','a','m', '\0'};
      //sprintf (buf, "%d", i);
      for(j = 0; j < 4; j++){
          g_variant_builder_add (&builder, "y", buf[i]);
      }
      g_variant_builder_close(&builder);
    }
  return g_variant_builder_end (&builder);
}

GVariant *
make_pointless_dictionary (void)
{
  GVariantBuilder builder;
  int i;
  g_variant_builder_init (&builder, G_VARIANT_TYPE_ARRAY);
  for (i = 0; i < 16; i++)
    {
      gchar buf[3];
      sprintf (buf, "%d", i);
      g_variant_builder_add (&builder, "{is}", i, buf);
    }
  return g_variant_builder_end (&builder);
}

static GVariant* encode_aay(guchar **txt_aay, int rows){
    GVariantBuilder builder;
    int i, j, length;
    g_variant_builder_init (&builder, G_VARIANT_TYPE ("aay"));
    for (i = 0; i < rows; i++)
    {
        g_variant_builder_open (&builder, G_VARIANT_TYPE("ay"));
        length = strlen(txt_aay[i]);
        // g_printf("length of rows%d: %d\n", i, length);
        for(j = 0; j < length; j++){
            g_variant_builder_add (&builder, "y", txt_aay[i][j]);
        }
        g_variant_builder_close(&builder);
    }
    return g_variant_builder_end (&builder);
}

int main(int argc, char *argv[])
{
    g_print("type '%s'\n", g_variant_get_type_string(make_pointless_dictionary()));
    g_print("type '%s'\n", g_variant_get_type_string(make_aay()));
    char *name = "try";
    guint16 port = 80;
    guchar *one = "one";
    guchar *two = "two222";
    guchar **txt_aay = (guchar**)malloc(sizeof(guchar*) * 2);
    txt_aay[0] = one;
    txt_aay[1] = two;
    encode_aay(txt_aay, 2);
}

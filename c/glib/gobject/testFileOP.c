#include<glib.h> 

int main(int argc, char* argv[])
{
    gchar *home = getenv("HOME");
    g_printf("home:%s", home);
    g_message("Couldn't set messages locale category from environment.");
    return 0;
}

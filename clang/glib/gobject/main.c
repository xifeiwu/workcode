#include <glib.h>
void run_cmd(gchar* cmd)
{
    gint argc;
    gchar **argv = NULL;
    GError *e = NULL;
    gboolean ok;
    if (!g_shell_parse_argv(cmd, &argc, &argv, &e)) {
        g_message("Error parsing startup command: %s",
                  e->message);
        g_error_free(e);
        e = NULL;
    }
    g_printf("argc: %d\n", argc);
    g_printf("values of argv:\n");
    int i=0;
    for(i=0; i<argc; i++){
        g_printf("%s\n", argv[i]);
    }
    g_printf("end\n\n");
}
/* **************************************************
 * sh_cmd() - executes a command in the background
 * returns TRUE is command was executed  (not the result of the command though..)
 * NO GLOBALS
*/
gint sh_cmd (gchar * path, gchar * cmd, gchar * args)
{
  gchar     cmd_line[256];
  gchar   **argv;
  gint      argc;
  gint      rc = 0;
  if (cmd == NULL)
    return FALSE;
  if (cmd[0] == '\0')
    return FALSE;
  if (path != NULL)
    chdir (path);
  snprintf (cmd_line, sizeof (cmd_line), "%s %s", cmd, args);
  rc = g_shell_parse_argv (cmd_line, &argc, &argv, NULL);
  if (!rc)
  {
    g_strfreev (argv);
    return rc;
  }
  rc = g_spawn_async (path, argv, NULL,
		      G_SPAWN_STDOUT_TO_DEV_NULL | G_SPAWN_SEARCH_PATH,
		      NULL, NULL, NULL, NULL);
    g_printf("argc: %d\n", argc);
    g_printf("rc: %d\n", rc);
    g_printf("values of argv:\n");
    int i=0;
    for(i=0; i<argc; i++){
        g_printf("%s\n", argv[i]);
    }
    g_printf("end\n\n");
  g_strfreev (argv);
  return rc;
}
int main(int argc, char *argv[])
{
    gchar *cmd = "pwd";
    sh_cmd("/home/cos", "touch", "wirte");
//    run_cmd(cmd);
    g_print("g_get_home_dir: %s\n", g_get_home_dir());
    g_print("g_getenv: %s\n", g_getenv("DESKTOP_AUTOSTART_ID"));

    g_print("\nOptions:\n");
    g_print("  --help              Display this help and exit\n");
    g_print("  --version           Display the version and exit\n");
    g_print("  --replace           Replace the currently running window manager\n");
    g_print("  --config-file FILE  Specify the path to the config file to use\n");
    g_print("  --sm-disable        Disable connection to the session manager\n");
    g_print("\nPassing messages to a running Openbox instance:\n");
    g_print("  --reconfigure       Reload Openbox's configuration\n");
    g_print("  --restart           Restart Openbox\n");
    g_print("  --exit              Exit Openbox\n");
    g_print("\nDebugging options:\n");
    g_print("  --sync              Run in synchronous mode\n");
    g_print("  --startup CMD       Run CMD after starting\n");
    g_print("  --debug             Display debugging output\n");
    g_print("  --debug-focus       Display debugging output for focus handling\n");
    g_print("  --debug-session     Display debugging output for session management\n");
    g_print("  --debug-xinerama    Split the display into fake xinerama screens\n");
}

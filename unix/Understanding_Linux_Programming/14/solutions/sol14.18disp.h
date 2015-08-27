
#define DFL_HOST "localhost"
#define DFL_PORT 2828

char *display_info(char *, char [], int);
void display_init(char *hostname, int portnum);
int tell_server(char *msg);
int from_server(char buf[], int l);
void display_clear();
int display_draw(int r, int c, char *str);

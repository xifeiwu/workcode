#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

int make_internet_address(char *, int, struct sockaddr_in *);
int make_dgram_server_socket(int);
int make_dgram_client_socket();
int get_internet_address(char *, int , int *, struct sockaddr_in *);

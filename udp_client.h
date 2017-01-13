#include <stdio.h>
#include <sys/types.h>  /* socket() */
#include <sys/socket.h> /* socket() */
#include <netinet/in.h> /* struct sockaddr_in */
#include <netdb.h>      /* gethostbyname() */

#include <sched.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFERSIZE      1024
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); } while (0)

extern void echo_client_udp( char *server, int portno, int nstr, char *strv[] );
extern int udp_port_nobind();
extern int sockaddr_in_init( struct sockaddr_in *addr, char *hostname, int portno );
/* extern void sockaddr_in_print( struct sockaddr_in *addr ); */
/* extern void sockname_print( int s ); */


/*
  echo-server-udp.c -- 文字列を送受信するサーバ(UDP/IP版) 
  ~yas/syspro/ipc/echo-server-udp.c
  Start: 1997/06/16 21:22:26
*/
#include <stdio.h>
#include <sys/types.h>  /* socket() */
#include <sys/socket.h> /* socket() */
#include <netinet/in.h> /* struct sockaddr_in */
#include <netdb.h>      /* gethostbyname() */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>      /* read(),write(),fork(),close(),...       */
#include <arpa/inet.h>   /* inet_ntoa(),....                        */
#include <sys/un.h>      /* struct sockaddr_un                      */
#include <fcntl.h>       /* open(),...                              */
#include <tcpd.h>        /* hosts_access().....                     */ 
#include <syslog.h>
#include <termios.h>


extern void echo_server_udp( int portno );
extern int udp_port_bind( int portno );
extern void sockaddr_in_print( struct sockaddr_in *addr );
extern void sockname_print( int s );
int getch(void);

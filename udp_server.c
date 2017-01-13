#include "udp_server.h"

#define BUFFERSIZE      1024

void echo_server_udp( int portno )
{
  int s, rcount, scount, addrlen ;
  struct sockaddr_in addr ;
  char buffer[BUFFERSIZE];
  
  s = udp_port_bind( portno );
  if( s<0 )
    exit( -1 );
  printf("my port is "); sockname_print( s );
  
  while( 1 )
    {
      addrlen = sizeof( addr );
      rcount = recvfrom( s, buffer, BUFFERSIZE, 0,
			 (struct sockaddr *)&addr, &addrlen );
      printf("%s", buffer);
      
      /* if( rcount < 0 ) */
      /* 	{ */
      /* 	  perror("recvfrom()"); */
      /* 	  exit( 1 ); */
      /* 	} */
      /* buffer[rcount] = 0 ; */
      /* printf("received %d bytes [%s] from ",rcount, buffer ); */
      /* sockaddr_in_print( &addr ); */
      
      /* printf("sending back [%s] (%d bytes) to ", buffer, rcount ); */
      /* sockaddr_in_print( &addr ); */
      /* scount=sendto( s, buffer, rcount, 0, (struct sockaddr *)&addr, addrlen ); */
      /* if( scount!= rcount ) */
      /* 	{ */
      /* 	  perror("sendto()"); */
      /* 	  exit( 1 ); */
      /* 	} */
    }
}

int udp_port_bind( int portno )
{
  struct hostent *hostent ;
  struct sockaddr_in addr ;
  int addr_len ;
  int s ;
  
  if( (s = socket(PF_INET, SOCK_DGRAM, 0)) < 0 )
    {
      perror("socket");
      return( -1 );
    }
  
  addr.sin_family = AF_INET ;
  addr.sin_addr.s_addr = INADDR_ANY ;
  addr.sin_port = htons( portno );
  
  if( bind(s,(struct sockaddr *)&addr,sizeof(addr)) < 0 )
    {
      perror( "bind: " );
      fprintf(stderr,"port number %d is already used. kill another program.\n",
	      portno );
      return( -1 );
    }
  return( s );
}

void
sockaddr_in_print( struct sockaddr_in *addr )
{
  union {
    int i ;
    unsigned char byte[4] ;
  } x ;
  x.i = addr->sin_addr.s_addr ;
  printf("sockaddr_in: %d.%d.%d.%d:%d\n",
	 x.byte[0],x.byte[1],x.byte[2],x.byte[3],
	 ntohs( addr->sin_port ));
}

void
sockname_print( int s )
{
  struct sockaddr_in addr ;
  int len ;
  len = sizeof( addr );
  if( getsockname( s, (struct sockaddr *)&addr, &len )< 0 )
    {
      perror("getsockname");
      exit( -1 );
    }
  sockaddr_in_print( &addr );
}

int getch()
{
  static int ch = -1, fd = 0;
  struct termios neu, alt;
  fd = fileno(stdin);
  tcgetattr(fd, &alt);
  neu = alt;
  neu.c_lflag &= ~(ICANON|ECHO);
  tcsetattr(fd, TCSANOW, &neu);
  ch = getchar();
  tcsetattr(fd, TCSANOW, &alt);
  return ch;
}

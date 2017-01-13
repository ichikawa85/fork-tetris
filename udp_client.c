
/*
  echo-client-udp.c -- 文字列を送受信するクライアント(UDP/IP版)
  ~yas/syspro/ipc/echo-client-udp.c
  Start: 1997/06/16 21:22:26
*/

#include "udp_client.h"

void
echo_client_udp( char *server, int portno, int nstr, char *strv[] )
{
  int          sock ;
  int          slen,scount,rcount ;
  char         sbuf[BUFFERSIZE];
  char         rbuf[BUFFERSIZE];
  int          i ;
  int          fromlen ;
  struct sockaddr_in to, from ;

  /* int fd; */
  /* fd = open("/proc/2854/ns/net", O_RDONLY);   /\* Get descriptor for namespace *\/ */
  /* if (setns(fd, 0) == -1)         /\* Join that namespace *\/ */
  /*   errExit("setns"); */

  /* system("ip link"); */
  /* system("netstat -r"); */
  
  sock = udp_port_nobind();
  if( sock<0 )
    exit( -1 );
  printf("my port is "); sockname_print( sock );

  /* int pid; */
  /* if( (pid=fork()) <0 ) { */
  /*   perror("fork"); */
  /*   return 1; */
  /* } else if(pid==0) { */
  /*   portno = 22222; */
  /* } */
  
  if( sockaddr_in_init( &to, server, portno )<0 )
    {
      perror("sockaddr_in_init");
      exit( -1 );
    }
  sockaddr_in_print( &to );
  
  for( i=0 ; i<nstr ; i++ )
    {
      snprintf( sbuf,BUFFERSIZE,"%s\n",strv[i] );
      slen = strlen( sbuf );
      
      printf("sending [%s] (%d bytes) to ", sbuf, slen );
      sockaddr_in_print( &to );
      
      /* my addition */
      while(1){
	scount = sendto( sock, sbuf, slen, 0,
			 (struct sockaddr *)&to, sizeof(to) );
	if( scount != slen )
	  {
	    perror("sendto()");
	    exit( 1 );
	  }
	sleep(1);
      }
      /**********/

      fromlen = sizeof( from );
      rcount = recvfrom( sock, rbuf, BUFFERSIZE, 0,
			 (struct sockaddr *)&from, &fromlen);
      if( rcount < 0 )
	{
	  perror("recvfrom()");
	  exit( 1 );
	}
      rbuf[rcount] = 0 ;
      printf("received %d bytes [%s] from ",rcount, rbuf );
      sockaddr_in_print( &from );
    }
  printf("after sendto(), my port is "); sockname_print( sock );
  close( sock );
}

int
udp_port_nobind()
{
  int s ;
  if( (s = socket(PF_INET, SOCK_DGRAM, 0)) < 0 )
    {
      perror("socket");
      return( -1 );
    }
  return( s );
}

int
sockaddr_in_init( struct sockaddr_in *addr, char *hostname, int portno )
{
  struct hostent *hostent ;
  memset( addr, 0, sizeof(struct sockaddr_in) );
  addr->sin_family = AF_INET ;
  if( (hostent = gethostbyname( hostname )) == NULL )
    {
      fprintf(stderr,"unknown host %s\n",hostname );
      return( -1 );
    }
  memcpy( &addr->sin_addr, hostent->h_addr, hostent->h_length );
  addr->sin_port = htons( portno );
  return( 0 );
}

/* void */
/* sockaddr_in_print( struct sockaddr_in *addr ) */
/* { */
/*   union { */
/*     int i ; */
/*     unsigned char byte[4] ; */
/*   } x ; */
/*   x.i = addr->sin_addr.s_addr ; */
/*   printf("sockaddr_in: %d.%d.%d.%d:%d\n", */
/* 	 x.byte[0],x.byte[1],x.byte[2],x.byte[3], */
/* 	 ntohs( addr->sin_port )); */
/* } */

/* void */
/* sockname_print( int s ) */
/* { */
/*   struct sockaddr_in addr ; */
/*   int len ; */
/*   len = sizeof( addr ); */
/*   if( getsockname( s, (struct sockaddr *)&addr, &len )< 0 ) */
/*     { */
/*       perror("getsockname"); */
/*       exit( -1 ); */
/*         } */
/*   sockaddr_in_print( &addr ); */
/* } */

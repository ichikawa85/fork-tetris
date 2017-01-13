
#include "udp_server.h"

main( int argc, char *argv[] )
{
  int portno ;
  if( argc >= 3 )
    {
      fprintf( stdout,"Usage: %s [portno]\n",argv[0] );
      exit( -1 );
    }
  if( argc == 2 )
    portno = atoi( argv[1] );
  else
    portno = getuid();
  echo_server_udp( portno );
}


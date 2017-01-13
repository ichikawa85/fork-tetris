serv:
	cc -Wall -o udp_server udp_tetris.c udp_server.c udp_client.c

sub:
	cc -Wall -o sub common.c acl.c sub_server.c -lwrap

steal:
	cc -Wall -o steal common.c acl.c steal_server.c -lwrap

cli:
	cc -Wall -o udp_term udp_term.c udp_server.c

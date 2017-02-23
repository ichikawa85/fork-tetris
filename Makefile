serv:
	cc -Wall -o tetris tetris.c common.c acl.c -lwrap

steal:
	cc -Wall -o steal common.c acl.c steal.c -lwrap

cli:
	cc -Wall -o client cloud_client.c common.c acl.c -lwrap

proxy:
	cc -Wall -o proxy_server common.c acl.c proxy_server.c -lwrap

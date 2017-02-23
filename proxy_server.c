#include "common.h"
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

//#define FD_SETSIZE 5
#define MAXITEM 2
#define SYS_REQUEST_FORK 310
#define SYS_SWITCH_FD 314

int fork_flag = 1;

int calc_time(char *word) {
  /* FILE *outputfile; */
  /* outputfile = fopen("proxylog.txt", "a"); */
  /* if(outputfile == NULL){ */
  /*   printf("cannot open\n"); */
  /*   exit(1); */
  /* } */
  
  struct timeval myTime;
  struct tm *time_st;   
  const char weekName[7][4] = {
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat"
  };

  gettimeofday(&myTime, NULL); 
  time_st = localtime(&myTime.tv_sec);

  printf("%s : %d/%02d/%02d(%s) %02d:%02d:%02d.%06d\n", word,
  	 time_st->tm_year+1900,
  	 time_st->tm_mon+1,
  	 time_st->tm_mday,
  	 weekName[time_st->tm_wday],
  	 time_st->tm_hour,
  	 time_st->tm_min,
  	 time_st->tm_sec,
  	 myTime.tv_usec
  	 );
  //fclose(outputfile);
  return 0;
}


int split(char *str, const char *delim, char *outlist[]) {
  char    *tk;
  int     cnt = 0;
  
  tk = strtok( str, delim );
  while( tk != NULL && cnt < MAXITEM ) {
    outlist[cnt++] = tk;
    tk = strtok( NULL, delim );
  }
  return cnt;
}

static int proxy_server(int server_port,char* server ,int client_port, int steal_pid)
{
  int s1;
  struct timeval t_val = {0, 1000};
  int select_ret, select_ret_client;
  char buf[BUFSIZ];
  //  char str1[128]={0};  
  
  fd_set fds, readfds, fds_client, readfds_client;
  int accept_list[5];
  int i, j, c;
  
  s1 = server_socket_procedure(client_port);
  
  //********for self client variation********
  int s[FD_SETSIZE];
  char game_buf[BUFSIZ];
  int fork_num = 0;
  //***********************************
  struct sock_list {
    int client[5];
    int server;
  };
  struct sock_list table[5];
  
  s[fork_num++] = client_socket_procedure(server,server_port);
  table[0].server = s[0];
  
  FD_ZERO(&readfds);
  FD_SET(s1, &readfds);
  
  int pid;
  while (1) {
    memcpy(&fds, &readfds, sizeof(fd_set));
    select_ret = select(s1+accept_list[0]+1, &fds, NULL, NULL, &t_val);
    if(select_ret != 0){
      if(FD_ISSET(s1, &fds)){ /* manage accept list */
    	struct sockaddr_in client;
    	socklen_t len = sizeof(client);
    	int client_sock = accept(s1, (struct sockaddr *)&client, &len);
    	if(client_sock != 0){
    	  j=0;
    	  while(j < FD_SETSIZE && accept_list[j] != 0) j++;
    	  if(j != FD_SETSIZE){
    	    FD_SET(client_sock, &readfds);
    	    accept_list[j] = client_sock;
	    table[0].client[j] = accept_list[j];
	    printf("table[0].client[j] = %d\n", table[0].client[j]);
	    /* my addition */
	    
	    /* c=0; */
	    /* while(c < 5 && table[0].client[c] != 0) c++; */
	    /* if(c != 5){ */
	    /*   printf("table[%d].client=%d\n", j, table[j].client[c]); */
	    /*   table[0].client[c] = accept_list[j]; */
	    /* } */
	    
	    /********************/
    	    printf("accept\n");
    	  }else{
    	    printf("no empty\n");
    	  }
    	}else{
    	  printf("accept error\n");
    	}
      }
      /* Write for server from client  (client -> proxy -> server) */
      int w, v;
      for(w=0 ; w < fork_num ; w++){
	for(v=0 ; v < sizeof(table[w].client)/sizeof(int) ; v++){
	  if(FD_ISSET(table[w].client[v], &fds)){
	    read(table[w].client[v] ,buf,BUFSIZ);
	    write(table[w].server, buf, sizeof(buf));
	    //printf("%s\n",buf);
	  }
	}
      }
    }
    
    int m, k;
    /*  Read from server input  (server -> proxy -> client) */
    for(k=0 ; k<fork_num ; k++){
      char a_buf[BUFSIZ];
      char str1[128]={0};
      int cnt;
      read(table[k].server,game_buf,BUFSIZ);
      //printf("table[%d].server=%d\n", k, table[k].server);
      for(m = 0 ; m < sizeof(table[k].client)/sizeof(int) ; m++){
	//printf("table[%d].cleint=%d \n", k, table[k].client[m]);
	if(table[k].client[m]>0) write(table[k].client[m], game_buf, sizeof(game_buf));	
      }
    } /* for */

    /*  Connect new port server when forked it  */
    if(strcmp("102", buf) == 0 && fork_flag == 1){
      fork_flag = 0;
      printf("fork\n");
      //calc_time("*********start************");
      int target_pid = -1, ret_pid = -1;
      char cmd[256], buff[1024], buff2[1024];
      calc_time("find_pid");
      /************   ruby program find pid  *********/
      sprintf(cmd, "/home/ichikawa/.rbenv/shims/ruby find.rb %d", 0);
      FILE *fp = popen(cmd, "r");
      while (fgets(buff, sizeof(buff), fp)) {
	printf("fgets: %s", buff);
      }
      pclose(fp);
      target_pid = atoi(buff);
      calc_time("request_fork");
      /************   request fork  *********/
      syscall(SYS_REQUEST_FORK, target_pid);  //fork_request to kernel
      
      printf("get new socket\n");
      calc_time("get_new_socket");
      /*****  get new socket  *****/
      s[fork_num] = client_socket_procedure("172.21.16.42",22222);
      table[fork_num].client[0] = table[0].client[1];
      table[0].client[1] = 0;
      table[fork_num].server = s[fork_num];
      printf("s[%d], table[%d].client[0]=%d\n", fork_num, fork_num, table[fork_num].client[0]);
      fork_num++;
      
      printf("switch fd\n");
      //read(table[fork_num].server, game_buf,BUFSIZ);
      sleep(1);
      calc_time("switch_fd_after_sleep");
      /**** switch fd ***/
      sprintf(cmd, "/home/ichikawa/.rbenv/shims/ruby find.rb %d", 1);
      FILE *fp2 = popen(cmd, "r");
      while (fgets(buff2, sizeof(buff2), fp2)) {
	printf("fgets: %s", buff2);
      }
      printf("read ok\n");
      pclose(fp2);
      ret_pid = atoi(buff2);
      syscall(SYS_SWITCH_FD, ret_pid, steal_pid);  //switch old and new fd

      /* printf("kill start\n"); */
      /* sprintf(cmd, "kill -9  %d", steal_pid); */
      /* system(cmd); */
      printf("end\n");
      calc_time("**********end************");
    } //strcmp
  } //while 
  exit(0);
  close(s1);    
  //  close(s2_a);
  return 0;
}

int main(int argc,char *argv[])
{
  //  int port = PORT_NO;
  int server_port;
  int client_port;
  int steal_pid;
  
  if( argc<2 ) {
    //    port = PORT_NO;
    return printf("Please input port Number/n");
  } else {
    server_port = atoi(argv[1]);
    client_port = atoi(argv[3]);
    steal_pid = atoi(argv[4]);
  }
  printf("for server port no. = %d\n", server_port);
  printf("for client port no. = %d\n", client_port);
  return  proxy_server(server_port, argv[2],client_port, steal_pid);
}

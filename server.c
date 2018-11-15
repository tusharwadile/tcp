#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_HOST_ADDR  "localhost"
#define SERVER_TCP_PORT	  7000
#define MAXBUF 256

main()
{
	char buf[MAXBUF];
	int listenSocket,acceptSocket,clientAddrLen,pid;
	int bind_value;
	int reuse = 1;

	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;

	listenSocket=socket(PF_INET,SOCK_STREAM,0);
	if(listenSocket < 0)
		printf("Server: cannot open socket stream\n") ;

	serverAddr.sin_family = PF_INET ;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY) ;
	serverAddr.sin_port = htons(SERVER_TCP_PORT) ;

	bind_value = bind(listenSocket, (struct sockaddr*)&serverAddr,sizeof(serverAddr)) ;
	if(bind_value < 0)
		printf("Server: Cannot bind to local address.\n") ;

	setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	listen(listenSocket, 5) ;

	clientAddrLen = sizeof(clientAddr) ;

	if(listenSocket < 0)
		printf("Server: Cannot accept client request\n") ;

	for(;;)
		{
			acceptSocket=accept(listenSocket, (struct sockaddr*)&clientAddr,&clientAddrLen) ;
			if(acceptSocket < 0)
			{
			  printf("Server: Cannot accept client request\n") ;
			  exit(0);
			}

			pid=fork();
			if(pid != 0)   /* for parent */
			{
				close(acceptSocket);
				continue;
			}
			else		/* for child	*/
			{
				close(listenSocket);
				memset(buf,0,MAXBUF);
				while(read(acceptSocket,buf,MAXBUF)>0)
				{
					printf("Message Received\n");
					send(acceptSocket,buf,strlen(buf),0);
					if(strstr(buf,"exit") != NULL)
					  break;
					memset(buf,0,MAXBUF);
				}
				close(acceptSocket);
				shutdown(acceptSocket,SHUT_RDWR);
				exit(1);
			}
		}
}

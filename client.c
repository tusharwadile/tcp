#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_HOST_ADDR  "192.168.43.91"
#define SERVER_TCP_PORT	  7000
#define MAXBUF 256

main()
{
	char buf[MAXBUF];
	int activeSocket;
	int len,n;
	int connect_value;
	struct sockaddr_in remoteAddr;

 	activeSocket=socket(PF_INET,SOCK_STREAM,0);
	if(activeSocket < 0)
		printf("Client: cannot open socket stream\n") ;

	remoteAddr.sin_family = PF_INET ;
	remoteAddr.sin_addr.s_addr = inet_addr(SERVER_HOST_ADDR) ;
	remoteAddr.sin_port = htons(SERVER_TCP_PORT) ;

	connect_value = connect(activeSocket, (struct sockaddr *)&remoteAddr,sizeof(remoteAddr)) ;
	if(connect_value < 0)
	{
		printf("Client: cannot connect to remote server...\n") ;
		exit(0);
	}

	printf("\n\tConnection Established ...") ;

	while(1)
	{
	 	printf("\n\tType your data (\"exit\" to stop) : ") ;
		fgets(buf, MAXBUF, stdin) ;
		len = strlen(buf);
		n = write(activeSocket, buf, len) ;
		//fprintf(stdout,"%s\n",buf);
		if(n<0)
			printf("Client: Write fault error on socket...\n");

		if(strstr(buf,"exit") != NULL)
		{
		  fprintf(stdout,"\tECHO: %s\n","Exiting...");
		  shutdown(activeSocket,SHUT_RDWR);
		  break;
		}
		else
		{
		  recv(activeSocket,buf,MAXBUF,0);
		  fprintf(stdout,"%s\n",buf);
		}
	}
}


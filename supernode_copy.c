

/* SUPERNODE FILE */

#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include<arpa/inet.h>

char buffer[1024];
char buffer1[1024];

int i=0,j;
char * reg(char buffer[1024]);
char * connect(char buffer[1024]);
void *more_supernode(void*);
char * find_entries(char * u_name);
char u_name1[20][20],ipaddr1[20][20],port1[20][20];
char *temp1,*temp2;


int main( int argc, char *argv[] ) 
{

	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in s_addr; 
	struct sockaddr_in cli_addr;
	int  n;
	char * token, *tok, u_name[20],ip_add[50],port[10];
	int totalsupernode,k;
	char supernodeip[10];
	pthread_t bthread;
	pthread_create(&bthread,NULL,&more_supernode,NULL);
    
	/* First call to socket() function */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
	perror("ERROR opening socket\n");
	exit(1);
	}

	/* Initialize socket structure */
	bzero((char *) &s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = INADDR_ANY;
	s_addr.sin_port = htons(atoi(argv[1]));

	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0) {
	perror("ERROR on binding\n");
	exit(1);
	}
	printf("1");

	/* Now start listening for the clients, here process will
	* go in sleep mode and will wait for the incoming connection
	*/

	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	setbuf(stdout,NULL);
    
	while(1)
	{
		//printf("1...\n");
		/* Accept actual connection from the client */
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		printf("1");
		if (newsockfd < 0) {
			perror("ERROR on accept\n");
			exit(1);
			}
		printf("12\n");
		bzero(buffer,1024);
		n = read( newsockfd,buffer,255 );
		printf("reading buffer %s",buffer);
		if (n < 0) {
			perror("ERROR reading from socket\n");
			exit(1);
			}
		token = strtok(buffer," ");
		printf("token outside %s\n",token);
		if(strcmp(token,"reg")== 0)
		{
			printf("about to register\n");
			temp1 = reg(token);
		n = write(newsockfd,"I got your message",18);

		if (n < 0)
        {
			perror("ERROR writing to socket\n");
			exit(1);
			}
		}
		printf("\n Before conn\n");
		if(strcmp(token,"conn")==0)
		{
			printf("about ot coonect\n");
			bzero((char *)&buffer1,sizeof(buffer1));
			sprintf(buffer1, "%s", connect(token));
			printf("value in temp2: %s\n",buffer1);
            
	 		/* Write a response to the client */
			n = write(newsockfd,buffer1,sizeof(buffer1));
		}
		

		if (n < 0)
        {
			perror("ERROR writing to socket\n");
			exit(1);
        }

		/* If connection is established then start communicating */


	}
close(sockfd);

return 0;
}

//FUNCTION FOR THE REGISTRATION ON SUPERNODE BY USER

// function for User registration on supernode

char * reg(char * token)
{
	char * tok, u_name[50],ip_add[50],port[50];
	int s_sockfd;
  	memset(u_name,0,50);
	memset(ip_add,0,50);
	memset(port,0,50);
	tok = strtok(NULL," ");
	strcpy(u_name,tok);
	strcpy(u_name1[i],u_name);
	printf("u_name = %s\n",u_name);
	tok = strtok(NULL," ");
	strcpy(ipaddr, tok);
	strcpy(ipaddr1[i],ip_add);
   	printf("IPaddress = %s\n",ip_add);
	tok = strtok(NULL," ");
	strcpy(port, tok);
	strcpy(port1[i],port);
   	printf("Port number = %s\n",port);
   	for(int k=0;k<=i;k++)
	{
	printf("nPrinting all details about u_name");	
	printf("%s,%s,%s\n",u_name1[k],ipaddr1[k],port1[k]);
	}
	i++;
  
return "0";
}

//FUNCTION TO FIND A USERNAME FOUND IN THE ENTRIES OF ONE SUPERNODE THEN RETURN ITS IP OR ASK ANOTHER SUPERNODE WHICH DOES THE SAME THING

/* Function to find username in supernode and get IP address of that
 or contact other supernode to find user name */
char * connect(char * token)
{
	char * tok;
    char findname[20],findipaddr[20],findport[20];
	char * send_buffer = malloc(500);
	char send_buffer1[1024];
	char receive_buffer[1024];
	char * supernodeip = malloc(20);
	char  u_name[20];
	int user_not_found = 0,portno2, user_found = 0;
	int s_sockfd,returnStatus,ret;
	struct sockaddr_in s_addr; 
	struct sockaddr_in cli_addr;
	printf("content in buffer: %s\n",token);
	tok = strtok(NULL," ");
	strcpy(u_name,tok);
	printf("To find %s\n",u_name);
		
	for(j=0;j<=i;j++)
	{
	printf("%d,%s,%s,%s\n",j,u_name1[j],ipaddr1[j],port1[j]);
	}
	
	int n =0;
	while(n<20)
    {
            if(strcmp(u_name1[n], u_name) == 0) 
			{
                printf("Found %s!\n",u_name);
				strcpy(findname,u_name1[k]); 
				strcpy(findipaddr,ipaddr1[k]);
				strcpy(findport,port1[k]);
				printf("%s,%s,%s\n",findname,findipaddr,findport);
				memset(send_buffer, '\0', sizeof( send_buffer ));
				strcpy(send_buffer,findname);
				strcat(send_buffer," ");
				strcat(send_buffer,findipaddr);
				strcat(send_buffer," ");
				strcat(send_buffer,findport);
				user_found = 1;
                break;
	        }
		n++;
	}
	if(user_found != 1)
	{		
		printf("contact other supernodes\n");
		strcpy(send_buffer1,"find ");
		strcat(send_buffer1,u_name);
		printf("send_buffer1:%s\n",send_buffer1);
		while(user_not_found == 0)
		{
			
			s_sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (s_sockfd == -1) 
               {
               fprintf(stderr, "Cannot create a socket!\n");
               exit(1);
               } 
               else 
               {
               fprintf(stderr, "Socket is created!\n");
               }
			
			printf("Enter other supernode IP:\n");
			scanf("%s",supernodeip);
			printf("Enter its PORT_NO:\n");	
			scanf("%d",&portno2);
			bzero((char *)&s_addr,sizeof(s_addr));
			returnStatus = inet_aton(supernodeip, &s_addr.sin_addr);
			s_addr.sin_family = AF_INET;
			s_addr.sin_port = htons(portno2);

		  // connect to the address and port with our socket
			ret= connect(s_sockfd,(struct sockaddr *) &s_addr, sizeof(s_addr));
			
				
			if(write(s_sockfd,send_buffer1,sizeof(send_buffer1))!= sizeof(send_buffer1))
			{
				printf("error in writing");
				exit(0);
			}
			if(read(s_sockfd, receive_buffer, sizeof(receive_buffer)) < 0)
			{
				perror("error in reading\n");
				exit(0);
			}
			printf("receive buffer:%s\n", receive_buffer);
			//compare if user_not_found = 'N'
			if(strcmp(receive_buffer,"N") == 0)
			{
				user_not_found = 0;
			}
			else
			{
				user_not_found = 1;
				strcpy(send_buffer,receive_buffer);
				printf("send_buffer:%s\n", send_buffer);
			}
			
	
			
		}
		close(s_sockfd);
	}
	return send_buffer;
}

//FUNCTION TO SWITCH TO ANOTHER SUPERNODE

void *more_supernode(void *ptr)
{
int sockfd, newsockfd, portno, clilen;
struct sockaddr_in s_addr; 
struct sockaddr_in cli_addr;
char * token, * u_name;
char receive_buffer[1024];
char * unameresult;

	/* First call to socket() function */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		perror("ERROR opening socket\n");
		exit(1);
	}

	/* Initialize socket structure */
	bzero((char *) &s_addr, sizeof(s_addr));
	//portno = 2626;
	
	printf("Enter the port number\n");
	scanf("%d",&portno);
	//PORT_NO = portno;

	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = INADDR_ANY;
	s_addr.sin_port = htons(portno);

	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0) {
		perror("ERROR on binding\n");
		exit(1);
	}
	printf("Thread Running\n");

	/* Now start listening for the clients, here process will
	* go in sleep mode and will wait for the incoming connection
	*/

	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	setbuf(stdout,NULL);
	
	while(1)
	{
		printf("Thread Inside while\n");
		/* Accept actual connection from the client */
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		printf("Thread after accept");
		if (newsockfd < 0) 
		{
			perror("ERROR on accept\n");
			exit(1);
		}
			
		if(read(newsockfd, receive_buffer, sizeof(receive_buffer)) < 0)
		{
			perror("Read Error\n");
			exit(0);
		}
		printf("Thread - the recvbuff is %s\n",receive_buffer);		
		token = strtok(receive_buffer," ");
		printf("Thread - the token is %s\n",token);
		if(strcmp(token,"search") == 0)
		{
			u_name = strtok(NULL," ");
			char send_results[1024];
			sprintf(send_results, "%s", find_entries(u_name));
			printf("send_results: %s\n",send_results);
						
			if(write(newsockfd,send_results,sizeof(send_results))!= sizeof(send_results))
			{
			printf("write error");
			exit(0);
			}				
		}
	}
}

//FUNCTION TO SEARCH FOR ENTRIES IN A SUPERNODE

/function search users in supernode

char * find_entries(char * u_name)
{

char * tok;
char findname[20],findipaddr[20],findport[20];
char * send_buffer = malloc(500);
int user_found = 0,i=0;

	while(u_name1[i] != NULL)
		{
            if(strcmp(u_name1[i], u_name) == 0) 
			{
				
				printf("username found %s!\n",u_name);
				strcpy(findname,u_name1[i]); 
				strcpy(findipaddr,ipaddr1[i]);
				strcpy(findport,port1[k]);
				printf("%s,%s,%s\n",findname,findipaddr,findport);
				memset( send_buffer, '\0', sizeof( send_buffer ));
				strcpy(send_buffer,findname);
				strcat(send_buffer," ");
				strcat(send_buffer,findipaddr);
				strcat(send_buffer," ");
				strcat(send_buffer,findport);
				printf("%s\n",send_buffer);
				user_found = 1;
				break;
			}
		k++;
		}
	if(user_found != 1)
	{
		send_buffer = "N";
	}
	return send_buffer;
}


// This is the echo SERVER server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <sys/socket.h>
#include <netdb.h>

#define SINGLE 0
#define INIT 1
#define SEND 2
#define DONE 3

#define  MAX 512
#define SERVER_HOST "localhost"
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 1234

// Define variables:
struct sockaddr_in  server_addr, client_addr;
int mysock, csock;
int r, len, n;

char home[MAX];
char wd[MAX];

// Server initialization code:
int server_init()
{
   printf("==================== server init ======================\n");   
   // get DOT name and IP address of this host
   printf("1 : create a TCP STREAM socket\n");
   mysock = socket(AF_INET, SOCK_STREAM, 0);
   
   printf("2 : fill server_addr with host IP and PORT# info\n");
   // initialize the server_addr structure
   server_addr.sin_family = AF_INET;                  // for TCP/IP
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // THIS HOST IP address  
   server_addr.sin_port = htons(SERVER_PORT);         // port number

   printf("3 : bind socket to server address\n");
   // bind syscall: bind the socket to server_addr info
   r = bind(mysock,(struct sockaddr *)&server_addr, sizeof(server_addr));
   if (r < 0){
       printf("bind failed\n");
       exit(3);
   }
   printf("host = %s port = %d\n", SERVER_HOST, SERVER_PORT);
   
   printf("4 : server is listening ....\n");
   listen(mysock, 5); // listen queue length = 5
   printf("===================== init done =======================\n");
}


int main(int argc, char *argv[])
{
	char line[MAX], ans[MAX], *cmd, *path;
  	getcwd(home, sizeof(home));
  	getcwd(wd, sizeof(wd));
  	printf("\n Working out of %s \n", wd);
  	server_init(); 
   
   while(1){    // Try to accept a client request
     printf("server: accepting new connection ....\n"); 

     // Try to accept a client connection as descriptor csock
     len = sizeof(client_addr);
     csock = accept(mysock, (struct sockaddr *)&client_addr, &len);
     if (csock < 0){
        printf("server: accept error\n");
        exit(1);
     }
     printf("server: accepted a client connection from\n");
     printf("-----------------------------------------------\n");
     printf("Client: IP=%s  port=%d\n", "127.0.0.1",
                            ntohs(client_addr.sin_port));
     printf("-----------------------------------------------\n");

     // Processing loop: csock <-- data --> client
     while(1){
       n = read(csock, line, MAX);
       if (n==0){
           printf("server: client died, server loops\n");
           close(csock);
           break;
      }
      // show the line string
      printf("server: read  n=%d bytes; line=[%s]\n", n, line);
      
      cmd = strtok(line, " ");
      path = strtok(NULL, "\n");
      procCmd(cmd, path, line);

      // process the line from clinet
      //strcat(line, " ECHO");
      // send the echo line to client 
      //n = write(csock, line, MAX);
      //sockPrint(line);

      //printf("server: wrote n=%d bytes; ECHO=[%s]\n", n, line);
      printf("server: ready for next request\n");
    }
 }
}

int sockPrint(char *line, int status){
	int n;
	char buf[MAX];
	char ans[MAX];
	char ping[MAX];
	
	bzero(buf,MAX);
	bzero(ans,MAX);
	bzero(ping,MAX);
	if(line != NULL)
		strncpy(buf, line, sizeof(buf) -1);
	
	if(status == SINGLE){

		strncpy(ping, "!init!", sizeof(ping) -1);
		write(csock, ping, MAX); //check that read is ready
		
		read(csock, ans, MAX);
		
		if(!strcmp(ans, "!ready!")){
		
			n = write(csock, buf, MAX);
			
			strncpy(ping, "!done!", sizeof(ping) -1);
			write(csock, ping, MAX);
			
			printf("SockPrint: sent n=%d %s\n", n, buf);
		}
		else{
			printf("Invalid ping return: (%s)", ans);
			return -1;
		}
	}

	else if(status == INIT){
		strncpy(ping, "!init!", sizeof(ping) -1);
		write(csock, ping, MAX); //check that read is ready
		
		read(csock, ans, MAX);
		
		if(!strcmp(ans, "!ready!")){
			if(buf != NULL)
				n = write(csock, buf, MAX);
		}
		else{
			printf("Failed to initialize print session");
		}
	}
	else if(status == SEND){
		write(csock, buf, MAX);
	}
	else if(status == DONE){
		if(buf != NULL)
				n = write(csock, buf, MAX);
		
		strncpy(ping, "!done!", sizeof(ping) -1);
		write(csock, ping, MAX); //check that read is ready
	}
	else{
		printf("Invalid status\n");
		return -1;
	}
	return 0;
}



int procCmd(char *cmd, char *path, char *line){ //return 0: lcmd, 1:lcmd success, -1:error
	char cd[MAX];
	char buf[MAX];
	
	if(!strcmp(cmd, "ls")){
		return doLs(path);
	}
	
	/*
	if (!path){
		printf("path=null\n");
		return -1;
	}
	*/
	
	if(!strcmp(cmd, "cd")){//cd
		if(chdir(path)){
			sockPrint("Failed to cd\n", SINGLE);
			return -1;
		}
		else{
			strcat(wd,"/");
			strcat(wd, path);
			return 0;
		}
	}
	else if(!strcmp(cmd, "pwd")){//pwd
		sprintf(buf, "\n%s\n",wd);
		sockPrint(buf, SINGLE);
	}
	else if(!strcmp(cmd, "mkdir")){//mkdir
		if(mkdir(path, 0777))
			sockPrint("Error Creating directory\n", SINGLE);
		return 0;
	}
	else if(!strcmp(cmd, "rmdir")){//rmdir
		if(rmdir(path))
			sockPrint("Error Erasing directory\n", SINGLE);
		return 0;
	}
	else if(!strcmp(cmd, "rm")){//rm
		if(remove(path))
			sockPrint("Error Erasing file\n", SINGLE);
		return 0;
	}
	else if(!strcmp(cmd, "cat")){//cat
		if(doCat(path))
			sockPrint("Error in cat\n", SINGLE);
		return 0;
	}
	else if(!strcmp(cmd, "get")){//get
		if(doCat(path))
			sockPrint("Error in get\n", SINGLE);
		return 0;
	}
	else if(!strcmp(cmd, "put")){//put
		if(doPut(path)){
			sockPrint("Error in put\n", SINGLE);
			return -1;
		}
		else 
			return 0;
	}
	else{ //error
		sprintf(buf, "(\"%s\": Unknown command\n",cmd);
		sockPrint(buf, SINGLE);
	}
	

}



int doCat(char *path){
	char buf[MAX];
	char dir[MAX];
	sprintf(dir, "%s", wd);
	strcat(dir, "/");
	strcat(dir, path);
	int file = open(dir, O_RDONLY);
	int c;
	
	if(file==-1){ //check if file is opened
		close(file);
		return -1;
	}
	sockPrint("\n", INIT);
	while((c = read(file, buf, MAX)) > 0){
		sockPrint(buf, SEND);
	}
	sockPrint("\n", DONE);
	close(file);
	return 0;
}

int doLs(char *path){
	char buf[MAX];
	struct dirent *der;
	DIR *dir;
	
	if(path != NULL){
		dir = opendir(path);
	}
	else{
		dir = opendir(".");
	}
	
	if(dir == NULL){
		sockPrint("Failed to open Dir\n", SINGLE);
		return -1;
	}
	
	sockPrint("\n", INIT);
	while((der = readdir(dir)) != NULL){
		sprintf(buf, "\t%s\n", der->d_name);
		sockPrint(buf, SEND);
	}
	sockPrint("\n", DONE);
	return 0;
}

int doPut(char *path){
	char ans[MAX];
	char ping[MAX];
	int fileSave = 0;
	FILE *file;
	
	if(path != NULL){
		fileSave = 1;
		char dir[MAX];
		sprintf(dir,"%s/", wd);
		strcat(dir, path);
		file = fopen(dir, "w");
		if(file == NULL){
			printf("Failed to safeRead() to file\n");
			return -1;
		}
	}
	// Read a line from sock and show it
    int n = read(csock, ans, MAX);
    //printf("client: read  n=%d bytes; echo=(%s)\n", n, ans);
    //printf("SafeRead Entered:\n");
    if(!strcmp(ans, "!init!")){ //check that it's initialized
    	//printf("Received init ping\n");
    	strncpy(ping, "!ready!", sizeof(ping) -1);
		write(csock, ping, MAX); //respond ready
		//printf("Sent ready ping\n");
    	while(1){
    		//printf("Read loop entered");
    		bzero(ans, MAX);
    		read(csock, ans, MAX);
    		if(!strcmp(ans, "!done!"))
    			break;
    		if(fileSave)
    			fprintf(file, "%s", ans);
    		else
    			printf("%s",ans);
    	}
    }
    else{
    	return -1;
    }
    return 0;
}


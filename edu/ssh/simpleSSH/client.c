// The echo client client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <sys/socket.h>
#include <netdb.h>

#define MAX          512
#define SERVER_HOST "localhost"
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT  1234

#define SINGLE 0
#define INIT 1
#define SEND 2
#define DONE 3

#define SAVE 1
#define PRINT 0

// Define variables
struct sockaddr_in  server_addr; 
int sock, r;
char home[MAX];
char wd[MAX];

// clinet initialization code
int client_init()
{
  printf("======= clinet init ==========\n");
  
  printf("1 : create a TCP socket\n");
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock<0){
     printf("socket call failed\n");
     exit(1);
  }

  printf("2 : fill server_addr with server's IP and PORT#\n");
  server_addr.sin_family = AF_INET;
  //server_addr.sin_addr.s_addr = htonl((127<<24) + 1);
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);

  // Connect to server
  printf("3 : connecting to server ....\n");
  r = connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r < 0){
     printf("connect failed\n");
     exit(3);
  }
  printf("4 : connected OK to\n"); 
  printf("---------------------------------------------------------\n");
  printf("hostname=%s PORT=%d\n", SERVER_HOST, SERVER_PORT);
  printf("---------------------------------------------------------\n");
  printf("========= init done ==========\n");
}

int main(int argc, char *argv[ ])
{
  int n;
  char line[MAX], copy[MAX], ans[MAX], *cmd, *path;
  getcwd(home, sizeof(home));
  getcwd(wd, sizeof(home));

  client_init();

  printf("********  processing loop  *********\n");
  while (1){
    cmd = NULL;
    path = NULL;
    printf("input a line : ");
    bzero(line, MAX);                // zero out line[ ]
    fgets(line, MAX, stdin);         // get a line (end with \n) from stdin
    line[strlen(line)-1] = 0;        // kill \n at end
    if (line[0]==0)                  // exit if NULL line
       exit(0);
       
    //Process input
	sprintf(copy, "%s", line);
    cmd = strtok(line, " ");
    path = strtok(NULL, "\n");
    if(procCmd(cmd, path, copy) == 1)
    	safeRead(NULL);
    //n = read(sock, ans, MAX);
    //printf("client: read  n=%d bytes; echo=(%s)\n", n, ans);
  }
}

int safeRead(char *path){
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
    int n = read(sock, ans, MAX);
    //printf("client: read  n=%d bytes; echo=(%s)\n", n, ans);
    //printf("SafeRead Entered:\n");
    if(!strcmp(ans, "!init!")){ //check that it's initialized
    	//printf("Received init ping\n");
    	strncpy(ping, "!ready!", sizeof(ping) -1);
		write(sock, ping, MAX); //respond ready
		//printf("Sent ready ping\n");
    	while(1){
    		//printf("Read loop entered");
    		bzero(ans, MAX);
    		read(sock, ans, MAX);
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


int procCmd(char *cmd, char *path, char *line){ 
	char cd[MAX];
	
	if(!strcmp(cmd, "lls")){
		return doLs(path);
	}
	
	/*
	if (!path){
		printf("path=null\n");
		return -1;
	}
	*/
	
	if(!strcmp(cmd, "lcd")){//lcd
		if(chdir(path)){
			printf("Failed to cd\n");
			return -1;
		}
		else{
			strcat(wd,"/");
			strcat(wd, path);
			return 0;
		}
	}
	else if(!strcmp(cmd, "lpwd")){//lpwd
		printf("\n%s\n",wd);
	}
	else if(!strcmp(cmd, "lmkdir")){//lmkdir
		if(mkdir(path, 0777))
			printf("Error Creating directory\n");
		return 0;
	}
	else if(!strcmp(cmd, "lrmdir")){//lrmdir
		if(rmdir(path))
			printf("Error Erasing directory\n");
		return 0;
	}
	else if(!strcmp(cmd, "lrm")){//lrm
		if(remove(path))
			printf("Error Erasing file\n");
		return 0;
	}
	else if(!strcmp(cmd, "lcat")){//lcat
		if(doCat(path))
			printf("Error in lcat\n");
		return 0;
	}
	else if(!strcmp(cmd, "put")){//put
		write(sock, line, MAX);
		if(doPut(path)){
			printf("Error in put\n");
			return -1;
		}
		else
			return 0;
	}
	else if(!strcmp(cmd, "get")){//gwt
		write(sock, line, MAX);
		if(safeRead(path)){
			printf("Failed to safeRead() the given path");
			return -1;
		}
		else
			return 0;
	}
	else{ //server commands
		//strcat(cmd, " ");
		//strcat(cmd, path);
		int n = write(sock, line, MAX);
		//printf("client: wrote n=%d bytes; line=(%s)\n", n, line);
		return 1;
	}
	

}

int doPut(char *path){ //literally just a copy of server's doCat
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
		write(sock, ping, MAX); //check that read is ready
		
		read(sock, ans, MAX);
		
		if(!strcmp(ans, "!ready!")){
		
			n = write(sock, buf, MAX);
			
			strncpy(ping, "!done!", sizeof(ping) -1);
			write(sock, ping, MAX);
			
			printf("SockPrint: sent n=%d %s\n", n, buf);
		}
		else{
			printf("Invalid ping return: (%s)", ans);
			return -1;
		}
	}

	else if(status == INIT){
		strncpy(ping, "!init!", sizeof(ping) -1);
		write(sock, ping, MAX); //check that read is ready
		
		read(sock, ans, MAX);
		
		if(!strcmp(ans, "!ready!")){
			if(buf != NULL)
				n = write(sock, buf, MAX);
		}
		else{
			printf("Failed to initialize print session");
		}
	}
	else if(status == SEND){
		write(sock, buf, MAX);
	}
	else if(status == DONE){
		if(buf != NULL)
				n = write(sock, buf, MAX);
		
		strncpy(ping, "!done!", sizeof(ping) -1);
		write(sock, ping, MAX); //check that read is ready
	}
	else{
		printf("Invalid status\n");
		return -1;
	}
	return 0;
}

int doCat(char *path){
	char buf[MAX];
	char dir[MAX];
	sprintf(dir, "%s", wd);
	strcat(dir, "/");
	strcat(dir, path);
	int file = open(dir, O_RDONLY);
	int c;
	printf("lcat: path=%s wd=%s file=%d\n", path, wd, file);
	if(file==-1){ //check if file is opened
		printf("Failed to open file\n");
		return -1;
	}
	printf("\n");
	while((c = read(file, buf, MAX)) > 0){
		printf("%s",buf, MAX);
	}
	printf("\n");
	close(file);
	return 0;
}

int doLs(char *path){
	struct dirent *der;
	DIR *dir;
	
	if(path != NULL){
		dir = opendir(path);
	}
	else{
		dir = opendir(".");
	}
	
	if(dir == NULL){
		printf("Failed to open Dir\n");
		return -1;
	}
	
	while((der = readdir(dir)) != NULL){
		printf("\t%s\n", der->d_name);
	}
	
	return 0;
}

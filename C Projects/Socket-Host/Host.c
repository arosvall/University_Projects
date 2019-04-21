#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> 
#include <string.h> 
#include <sys/wait.h> 
  
#define bufsize 1024

//code provided below in handout
void eatZombies(int n){
  // This function removes the zombie process state left
  // after the child exits. Learn about this in NWEN 301!

  wait3(NULL,WNOHANG,NULL); // Nom Nom 
}


int main(int argc, char *argv[]){
  int sock, length, msgsock, status;
  struct sockaddr_in server;
  

  // for forking, and cleaning up zombie child state afterwards
  // You must not change this code. 

  pid_t id;       
  signal(SIGCHLD, &eatZombies);
 
  // My code starts here. 
 
 /* Start by creating the socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  
  //check that socket creation hasn't thrown an error 
  if(sock < 0) {
  printf("error in creating the socket \n");
  return -1;
  }	

  // Next, create the socket addressing structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(argv[1])); // this time 1st arg is port#

  //assign the server size to the length variable for convenience
  length = sizeof(server);
  
  
  /* Bind the socket to the server */  
  int bd = bind(sock, &server, length);
  
  //check that if the bind fails the error will be caught
  if(bd < 0) {
  printf("Error occured while binding the socket \n");
  return -1;
  }
  
  // Right, now we are in the main server loop.
  // YOUR CODE HERE   

  /* Set the socket to start listening, the queue max length of pending connections is 5 */
  int lst = listen(sock, 5);
 
  //Check that listening has not caused a negative number to be returned 
  if(lst < 0 ) {
  printf("Error occured while listening \n");
  return -1;
  }
  
  while(1){
  
    /*accept the connection request, passing the socket, server and server size with a new int storing the newly created socket */
    int msgsock = accept(sock, &server, &length);

	/*check the acception has occured successfully, printing an erorr message and exiting the program if it has not */
	if(msgsock < 0) {
	printf("Error occured while accepting request \n");
	return -1;
	}
    

    // the next call makes a new child process that will actually handle the client.
    id = fork();
	
	/*check return value from id, if it is a negative number then an error has occured.
	*/
	if(id < 0) {
	printf("Error occured while forking \n");
	return -1;
	}
	/* if the id is 0, we are in the child and need to do the work for the server */
	else if(id ==0) {
	
	//create a new char array (buffer) to hold the passed in string.
	char buf[bufsize];
	
	/* Read information in from the socket in the form of a string */
	status =  read(msgsock, &buf, bufsize);	
	
	// Check the status has not returned a negative number indicating an error
	if(status < 0) {
	printf("Error occured while reading from socket \n");
	return -1;
	}
	
	/* change the string, capitalising all the letters in the passed string (Duplication of the SHOUTING server) */	
	int idx = 0;
	//while not reaching the null terminator
	while(buf[idx] != '\0') {
	    char c = buf[idx];
		//if the letter is lowercase, assign it to be uppercase
		if(c >= 'a' && c <= 'z') {
		    buf[idx] = c - 32;
		}
		idx++;
	}
	
	/* Write the changed string back to the original client socket, using the write() function */
	status = write(msgsock, buf, bufsize);
	
	//Check that no error occured while writing the string back to the client	
	if(status < 0) {
	printf("Error occured while writing to socket \n");
	return -1;
   }
    /* Now that the socket has received the string, close the socket */
	close(msgsock);	
	}
	
  }
}

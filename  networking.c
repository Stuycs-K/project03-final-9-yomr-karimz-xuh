#include "networking.h"

/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {

  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;


  //getaddrinfo
  getaddrinfo(server_address, PORT, hints, &results);
  
  int serverd;//store the socket descriptor here
  //create the socket
  serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (serverd < 0) {
    perror("socket creation in client failed\n");
  }

  
  //connect to the server

  int connection = connect(serverd, results->ai_addr, results->ai_addrlen);
  if (connection < 0) {
    perror("connection failed in client\n");
  }
  
  free(hints);
  freeaddrinfo(results);

  return serverd;
}

/*Accept a connection from a client
 *return the to_client socket descriptor
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);
   
    //accept the client connection
    client_socket = accept(listen_socket, (struct sockaddr *)&client_address, &sock_size);
    if (client_socket < 0) {
      perror("accept failed in server\n");
    }
    return client_socket;
}

/*Create and bind a socket.
* Place the socket in a listening state.
*/
int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  
  //create the socket
  int clientd;//store the socket descriptor here
  getaddrinfo(NULL, PORT, hints, &results); 
  clientd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (clientd < 0) {
    perror("socket creation in setup failed\n");
  }

  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");
  
  int connection = bind(clientd, results->ai_addr, results->ai_addrlen);
  if (connection < 0) {
    perror("connection failed in setup\n");
  }
  
  int listenState = listen(clientd, 10);
  if (listenState < 0) {
    perror("listen failed in setup\n");
  }
  
  free(hints);
  freeaddrinfo(results);
  
  return clientd;
}

void err(int i, char*message){
  if(i < 0){
	  printf("Error: %s - %s\n",message, strerror(errno));
  	exit(1);
  }
}

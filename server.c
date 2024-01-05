#include "networking.h"



void subserver_logic(int client_socket){
  char buffer[BUFFER_SIZE];
  int bytes_read;


  bytes_read = read(client_socket, buffer, sizeof(buffer));
  if (bytes_read <= 0) {
    printf("client disconnected\n");
    exit(0);
  }


  write(client_socket, buffer, bytes_read);
  printf("Sent back to client: %s\n", buffer);
}



int main(int argc, char *argv[] ) { 

  int client_socket = server_setup(); 


  while (1) {
    int subserver_socket = server_tcp_handshake(client_socket);

    int process = fork();
    if (process < 0) {
      perror("Forking error\n");
      exit(1);
    }


    else if (process == 0) {
      

      
      printf("[server] connected to client\n");
      subserver_logic(subserver_socket);
      exit(0);

    }

    else {

      close(subserver_socket);
    }
    
    

  }
  
  return 0;
}

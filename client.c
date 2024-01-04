#include "networking.h"

void clientLogic(int server_socket){

  char buffer[BUFFER_SIZE];
  int bytes_read;

  
  printf("Enter a message: ");
  fgets(buffer, sizeof(buffer), stdin);
  int i = 0;
  while (buffer[i]) {
    if (buffer[i] == '\n') buffer[i] = '\0';
    i++;
  }
  write(server_socket, buffer, strlen(buffer) + 1);
  bytes_read = read(server_socket, buffer, sizeof(buffer));
  if (bytes_read <= 0) {
    printf("server disconnected\n");
    exit(0);
  }

  printf("Received from server: %s\n", buffer);  
  close(server_socket);

}

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");
  clientLogic(server_socket);
}

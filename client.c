#include "networking.h"

// sighandler to handle game ending
void game_end(int signum) {
  
}

void clientLogic(int server_socket){

  char question_buffer[BUFFER_SIZE];
  char response_buffer[BUFFER_SIZE];
  int bytes_read;
  int current_question_number = 0;

  // wait for game to start, block until server sends a question
  printf("Waiting for game to start...");
  bytes_read = read(server_socket, question_buffer, sizeof(question_buffer));
  if (bytes_read <= 0) {
    printf("server disconnected\n");
    exit(0);
  }

  // once server sends the first question, loop
  while (1) {
    // read question
    bytes_read = read(server_socket, question_buffer, sizeof(question_buffer));
    if (bytes_read <= 0) {
      printf("server disconnected\n");
      exit(0);
    }

    // handle game ending signal

    printf("Question %d: %s\n", current_question_number, question_buffer); // print question to client
    fgets(response_buffer, sizeof(response_buffer), stdin); // read client response from command line
    int i = 0;
    while (response_buffer[i]) {
      if (response_buffer[i] == '\n') response_buffer[i] = '\0';
      i++;
    }
    write(server_socket, response_buffer, sizeof(response_buffer)); // write the response back to the server

    current_question_number++;
  }

  // display final scores at the end

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

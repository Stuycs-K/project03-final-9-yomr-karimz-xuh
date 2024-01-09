#include "networking.h"

// sighandler to handle game ending
void game_end(int signum) {
  printf("\n game is over! displaying final scores of players...");
  exit(0);
}

void clientLogic(int server_socket){

  char question_buffer[BUFFER_SIZE];
  char response_buffer[BUFFER_SIZE];
  int bytes_read;
  int current_question_number = 0;

  // wait for game to start, block until the server sends a question
  printf("Waiting for game to start...");
  bytes_read = read(server_socket, question_buffer, sizeof(question_buffer));
  if (bytes_read <= 0) {
    printf("Server disconnected\n");
    exit(0);
  }

  // once the server sends the first question, loop
  while (1) {
    // read question
    bytes_read = read(server_socket, question_buffer, sizeof(question_buffer));
    if (bytes_read <= 0) {
      printf("Server disconnected\n");
      exit(0);
    }

    // handle game ending signal
    if (strcmp(question_buffer, "GAME_END") == 0) {
      printf("Game has ended. Displaying final scores...\n");
      while (1) {
        bytes_read = read(server_socket, response_buffer, sizeof(response_buffer));
        if (bytes_read <= 0) {
          printf("Server disconnected\n");
          exit(0);
        }

        // Check if the server has finished sending scores
        if (strcmp(response_buffer, "END_SCORES") == 0) {
          break;
        }

        printf("%s\n", response_buffer);
      }

      exit(0);
    }

    printf("Question %d: %s\n", current_question_number, question_buffer); // print question to client
    fgets(response_buffer, sizeof(response_buffer), stdin); // read client response from the command line
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

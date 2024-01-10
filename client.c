#include "networking.h"

// sighandler to handle game ending
void game_end(int signum) {
  printf("\n game is over! displaying final scores of players...");
  exit(0);
}

long getTimeDifference(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
}




// function to check if the player's answer is correct and award points
int pointSystem(struct questionAndOptions* question, char* playerAnswer, struct timeval startTime) {
    struct timeval endTime;
    gettimeofday(&endTime, NULL);

    // Check if the player's answer is correct
    if (strcmp(playerAnswer, question->correctAnswer) == 0) {
        // Calculate time taken by the player
        long timeTaken = getTimeDifference(startTime, endTime);

        // award points based on time taken 
        int points = 0;
        if (timeTaken < 5000) {
            points = 100;
        } else if (timeTaken < 10000) {
            points = 50;
        } else {
            points = 25;
        }

        printf("Correct! Awarded %d points.\n", points);
        return points;
    } 
    else {
        printf("Incorrect! No points awarded.\n");
        return 0;
    }
}

void clientLogic(int server_socket){

    struct questionAndOptions * question_buffer;
    char response_buffer[BUFFER_SIZE];
    int score = 0;

    int bytes_read;
    int current_question_number = 0;

    // wait for game to start, block until server sends a question
    bytes_read = read(server_socket, response_buffer, sizeof(response_buffer));
    if (bytes_read <= 0) {
        printf("server disconnected LOL\n");
        exit(0);
    }

    printf("%s\n", response_buffer);

  // once server sends the first question, loop
    while (1) {
        // read question
        bytes_read = read(server_socket, response_buffer, sizeof(response_buffer));
        if (bytes_read <= 0) {
            printf("server disconnected\n");
            exit(0);
        }

        // handle game ending signal
        signal(SIGUSR1, game_end);

        struct timeval start_time;
        gettimeofday(&start_time, NULL);

        int i = 0;
        printf("Question %d: %s\n", current_question_number, response_buffer); // print question to client
        fgets(response_buffer, sizeof(response_buffer), stdin); // read client response from command line
        while (response_buffer[i]) {
            if (response_buffer[i] == '\n') response_buffer[i] = '\0';
            i++;
        }

        int add_points = pointSystem(question_buffer, response_buffer, start_time);
        score = score + add_points;

        write(server_socket, score, sizeof(response_buffer)); // write the client's total score back to the server

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

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
int pointSystem(char* correct_answer_buffer, char* playerAnswer, struct timeval startTime, char* optionA, char* optionB, char* optionC, char* optionD) {
    struct timeval endTime;
    gettimeofday(&endTime, NULL);

    if (strcmp(playerAnswer, "A") == 0) {
        strcpy(playerAnswer, optionA);
    }
    else if (strcmp(playerAnswer, "B") == 0) {
        strcpy(playerAnswer, optionB);
    }
    else if (strcmp(playerAnswer, "C") == 0) {
        strcpy(playerAnswer, optionC);
    }
    else if (strcmp(playerAnswer, "D") == 0) {
        strcpy(playerAnswer, optionD);
    }

    // Check if the player's answer is correct
    if (strcmp(playerAnswer, correct_answer_buffer) == 0) {
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

    //struct questionAndOptions * question_buffer;
    char response_buffer[BUFFER_SIZE];
    char correct_answer_buffer[BUFFER_SIZE];
    char optionA[BUFFER_SIZE];
    char optionB[BUFFER_SIZE];
    char optionC[BUFFER_SIZE];
    char optionD[BUFFER_SIZE];
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


    //printf("Here1!\n");
  // once server sends the first question, loop
    while (1) {
        int i = 0;
        //printf("Here!\n");

        // read question
        bytes_read = read(server_socket, response_buffer, sizeof(response_buffer));
        if (bytes_read <= 0) {
            printf("server disconnected for question string\n");
            exit(0);
        }
        // handle game ending signal

        signal(SIGUSR1, game_end);

        printf("Question %d: %s\n", current_question_number+1, response_buffer); // print question to client
        fgets(response_buffer, sizeof(response_buffer), stdin); // read client response from command line
        while (response_buffer[i]) {
            // uppercase the response
            if (response_buffer[i] >= 'a' && response_buffer[i] <= 'z') {
                response_buffer[i] = response_buffer[i] - 32;
            }

            i++;
        }

        printf("Your answer: %s\n", response_buffer); // print client response to client


        bytes_read = read(server_socket, correct_answer_buffer, sizeof(correct_answer_buffer));
        if (bytes_read <= 0) {
            printf("server disconnected for question\n");
            exit(0);
        }
        // remove newline from correct answer
        i = 0;
        while (correct_answer_buffer[i]) {
            if (correct_answer_buffer[i] == '\n') correct_answer_buffer[i] = '\0';
            i++;
        }

        

        printf("Got to A\n");
        bytes_read = read(server_socket, optionA, sizeof(optionA));
        if (bytes_read <= 0) {
            printf("server disconnected for optionA\n");
            exit(0);
        }
        i = 0;
        while (optionA[i]) {
            if (optionA[i] == '\n') optionA[i] = '\0';
            i++;
        }
        

        printf("Got to B\n");
        bytes_read = read(server_socket, optionB, sizeof(optionB));
        if (bytes_read <= 0) {
            printf("server disconnected for optionB\n");
            exit(0);
        }
        i = 0;
        while (optionB[i]) {
            if (optionB[i] == '\n') optionB[i] = '\0';
            i++;
        }
        bytes_read = read(server_socket, optionC, sizeof(optionC));
        if (bytes_read <= 0) {
            printf("server disconnected for optionC\n");
            exit(0);
        }
        i = 0;
        while (optionC[i]) {
            if (optionC[i] == '\n') optionC[i] = '\0';
            i++;
        }
        printf("Got to D\n");
        bytes_read = read(server_socket, optionD, sizeof(optionD));
        if (bytes_read <= 0) {
            printf("server disconnected for optionD\n");
            exit(0);
        }
        i = 0;
        while (optionD[i]) {
            if (optionD[i] == '\n') optionD[i] = '\0';
            i++;
        }
        

        

        struct timeval start_time;
        gettimeofday(&start_time, NULL);

        

        

        int add_points = pointSystem(correct_answer_buffer, response_buffer, start_time, optionA, optionB, optionC, optionD);
        score += add_points;

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

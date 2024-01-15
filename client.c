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

    //printf("correct_answer_buffer: %s\n", correct_answer_buffer);

    // remove newline from playerAnswer
    int i = 0;
    while (playerAnswer[i]) {
        if (playerAnswer[i] == '\n' || playerAnswer[i] == '\r') {
            playerAnswer[i] = '\0';
        }
        i++;
    }

    while (correct_answer_buffer[i]) {
        if (correct_answer_buffer[i] == '\n' || correct_answer_buffer[i] == '\r') {
            correct_answer_buffer[i] = '\0';
        }
        i++;
    }

    int checkValid = 0;

    while (checkValid == 0) {
        if (strcmp(playerAnswer, "A") == 0) {
            strcpy(playerAnswer, optionA);
            checkValid = 1;
        }
        else if (strcmp(playerAnswer, "B") == 0) {
            strcpy(playerAnswer, optionB);
            checkValid = 1;
        }
        else if (strcmp(playerAnswer, "C") == 0) {
            strcpy(playerAnswer, optionC);
            checkValid = 1;
        }
        else if (strcmp(playerAnswer, "D") == 0) {
            strcpy(playerAnswer, optionD);
            checkValid = 1;
        }
        else {
            printf("Invalid answer: please answer with A, B, C, or D: ");
            fgets(playerAnswer, BUFFER_SIZE, stdin);
            i = 0;
            while (playerAnswer[i]) {
                if (playerAnswer[i] == '\n' || playerAnswer[i] == '\r') {
                    playerAnswer[i] = '\0';
                }
                if (playerAnswer[i] >= 'a' && playerAnswer[i] <= 'z') {
                    //printf("changing case\n");
                    playerAnswer[i] = playerAnswer[i] - 32;
                }
                i++;
            }
        }

    }

    

    //printf("playerAnswer: %s\n", playerAnswer);
    //printf("compare result: %d\n", strcmp(playerAnswer, correct_answer_buffer));

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
    char name[BUFFER_SIZE];
    struct questionAndOptions question_buffer[BUFFER_SIZE];
    char correct_answer_buffer[BUFFER_SIZE];
    char optionA[BUFFER_SIZE];
    char optionB[BUFFER_SIZE];
    char optionC[BUFFER_SIZE];
    char optionD[BUFFER_SIZE];
    int score = 0;

    int bytes_read;
    int current_question_number = 0;

    int max_questions = 0;

    printf("What is your name?:  ");
    fgets(name, BUFFER_SIZE, stdin);
    struct player player;
    strcpy(player.name, name);
    player.score = score;

    // wait for game to start, block until server sends a question
    bytes_read = read(server_socket, response_buffer, sizeof(response_buffer));
    if (bytes_read <= 0) {
        printf("server disconnected LOL\n");
        exit(0);
    }

    bytes_read = read(server_socket, &max_questions, sizeof(max_questions));
    if (bytes_read <= 0) {
        printf("server disconnected max\n");
        exit(0);
    }

    //printf("number of questions: %d\n", max_questions);


    printf("%s\n", response_buffer);
    int goNext = 0;
    int begin = 0;
    //printf("Here1!\n");
  // once server sends the first question, loop
    while (current_question_number < max_questions) {
        int i = 0;
        // read question
        bytes_read = read(server_socket, question_buffer, sizeof(question_buffer));
        if (bytes_read <= 0) {
            printf("server disconnected for question string\n");
            exit(0);
        }
        goNext = 0;
        write(server_socket, &goNext, sizeof(goNext)); // write to server to PAUSE to next question
        
        //printf("question from server: %sbruh\n", question_buffer->question);

        //printf("bytes read for question buffer %d\n", bytes_read);
        sprintf(response_buffer, "Question %d: %s\nA: %s\nB: %s\nC: %s\nD: %s\n", current_question_number+1, question_buffer->question, question_buffer->optionA, question_buffer->optionB, question_buffer->optionC, question_buffer->optionD);

        
        printf("%s", response_buffer); // print question to client
        fgets(response_buffer, sizeof(response_buffer), stdin); // read client response from command line

        while (response_buffer[i]) {
            // uppercase the response
            if (response_buffer[i] >= 'a' && response_buffer[i] <= 'z') {
                //printf("changing case\n");
                response_buffer[i] = response_buffer[i] - 32;
            }

            i++;
        }

        printf("Your answer: %s\n", response_buffer); // print client response to client

        struct timeval start_time;
        gettimeofday(&start_time, NULL);

        strcpy(correct_answer_buffer, question_buffer->correctAnswer);
        strcpy(optionA, question_buffer->optionA);
        strcpy(optionB, question_buffer->optionB);
        strcpy(optionC, question_buffer->optionC);
        strcpy(optionD, question_buffer->optionD);
        
        // calculate points and add to score
        int add_points = pointSystem(correct_answer_buffer, response_buffer, start_time, optionA, optionB, optionC, optionD);
        score += add_points;

        goNext = 1;
        write(server_socket, &goNext, sizeof(goNext)); // write to server to go to next question
        player.score = score;

        int receivedNext;
        bytes_read = read(server_socket, &receivedNext, sizeof(receivedNext));
        if (bytes_read <= 0) {
            printf("server disconnected for before send player\n");
            exit(0);
        }
        if (receivedNext == 100) {
            //printf("receivedNext: %d\n", receivedNext);
            write(server_socket, &player, sizeof(player)); // write player struct to server



        }

        current_question_number++;
    }



  // display final scores at the end
    printf("Your final score is: %d\n", score);
    printf("Thanks for playing!\n");

    // write the player struct to the server
    

    // bytes_read = write(server_socket, &player, sizeof(player));
    // if (bytes_read <= 0) {
    //     printf("server disconnected for final score\n");
    //     exit(0);
    // }
    // printf("sent player struct to server, with %d bytes read\n", bytes_read);

    // read the winner from the server
    char winner[BUFFER_SIZE];
    bytes_read = read(server_socket, winner, BUFFER_SIZE);
    if (bytes_read <= 0) {
        printf("server disconnected for winner\n");
        exit(0);
    }
    printf("%s\n", winner);

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

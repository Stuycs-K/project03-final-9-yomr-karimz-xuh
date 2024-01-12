#include "networking.h"

void broadcast_message(int* client_sockets, int num_clients, char message[BUFFER_SIZE], int sizeBuff) {
    for (int i = 0; i < num_clients; i++) {
        printf("sizeBuff: %d\n", sizeBuff);
        int bytes_sent = write(client_sockets[i], message, BUFFER_SIZE);
        if (bytes_sent < 0) {
            perror("write error\n");
        }

        //printf("client socket: %d\n", client_sockets[i]);
        //printf("\n");
    }
}

void send_question(int* client_sockets, int num_clients, struct questionAndOptions question, int sizeBuff) {
    for (int i = 0; i < num_clients; i++) {
        printf("sending question to client %d\n", i);
        printf("question: %s\n", question.question);
        printf("optionA: %s\n", question.optionA);
        printf("optionB: %s\n", question.optionB);
        printf("optionC: %s\n", question.optionC);
        printf("optionD: %s\n", question.optionD);
        printf("correctAnswer: %s\n", question.correctAnswer);

        write(client_sockets[i], &question, sizeBuff);
        //printf("client socket: %d\n", client_sockets[i]);
        //printf("\n");
    }
}



struct questionAndOptions* read_csv() {

    // num of lines
    FILE* question_bank = fopen("./question_bank.csv", "r");
    //printf("opened file\n");
    char buff[BUFFER_SIZE];
    //printf("created buff\n");
    int lineCount = -1;
    while (fgets(buff, BUFFER_SIZE, question_bank) != NULL) {
        if (strlen(buff) < 5) {
            continue;
        }
        lineCount++;
    }
    //printf("lineCount: %d\n", lineCount);
    fclose(question_bank);


    question_bank = fopen("./question_bank.csv", "r");


    fgets(buff, BUFFER_SIZE, question_bank);
    char* tempBuff = strdup(buff);
    if (tempBuff == NULL) {
        perror("strdup error tempBuff\n");
    }
    int columnCount = 0;
    char* substring;
    // num of columns
    while ((substring = strsep(&tempBuff, ",")) != NULL) {
        columnCount++;
    }
    //printf("columnCount: %d\n", columnCount);

    fclose(question_bank);
    int NumberOfElements = columnCount * lineCount;
    struct questionAndOptions* questions = malloc(NumberOfElements * sizeof(struct questionAndOptions));  
    if (questions == NULL) {
        perror("malloc error questions\n");
    }
    
    question_bank = fopen("./question_bank.csv", "r");
    fgets(buff, BUFFER_SIZE, question_bank);



    // filling entries
    int currentLine = 0;

    while (fgets(buff, BUFFER_SIZE, question_bank) != NULL) {

        // check if line is empty
        if (strlen(buff) < 5) {
            continue;
        }

        int currentColumn = 0;

        tempBuff = strdup(buff);
        if (tempBuff == NULL) {
            perror("strdup error tempBuff\n");
        }
        while ((substring = strsep(&tempBuff, ",")) != NULL && currentColumn < 6) {
            if (strlen(substring) <= 0) {
                continue;
            }
            //printf("substring: %s\n", substring);
            if (currentColumn == 0) {
                strcpy(questions[currentLine].question, substring);
            } else if (currentColumn == 1) {
                strcpy(questions[currentLine].optionA, substring);
            } else if (currentColumn == 2) {
                strcpy(questions[currentLine].optionB, substring);
            } else if (currentColumn == 3) {
                strcpy(questions[currentLine].optionC, substring);
            } else if (currentColumn == 4) {
                strcpy(questions[currentLine].optionD, substring);
            } else if (currentColumn == 5) {
                strcpy(questions[currentLine].correctAnswer, substring);
            }
            currentColumn++;
        }

        
        currentLine++;

    }


    fclose(question_bank);

    // print all questions with options in the format
    // question
    // A. optionA
    // B. optionB
    // C. optionC
    // D. optionD
    // correctAnswer

    // for (int i = 0; i < lineCount; i++) {
    //     printf("%s\n", questions[i].question);
    //     printf("A. %s\n", questions[i].optionA);
    //     printf("B. %s\n", questions[i].optionB);
    //     printf("C. %s\n", questions[i].optionC);
    //     printf("D. %s\n", questions[i].optionD);
    //     printf("%s\n", questions[i].correctAnswer);
    // }


    return questions;
}




int main(int argc, char *argv[] ) {

    struct questionAndOptions* questions = read_csv();


    int listen_socket = server_setup(); 

    int num_questions = 0;
    while (questions[num_questions].question[0] != '\0') {
        num_questions++;
    }


    int client_sockets[MAX_PLAYERS];
    int client_count = 0;


    fd_set read_fds;
    FD_ZERO(&read_fds);
    int questionIndex = 0;
    int start = 0; 
    while (questionIndex < num_questions) {
        
        printf("Out of the for loop\n");
        int goNext = 0;
        
        FD_SET(listen_socket, &read_fds);

        for (int i = 0; i < MAX_PLAYERS; i++) {
            FD_SET(client_sockets[i], &read_fds);
        }

        select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);

        // Check for new client connections
        if (FD_ISSET(listen_socket, &read_fds)) {
            printf("got here now\n");
            int new_client_socket = accept(listen_socket, NULL, NULL);
            if (new_client_socket != -1 && client_count < MAX_PLAYERS) {
                client_sockets[client_count++] = new_client_socket;
                printf("New client connected.\n");


                // send question to client
                char* question = questions[questionIndex].question;
                char* optionA = questions[questionIndex].optionA;
                char* optionB = questions[questionIndex].optionB;
                char* optionC = questions[questionIndex].optionC;
                char* optionD = questions[questionIndex].optionD;

                char message[BUFFER_SIZE];
                

                if (client_count == MAX_PLAYERS && start == 0) {
                    printf("Beginning in server...\n");
                    //sprintf(message, "Game starting...\n");
                    char str1[20] = "Game starting...\n";
                    strcpy(message, str1);
                    broadcast_message(client_sockets, client_count, message, strlen(message)+1);
                    send_question(client_sockets, client_count, questions[0], sizeof(questions[questionIndex]));
                    start = 1;
                    //printf("Sent all options\n");
                }

                if (client_count == MAX_PLAYERS && start == 1 && goNext == 1) {
                    questionIndex++;
                    printf("Back again!\n");
                    //printf("Sending question to client\n");
                    send_question(client_sockets, client_count, questions[questionIndex], sizeof(questions[questionIndex]));

                    printf("Sent all options\n");
                    goNext = 0;
                }
            

                
                

            }
        }

        // Every client socket sends an int, goNext. If goNext == 1 in EVERY CLIENT, then send the next question to all clients
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (FD_ISSET(client_sockets[i], &read_fds)) {
                printf("running read\n");
                int clientGoNext;
                read(client_sockets[i], &clientGoNext, sizeof(goNext));
                if (clientGoNext == 1) {
                    printf("Client %d is ready for next question\n", i);
                    goNext = 1;
                }
                else {
                    // break out of loop if one client is not ready
                    goNext = 0;
                    break;
                }
            }
        }

        
    }

    // Close client sockets and perform cleanup if needed
    for (int i = 0; i < client_count; i++) {
        close(client_sockets[i]);
    }

    return 0;
}



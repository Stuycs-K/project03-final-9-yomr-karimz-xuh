#include "networking.h"

void broadcast_message(int* client_sockets, int num_clients, char* message, int sizeBuff) {
    for (int i = 0; i < num_clients; i++) {
        write(client_sockets[i], message, sizeBuff);
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
                questions[currentLine].question = strdup(substring);
            }
            else if (currentColumn == 1) {
                questions[currentLine].optionA = strdup(substring);
            }
            else if (currentColumn == 2) {
                questions[currentLine].optionB = strdup(substring);
            }
            else if (currentColumn == 3) {
                questions[currentLine].optionC = strdup(substring);
            }
            else if (currentColumn == 4) {
                questions[currentLine].optionD = strdup(substring);
            }
            else if (currentColumn == 5) {
                questions[currentLine].correctAnswer = strdup(substring);
            }
            else {
                perror("error in filling entries\n");
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


    int client_sockets[MAX_PLAYERS];
    int client_count = 0;


    fd_set read_fds;
    FD_ZERO(&read_fds);

    while (1) {
        FD_SET(listen_socket, &read_fds);

        for (int i = 0; i < MAX_PLAYERS; i++) {
            FD_SET(client_sockets[i], &read_fds);
        }

        select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);

        // Check for new client connections
        if (FD_ISSET(listen_socket, &read_fds)) {
            int new_client_socket = accept(listen_socket, NULL, NULL);
            if (new_client_socket != -1 && client_count < MAX_PLAYERS) {
                client_sockets[client_count++] = new_client_socket;
                printf("New client connected.\n");


                // send question to client
                char* question = questions[0].question;
                char* optionA = questions[0].optionA;
                char* optionB = questions[0].optionB;
                char* optionC = questions[0].optionC;
                char* optionD = questions[0].optionD;

                char message[BUFFER_SIZE];
                

                if (client_count == MAX_PLAYERS) {
                    printf("Beginning...\n");
                    //sprintf(message, "Game starting...\n");
                    char str1[20] = "Game starting...\n";
                    strcpy(message, str1);
                    broadcast_message(client_sockets, client_count, message, strlen(message)+1);
                    sprintf(message, "%s\nA. %s\nB. %s\nC. %s\nD. %s\n", question, optionA, optionB, optionC, optionD);
                    broadcast_message(client_sockets, client_count, message, strlen(message)+1);

                    //printf("Sent question to all clients\n");
                    char correct_answer[BUFFER_SIZE];
                    strcpy(correct_answer, questions[0].correctAnswer);
                    int i = 0;
                    while (correct_answer[i]) {
                        if (correct_answer[i] == '\n') correct_answer[i] = '\0';
                        i++;
                    }
                    
                    broadcast_message(client_sockets, client_count, correct_answer, strlen(correct_answer)+1);
                    //printf("Sent correct answer to all clients\n");
                    broadcast_message(client_sockets, client_count, optionA, strlen(optionA)+1);
                    broadcast_message(client_sockets, client_count, optionB, strlen(optionB)+1);
                    broadcast_message(client_sockets, client_count, optionC, strlen(optionC)+1);
                    broadcast_message(client_sockets, client_count, optionD, strlen(optionD)+1);
                }
            

                
                

            }
        }

        // Check for data from existing clients
        for (int i = 0; i < client_count; i++) {
            if (FD_ISSET(client_sockets[i], &read_fds)) {
                int client_new_score;
                read(client_sockets[i], &client_new_score, sizeof(int));
                // Process the score if needed
            }
        }
    }

    // Close client sockets and perform cleanup if needed
    for (int i = 0; i < client_count; i++) {
        close(client_sockets[i]);
    }

    return 0;
}



#include "networking.h"

void broadcast_message(int* client_sockets, int num_clients, char* message) {
    for (int i = 0; i < num_clients; i++) {
        write(client_sockets[i], message, strlen(message) + 1);
    }
}

// Function to send a question to all connected clients
void send_question(int* client_sockets, int num_clients, char* question) {
    for (int i = 0; i < num_clients; i++) {
        write(client_sockets[i], question, strlen(question) + 1);
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
        while ((substring = strsep(&tempBuff, ",")) != NULL) {
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

    for (int i = 0; i < lineCount; i++) {
        printf("%s\n", questions[i].question);
        printf("A. %s\n", questions[i].optionA);
        printf("B. %s\n", questions[i].optionB);
        printf("C. %s\n", questions[i].optionC);
        printf("D. %s\n", questions[i].optionD);
        printf("%s\n", questions[i].correctAnswer);
    }


    return questions;
}

void subserver_logic(int * client_sockets, int client_count){
    struct questionAndOptions* questions = read_csv();
    int client_new_score;

    while (1) {
        fd_set descriptors;
        FD_ZERO(&descriptors);

        int max_descriptor = -1;
        for (int i = 0; i < client_count; i++) {
            int client_socket = client_sockets[i];
            FD_SET(client_socket, &descriptors);
            if (client_socket > max_descriptor) {
                max_descriptor = client_socket;
            }
        }

        select(max_descriptor + 1, &descriptors, NULL, NULL, NULL);
        
        for (int i = 0; i < client_count; i++) {
            if (FD_ISSET(client_sockets[i], &descriptors)) {
                // score processing is handled on client side
                read(client_sockets[i], client_new_score, sizeof(int));
            }
        }
    }
}


int main(int argc, char *argv[] ) {

  // int client_socket = server_setup(); 


  // while (1) {
  //   int subserver_socket = server_tcp_handshake(client_socket);

  //   int process = fork();
  //   if (process < 0) {
  //     perror("Forking error\n");
  //     exit(1);
  //   }


  //   else if (process == 0) {
      
 
      
  //     printf("[server] connected to client\n");
  //     subserver_logic(subserver_socket);
  //     exit(0);

  //   }

  //   else {

  //     close(subserver_socket);
  //   }
    
    

  // }
  
  return 0;
}

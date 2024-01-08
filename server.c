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
    char buff[BUFFER_SIZE];

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
            // handle data from each client in HEREREREREREREREEEEEEE
            
        }
    }
}


int main(int argc, char *argv[] ) {

  struct questionAndOptions* questions = read_csv();


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

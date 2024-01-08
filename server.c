#include "networking.h"





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



void subserver_logic(int client_socket, struct questionAndOptions* questions){
    char buffer[BUFFER_SIZE];
    int bytes_read;

    sprintf(buffer, "Welcome to our Quiz Show!\nToday's topic will be general trivia.\nBeginning...\n");
    write(client_socket, buffer, bytes_read);

    int i = 0;
    while (questions[i].question != NULL) {
        sprintf(buffer, "%s\nA. %s\nB. %s\nC. %s\nD. %s\n", questions[i].question, questions[i].optionA, questions[i].optionB, questions[i].optionC, questions[i].optionD);
        if (write(client_socket, buffer, bytes_read) < 0) perror("Couldn't send question to server\n");
        if (read(client_socket, buffer, bytes_read) < 0) perror("Couldn't write answer from client\n");
    }

    bytes_read = read(client_socket, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
    printf("client disconnected\n");
    exit(0);
    }


    
    printf("Sent back to client: %s\n", buffer);
}



int main(int argc, char *argv[] ) {

  struct questionAndOptions* questions = read_csv();


  int client_socket = server_setup(); 


  while (1) {
    int subserver_socket = server_tcp_handshake(client_socket);

    int process = fork();
    if (process < 0) {
      perror("Forking error\n");
      exit(1);
    }


    else if (process == 0) {
      

      
      printf("[server] connected to client\n");
      subserver_logic(subserver_socket, questions);
      exit(0);

    }

    else {

      close(subserver_socket);
    }
    
    

  }
  
  return 0;
}

#include "networking.h"





struct questionAndOptions* read_csv() {

    // num of lines
    FILE* question_bank = fopen("./question_bank", "r");
    char buff[BUFFER_SIZE];
    int lineCount = -1;
    while (fgets(buff, BUFFER_SIZE, question_bank)) {
        lineCount++;
    }
    fclose(question_bank);


    question_bank = fopen("./question_bank", "r");


    fgets(buff, BUFFER_SIZE, question_bank);
    char* substring;
    int columnCount = 0;

    char* tempBuff = malloc(BUFFER_SIZE * sizeof(char));
    if (tempBuff == NULL) {
        perror("malloc error tempBuff\n");
    }
    strcpy(tempBuff, buff);
    // num of columns
    while ((substring = strsep(&tempBuff, ",")) != NULL) {
        columnCount++;
    }
    

    fclose(question_bank);
    int NumberOfElements = columnCount * lineCount;
    struct questionAndOptions* questions = malloc(lineCount * sizeof(struct questionAndOptions));  
    if (questions == NULL) {
        perror("malloc error questions\n");
    }
    
    question_bank = fopen("./question_bank", "r");
    fgets(buff, BUFFER_SIZE, question_bank);



    // filling entries
    int currentLine = 0;

    while (fgets(buff, BUFFER_SIZE, question_bank) != NULL) {

        sscanf(buff, "%[^','] %[^','] %[^','] %[^','] %[^','] %[^',']", questions[currentLine].question, questions[currentLine].optionA, questions[currentLine].optionB, questions[currentLine].optionC, questions[currentLine].optionD, questions[currentLine].correctAnswer);
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



void subserver_logic(int client_socket){
  char buffer[BUFFER_SIZE];
  int bytes_read;


  bytes_read = read(client_socket, buffer, sizeof(buffer));
  if (bytes_read <= 0) {
    printf("client disconnected\n");
    exit(0);
  }


  write(client_socket, buffer, bytes_read);
  printf("Sent back to client: %s\n", buffer);
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

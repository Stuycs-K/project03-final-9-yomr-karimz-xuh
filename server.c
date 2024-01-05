#include "networking.h"

void read_csv() {

    // num of lines
    FILE* questions = fopen("./question_bank", "r");
    char buff[BUFFER_SIZE];
    int lineCount = -1;
    while (fgets(buff, BUFFER_SIZE, questions)) {
        lineCount++;
    }
    fclose(pop);


    questions = fopen("./question_bank", "r");


    fgets(buff, BUFFER_SIZE, pop);
    char* substring;
    int columnCount = 0;

    char* tempBuff = malloc(BUFFER_SIZE * sizeof(char));
    if (tempBuff == NULL) {
        err();
    }
    strcpy(tempBuff, buff);
    // num of columns
    while ((substring = strsep(&tempBuff, ",")) != NULL) {
        columnCount++;
    }
    

    fclose(pop);
    int NumberOfElements = columnCount * lineCount;
    struct questionAndOptions* questions = malloc(lineCount, sizeof(struct questionAndOptions));  
    if (entries == NULL) {
        err();
    }
    
    // filling in borough array
    char boroughs[columnCount+1][15];
    int boroIndex = 0;

    pop = fopen("./nyc_pop.csv", "r");
    fgets(buff, BUFFER_SIZE, pop);
    tempBuff = malloc(BUFFER_SIZE * sizeof(char));
    if (tempBuff == NULL) {
        err();
    }
    strcpy(tempBuff, buff);


    while ((substring = strsep(&tempBuff, ",")) != NULL) {
        if (strcmp(substring, "Year") != 0) {
            size_t endChar = strlen(substring) - 1;
            size_t carriageChar = strlen(substring) - 2;
            if (substring[carriageChar] == '\r') substring[carriageChar] = '\0';
            if (substring[endChar] == '\n') substring[endChar] = '\0';
                

            strcpy(boroughs[boroIndex], substring);
            boroIndex++;
        }
    }
    
    // filling entries
    int currentColumn = 0;
    int currentLine = 0;
    int currentYear = 0;
    int currentPopulation = 0;

    while (fgets(buff, BUFFER_SIZE, pop) != NULL) {
        currentColumn = 0;
        tempBuff = malloc(BUFFER_SIZE * sizeof(char));
        if (tempBuff == NULL) {
            err();
        }
        strcpy(tempBuff, buff);
        while ((substring = strsep(&tempBuff, ",")) != NULL && currentColumn < columnCount+1) { // going through columns
            if (currentColumn == 0) {
                sscanf(substring, "%d", &currentYear);
                currentColumn++;
                continue;
            }
            
            int index = currentLine * columnCount + currentColumn - 1;
            sscanf(substring, "%d", &currentPopulation);
            entries[index].year = currentYear;
            entries[index].population = currentPopulation;
            strcpy(entries[index].boro, boroughs[currentColumn - 1]);

            currentColumn++;


        }
        currentLine++;
    }


    fclose(pop);
    // writing to dat file
    printf("File written with %zd bytes\n", write(wfile, entries, NumberOfElements * sizeof(struct pop_entry)));
    close(wfile);
    
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
      subserver_logic(subserver_socket);
      exit(0);

    }

    else {

      close(subserver_socket);
    }
    
    

  }
  
  return 0;
}

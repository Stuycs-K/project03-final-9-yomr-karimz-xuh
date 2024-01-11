# Dev Log:

## Richard Yom

### 2024-01-03 - 7 pm
updated README.md instructions and project descriptors

### 2024-01-04 - 2:30 pm
updated / inclusion of DEVLOG.md 
working on making trivia questions with multiple choice answers

### 2024-01-05 - 2:40 pm
worked on making question bank csv files for all the trivia
categories for our kahoot/quiz game

### 2024-01-08 - 2:30 pm
worked on point system for game logic 
including handling storing point values for players

### 2024-01-09 - 2:50 pm
worked on game end system with displaying scores, sending points to server

### 2024-01-10 - 2:40 pm 
bug fixing for client.c broadcast message and beyond (pair programming)

### 2024-01-10 - 2:40 pm 
bug fixing for client.c for broadcast message and 
now making so we send structs over sockets (pair programming)

## Haokun Xu

### 2024-01-04 - 2:30 pm
updated PROPOSAL.md

### 2024-01-05 - 2:40 pm
implented client logic for reading and answering questions from the server

### 2024-01-08 - 2:30 pm
working on client connections to server from separate hosts using select

### 2024-01-09 - 2:50 pm
refactored score system and implemented into client 

### 2024-01-10 - 2:40 pm 
bug fixing for client.c broadcast message and beyond (pair programming)

### 2024-01-10 - 2:40 pm 
bug fixing for client.c for broadcast message and 
now making so we send structs over sockets (pair programming)

## Zidane Karim

### 2024-01-04 - 5:00 pm
NOTE: absent from class today

copied lab16 server, networking, and client files in order to adjust for kahooot

added time library to header

added question bank csv file

### 2024-01-05 - 2:40 pm
worked on CSV parsing; adjusting to fit size of questions
created question struct

### 2024-01-06 - 6:10 pm
finished CSV parsing; had to switch to strsep over sscanf because sscanf wouldn't work with questions with single letter answers
segmentation faults fixed by using strsep, couldn't figure out what caused seg. faults with sscanf

### 2024-01-08 - 2:30 pm
worked on game logic for the quiz including point system + text
logic + error handling + maximum client handling 

### 2024-01-09 - 2:50 pm 
NOTE: absent from class 

### 2024-01-09 - 10:50 pm 
worked on moving subserver code to main; current error is that text will not send to client, client immediately crashes

### 2024-01-10 - 2:40 pm 
bug fixing for client.c broadcast message and beyond (pair programming)

### 2024-01-10 - 2:40 pm 
bug fixing for client.c for broadcast message and 
now making so we send structs over sockets (pair programming)
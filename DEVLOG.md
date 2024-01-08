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

## Haokun Xu

### 2024-01-04 - 2:30 pm
updated PROPOSAL.md

### 2024-01-05 - 2:40 pm
implented client logic for reading and answering questions from the server

### 2024-01-08 - 2:30 pm
working on client connections to server from separate hosts

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
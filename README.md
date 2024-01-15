[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/SQs7pKlr)
# K - QUIZ

### 9-yomr-karimz-xuh

Richard Yom
Zidane Karim
Haokun Xu
       
### Project Description:

Kahoot/Quizlet esk style project that asks user(s) to input
the answer to a pre-determined trivia style question. Points are tallied 
based on speed and accuracy. Person(s) with the most points at the end
wins the game!
  
### Instructions:

How does the user install/compile/run the program.

Compilation options:

* To compile everything: make
* To compile just server: make server
* To compile just client: make client

After compiling:

* To run server: ./server
* To run client: ./client

How does the user interact with this program?

For server users:

1. Input the question set when prompted. If invalid string is inputted, it will default to the "general" set.
2. Input the amount of questions you wish to have in the game (integer 1-10)
3. The game will automatically start once the MAX_PLAYERS is reached!

For client users:

1. Input your name when prompted.
2. For each question, input the correct answer using the keyboard (A, B, C, D) into the prompt based on the question provided. Continuously answer questions until game is finished!

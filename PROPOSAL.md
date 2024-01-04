# Final Project Proposal

## Group Members:

Richard Yom
Zidane Karim
Haokun Xu
       
# Intentions:

Kahoot/Quizlet esk style project that asks user(s) to input the answer to a pre-determined trivia style question. Points are tallied based on speed and accuracy. Person(s) with the most points at the end wins the game!
    
# Intended usage:

User input will be collected through terminal keyboard input.
For clients, this will include answering questions by entering the number choice.
For the server, this will include starting the game by entering a keyword.
  
# Technical Details:

Using handshakes to connect, we will be using one server, which then distributes questions to each client. This will include forking, and looping in the server/client to actively check for responses. We will implement the questions and answers using structs and arrays. In order to store scores and questions, we will use writing and reading to local files so that we can retrieve the data even from a future session. Using signals and/or semaphores, we can implement exiting and kicking clients if they get too many questions wrong.
# Intended pacing:

1/8 - Networking complete
1/9 - Question/answer system complete
1/11 - Hard memory/retrieving past scores complete
1/12 - Signals complete and polishing

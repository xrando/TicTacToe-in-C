# TicTacToe-in-C
Tic Tac Toe game with GUI. Player vs Player mode as well as Player vs Computer mode with computer being an AI utilizing the minimax algorithm. Written in C using GTK lib 4.0.
Written during my undergraduate computing science course at the Singapore Institute of Technology, joint degree with the University Of Glasgow. This game was written for my programming methodology project assignment during my first year of study.
##
Started: October 1st, 2022  
Finished: November 23, 2022
##
This was my first foray into GTK and developing an application in C. I learnt GTK from the ground up and i've received my knowledge in C from my 1st trimester of studies. Due to the lack of documentation for the GTK library in C, researching and testing our various sample tutorial projects out in the market took me some time to grasp the overall functionality of GTK and this took me some time. After figuring out how to implement grids and buttons to represent the tictactoe board, I took some more time to figure out how to actually integrate this user interface with the backend logic of the game which consists of the normal player vs player game mode and then the player vs computer mode which is built upon utilizing the minimax algorithm.
## User Interface:
![image](https://user-images.githubusercontent.com/39940264/206885754-76f71acb-1124-479d-840a-f00621f43c33.png)  
-Restart button that resets the instance of the game and clears board state.  
-Quit button that kills the exe program and essentially simulates closing the app.  
-Game label to display current game mode.  
-3 game mode selection buttons: Player vs Player mode, Player vs Computer mode, Player vs Dumb Computer mode.  
-Game label to display current player turn.  
![image](https://user-images.githubusercontent.com/39940264/206885911-e497b341-fc45-4579-a52d-2ab7ebce2fbd.png)  
-Game space changes to move selected depending on which player's turn (x -> player 1, o -> player 2)  
![image](https://user-images.githubusercontent.com/39940264/206885936-023a30dd-4c65-409c-a8e0-20a851bce17d.png)  
-Pop-up notification in event of player 1 winning / player 2 winning / game ends in a draw  
## Game Modes
### Player vs Player mode:

### Player vs Computer mode:

### Player vs Dumb Computer Mode

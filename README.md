# TicTacToe-in-C
Tic Tac Toe game with GUI. Player vs Player mode as well as Player vs Computer mode with computer being an AI utilizing the minimax algorithm. Written in C using GTK lib 4.0.
Written during my undergraduate computing science course at the Singapore Institute of Technology, joint degree with the University Of Glasgow. This game was written for my programming methodology project assignment during my first year of study.
##
Started: October 1st, 2022  
Finished: November 23, 2022
##
This was my first foray into GTK and developing an application in C. I learnt GTK from the ground up and i've received my knowledge in C from my 1st trimester of studies. Due to the lack of documentation for the GTK library in C, researching and testing our various sample tutorial projects out in the market took me some time to grasp the overall functionality of GTK and this took me some time. After figuring out how to implement grids and buttons to represent the tictactoe board, I took some more time to figure out how to actually integrate this user interface with the backend logic of the game which consists of the normal player vs player game mode and then the player vs computer mode which is built upon utilizing the minimax algorithm.
## Prerequisites / Installations
### Building / Compiling Instructions
Steps to install gcc compiler via MSYS2:
```
URL: https://packages.msys2.org/package/mingw-w64-x86_64-gcc
```
Steps to install GTK 4.0 library extension via MSYS2:
```
URL: https://stdin.top/posts/gtk4-on-windows/
```
Steps to Compile program with gcc compiler (Note: run this command in vscode / MSYS2 in the same directory containing the .c program file of this program):
```
gcc -o tictactoe tictactoe.c
```
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
2 Players can play a game of TicTacToe, player 1 being X and player 2 being O. When there is a end of game state (player 1 win / player 2 win / draw) then a popup window will notify players of game outcome.
### Player vs Computer mode:
Player will be playing a game of TicTacToe with an AI computer that utilizes the Minimax algorithm. The Minimax algorithm is a decision rule used in artificial intelligence, decision theory, game theory etc for minimizing the possible loss for a worst case scenario. In this game, the Minimax algorithm is being used to compute and find the best move the computer can possibly make to have the highest probability of winning.
### Player vs Dumb Computer Mode
Player will be playing a game of TicTacToe with a dumb AI computer that utilizes a modified version of the Minimax algorithm. This is to allow the player to have a chance to win since the minimax AI will either win / draw everytime and this will get annoying for the player. This is implemented by introducing the idea of a pity counter which is similar to the pity counter of how gacha games out in the market work. When the player loses 2 times, the program will kick in to generate a random move for the computer instead of calculating the best move for the computer using the minimax algorithm.

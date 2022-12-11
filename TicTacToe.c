#include <stdio.h>
#include <gtk/gtk.h>
#include <stdbool.h>

//function prototypes
int bestMove(char[]);
int bestMove_dumb(char[]);
int minimax(char [], bool, int, int, int);
int Dumb_minimax(char[], bool, int);
char checkWin();
int getRandomMove();
int writeToFile(char);


//define variables

//============ GUI Widgets ============//
//can create multiple button pointers for clearer representation
GtkWidget *window, *grid, *label, *restart, *gametype, *quit;

//game mode buttons 
static GtkWidget *twoPlay, *onePlay, *dumb;

//dynamic display of message 
static GtkWidget *gameModeDisplay,*winMessage, *whoTurn;
//====================================//

//initialize variables 
int pityCounter = 0;
int turncount = 0; 
int currentMode = 0; // 0 = 2Players mode, 1 = 1Player mode, 2 = DumbAI mode

//game modes 
const char mode[4][25] = {"2 Players Mode", "1 Player Mode", "Dumb AI Mode"};

//create ttt board for logic and gui 
struct buttonTTT {
    GtkWidget* grid[9];
};
//initialize global array of 9 space to be used to store board state
char board[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};



//============ GUI Functions ============//

//disable TTT grid
void tttDisabled(struct buttonTTT *disable)
{
  for (int i = 0; i < 9; ++i){
        gtk_widget_set_sensitive(disable->grid[i], FALSE);
    }
}

//when TTT grid is clicked 
void tttClicked(GtkWidget *widget,gpointer data)
{
  const char *text = gtk_button_get_label(GTK_BUTTON(widget));
  const char move = turncount++ % 2 == 0 ? 'X' : 'O'; 
  char label[2] = {move, '\0'};
  //update board array
  board[*text] = move;

  //setting 'O' or 'X' in the UI
  gtk_button_set_label(GTK_BUTTON(widget), label);
  gtk_widget_set_child_visible(gtk_button_get_child(GTK_BUTTON(widget)), true);

  //disable button to prevent second clicks
  gtk_widget_set_sensitive(widget, FALSE);

  //check win condition
  if (turncount >= 5)
  {
    //check win function returns X,O,N,T (X means X won,O means O won, T means draw, N means game havent end)
    char winner = checkWin(board);
    //write to file for confusion matrix (currentMode == 2)
    if(currentMode == 2)
    {
        writeToFile(winner);
    }
    if(winner != 'N' && winner != 'T')
    {
      //disable ttt 
      tttDisabled((struct buttonGrid*)data);

      //Creating winner pop up
      GtkWidget *window = gtk_window_new();
      gtk_window_set_default_size (GTK_WINDOW (window), 350, 200);
      gtk_window_set_title(GTK_WINDOW(window), "Winner");

      if (winner == 'X')
      {
        winMessage = gtk_label_new ("Player X has Won!");  
        gtk_window_set_child (GTK_WINDOW (window), winMessage);
        gtk_window_present(GTK_WINDOW(window)); 
        return;
      }
      else
      {
        winMessage = gtk_label_new ("Player O has Won!");
        gtk_window_set_child (GTK_WINDOW (window), winMessage);
        gtk_window_present(GTK_WINDOW(window));  
      } 
    }
    else if (winner == 'T')
    {
      //Creating draw pop up
        GtkWidget *window = gtk_window_new();
        gtk_window_set_default_size (GTK_WINDOW (window), 350, 200);
        gtk_window_set_title(GTK_WINDOW(window), "Draw");
        winMessage = gtk_label_new ("It is a Draw!");
        gtk_window_set_child (GTK_WINDOW (window), winMessage);
        gtk_window_present(GTK_WINDOW(window));
    }
  } 
  int bestMoveIndex;
  //mode: player vs AI
  if(currentMode == 1 && move != 'O' && turncount < 9)
  {
    struct buttonTTT *gridP = data;
    //bestMoveIndex stores best move (0 to 8)
    bestMoveIndex = bestMove(board); //call best move function to get optimal move (best move returns index of array (0 to 8))
    //update ui 
    tttClicked(gridP->grid[bestMoveIndex], gridP);
  } 
  //mode: player vs dumbAI
  else if (currentMode == 2 && move != 'O' && turncount < 9)
  {
    struct buttonTTT *gridP = data;
    //call dumb best move
    bestMoveIndex = bestMove_dumb(board);
    //update ui
    tttClicked(gridP->grid[bestMoveIndex], gridP);
  }
}

//resets current data and clears tttboard
void resetGame(GtkWidget *widget,gpointer data)
{
  // Reset the board
  turncount = 0;
  struct buttonTTT *bGrid = data;
  //reset turn label
  gtk_label_set_text(whoTurn, "Player 1's Turn");
  for (char i=0; i < 9; ++i){
      int num = i + 1;
      board[i] = num + '0'; //changing number to char 
      // Enable the button
      gtk_widget_set_sensitive(bGrid->grid[i], TRUE);
      // Reset the labels
      char label[2] = {i, '\0'};
      gtk_button_set_label(GTK_BUTTON(bGrid->grid[i]), label);
      //  Hide button labels
      gtk_widget_set_child_visible(gtk_button_get_child(GTK_BUTTON(bGrid->grid[i])), FALSE);
  }

}

//Callback to change game mode 
static void twoP(GtkWidget *widget, gpointer data){
    gtk_widget_set_sensitive(twoPlay, FALSE); // Set current mode button to False
    gtk_widget_set_sensitive(onePlay, TRUE);
    gtk_widget_set_sensitive(dumb, TRUE);
    currentMode=0; // Update Current Mode value
    gtk_label_set_text(gameModeDisplay, "Current game mode:       2 PLAYERS MODE");
}
static void oneP(GtkWidget *widget, gpointer data){
    gtk_widget_set_sensitive(twoPlay, TRUE);
    gtk_widget_set_sensitive(onePlay, FALSE); // Set current mode button to False
    gtk_widget_set_sensitive(dumb, TRUE);
    currentMode=1; // Update Current Mode value
    gtk_label_set_text(gameModeDisplay, "Current game mode:       1 PLAYER MODE");
}
static void dumbAI(GtkWidget *widget, gpointer data){
    gtk_widget_set_sensitive(twoPlay, TRUE);
    gtk_widget_set_sensitive(onePlay, TRUE);
    gtk_widget_set_sensitive(dumb, FALSE); // Set current mode button to False
    currentMode=2; // Update Current Mode value
    //reset pityCounter
    pityCounter =0;
    gtk_label_set_text(gameModeDisplay, "Current game mode:       DUMB AI MODE");
}

static void wTurn(GtkWidget *widget, gpointer data){
  //update which player's turn to move 
  if (turncount % 2 == 0)
  {
    gtk_label_set_text(whoTurn, "Player 1's Turn");
  }
  else
  {
    gtk_label_set_text(whoTurn, "Player 2's Turn");
  }
}

//activate gui
static void activate (GtkApplication *app,gpointer user_data)
{
    //Initializing the tic tac toe 
    static struct buttonTTT bGrid;
    for (char i=0; i < 9; ++i){
        char label[2] = {i, '\0'};
        bGrid.grid[i] = gtk_button_new_with_label(label);
    }

    for (char i=0; i < 9; ++i){
        gtk_widget_set_size_request(bGrid.grid[i],200,200);
        g_signal_connect(bGrid.grid[i], "clicked", G_CALLBACK(tttClicked), &bGrid); //when button is clicked
        gtk_widget_set_child_visible(gtk_button_get_child(GTK_BUTTON(bGrid.grid[i])), FALSE); //hides label
        g_signal_connect(bGrid.grid[i], "clicked", G_CALLBACK(wTurn), NULL); //when button is clicked
    }

    //setup grid
    grid = gtk_grid_new();
    window = gtk_application_window_new (app);
    // gtk_grid_set_row_homogeneous (GTK_GRID (grid), TRUE);
    // gtk_grid_set_column_homogeneous (GTK_GRID (grid), TRUE);

    //build grid, set window title & size
    gtk_window_set_child(GTK_WINDOW (window), grid);
    gtk_window_set_title (GTK_WINDOW (window), "TicTacToe in C");
    gtk_window_set_default_size (GTK_WINDOW (window), 1000, 1000);

    //restart button 
    restart = gtk_button_new_with_label ("Restart");
    g_signal_connect (restart, "clicked", G_CALLBACK (resetGame), &bGrid);
    gtk_widget_set_size_request(restart,200,50);
    gtk_grid_attach(GTK_GRID(grid),restart,0,0,1,1);

    //quit button
    quit = gtk_button_new_with_label ("Quit");
    gtk_widget_set_size_request(quit,200,50);
    gtk_grid_attach(GTK_GRID(grid),quit,4,0,1,1);
    g_signal_connect_swapped(quit, "clicked", G_CALLBACK(gtk_window_destroy), window);
    
    //Display current game mode
    gameModeDisplay = gtk_label_new("Current game mode:       2 PLAYERS MODE");
    gtk_widget_set_size_request(gameModeDisplay,100,50);
    gtk_grid_attach(GTK_GRID(grid),gameModeDisplay,1,1,3,1);

    //Select game mode row  
    //2Players Button
    twoPlay = gtk_button_new_with_label(mode[0]);
    g_signal_connect(twoPlay, "clicked", G_CALLBACK(twoP), NULL);
    g_signal_connect(twoPlay, "clicked", G_CALLBACK(resetGame),&bGrid);
    gtk_grid_attach(GTK_GRID(grid), twoPlay, 1,2,1,1);
    gtk_widget_set_sensitive(twoPlay, FALSE); //2 Players mode at default
    //1Player Button
    onePlay = gtk_button_new_with_label(mode[1]);
    g_signal_connect(onePlay, "clicked", G_CALLBACK(oneP), NULL);
    g_signal_connect(onePlay, "clicked", G_CALLBACK(resetGame),&bGrid);
    gtk_grid_attach(GTK_GRID(grid), onePlay, 2,2,1,1);
    //DumbAI Button
    dumb = gtk_button_new_with_label(mode[2]);
    g_signal_connect(dumb, "clicked", G_CALLBACK(dumbAI), NULL);
    g_signal_connect(dumb, "clicked", G_CALLBACK(resetGame),&bGrid);
    gtk_grid_attach(GTK_GRID(grid), dumb, 3,2,1,1);

    //Area to display whos turn (player no)
    whoTurn = gtk_label_new("Player 1's Turn");
    gtk_widget_set_size_request(whoTurn,200,50);
    gtk_grid_attach(GTK_GRID(grid),whoTurn,2,3,1,1);

    //Setting up the Tic Tac Toe grid 
    gtk_grid_attach(GTK_GRID(grid), bGrid.grid[0], 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bGrid.grid[1], 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bGrid.grid[2], 3, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bGrid.grid[3], 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bGrid.grid[4], 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bGrid.grid[5], 3, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bGrid.grid[6], 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bGrid.grid[7], 2, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bGrid.grid[8], 3, 6, 1, 1);

    //display window
    gtk_window_present (GTK_WINDOW (window));
}

int main (int argc,char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}


// functions returns 3 outputs only, N when no one won yet, X/O when there is a win, T when its a tie
char checkWin()
{
    // set winner to N by default, meaning no one won yet
    char winner = 'N';
    // check every row for win, if found a win then return current mark won (X or O)
    if (board[0] == board[1] && board[1] == board[2] && board[0] != '-')
    {
        return board[0];
    }
    else if (board[3] == board[4] && board[4] == board[5] && board[3] != '-')
    {
        return board[3];
    }
    else if (board[6] == board[7] && board[7] == board[8] && board[6] != '-')
    {
        return board[6];
    }
    // check every column for win, if found a win then return current mark won (X or O)
    else if (board[0] == board[3] && board[3] == board[6] && board[0] != '-')
    {
        return board[0];
    }
    else if (board[1] == board[4] && board[4] == board[7] && board[1] != '-')
    {
        return board[1];
    }
    else if (board[2] == board[5] && board[5] == board[8] && board[2] != '-')
    {
        return board[2];
    }
    // check every diagonal for win, if found a win then return current mark won (X or O)
    else if (board[0] == board[4] && board[4] == board[8] && board[0] != '-')
    {
        return board[0];
    }
    else if (board[2] == board[4] && board[4] == board[6] && board[2] != '-')
    {
        return board[2];
    }
    int count = 0;
    // loop through all spaces in board to check how many are occupied
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == 'X' || board[i] == 'O')
        {
            count++;
        }
    }
    // if board full and no one won = draw, return T
    if (count == 9)
    {
        return 'T';
    }
    // return N if no one won yet
    return winner;
}

//compute best move index for computer to take
int bestMove(char board[])
{
    //initialize bestScore with -2147483647, bestMoveIndex with -1, alpha with -2147483647, beta with 2147483647
    int bestScore = INT_MIN,bestMoveIndex = -1, alpha = INT_MIN, beta = INT_MAX;
    //loop 9 times and test move on board space if it is empty to compute optimal move for computer
    for(int i = 0; i < 9; i++)
    {
        //check if current move space is available
        if(board[i] != 'X' && board[i] != 'O')
        {
            //test computer move on current empty space
            board[i] = 'O';
            //simulate possible moves the computer and player can make and compute score achieved
            //Set flag to false to simulate player move, depth to 1
            int score = minimax(board, false, 1,alpha,beta);
            //reset current move space
            board[i] = '-';
            //if score computed is higher than bestScore
            if(score > bestScore)
            {
                //update bestMoveIndex for computer to make to (i)
                bestMoveIndex = i;
                //update bestScore to score computed
                bestScore = score;
            }
        }
    }
    //return bestMoveIndex
    return bestMoveIndex;
}
//recurring function to simulate possible moves the computer and player can make and compute a score for computer's win rate
int minimax(char board[], bool isComputer, int depth, int alpha, int beta)
{
    //check current board state for any win or draw condition
    char result = checkWin();
    //if there exist a win or draw condition
    if(result != 'N')
    {
        //if result returns 'O' means computer won, return positive score of (10-depth) since computer won
        if(result == 'O')
        {
            return (10-depth);
        }
        //if result returns 'X' means player won, return negative score of -(10-depth) since computer lost
        else if (result == 'X')
        {
            return -(10-depth);
        }
        //if no one won, return score 0
        else
        {
            return 0;
        }
    }
    //simulate computer move (O), maximizing
    if(isComputer)
    {
        //initialize computerBestScore with lowest score -2147483647 
        int computerBestScore = INT_MIN;
        //loop 9 times and test move on board space if it is empty
        for (int i = 0; i < 9; i++)
        {
            //check if current move space is available
            if(board[i] != 'X' && board[i] != 'O')
            {
                //test computer move on current empty space
                board[i] = 'O';
                //simulate possible moves the computer and player can make and compute score achieved
                //Set flag to false to simulate player move, depth + 1
                int score = minimax(board,false,depth++,alpha,beta);
                //reset current move space, depth
                board[i] = '-';
                depth--;
                //if score > computerBestScore, store score as computerBestScore
                //computerBestScore = max(computerBestScore, score);
                if (score > computerBestScore)
                {
                    computerBestScore = score;
                }
                // alpha/beta pruning
                //alpha is the best value the maximizer currently can guarantee at that level or above
                //beta is the best value that the minimizer currently can guarantee at that level or below
                //if computerBestScore computed > alpha score then change alpha score to current computerBestScore
                if(computerBestScore>alpha)
                {
                    alpha = computerBestScore;
                }
                //if there already exists a better move available, stop searching branch
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        //return computerBestScore
        return computerBestScore;
    }
    //simulate player move (X), minimizing
    else
    {
        //initialize playerBestScore with highest score 2147483647
        int playerBestScore = INT_MAX;
        //loop 9 times and test move on board space if it is empty
        for (int i = 0; i < 9; i++)
        {
            //check if current move space is available
            if(board[i] != 'X' && board[i] != 'O')
            {
                //test player move on current empty space
                board[i] = 'X';
                //simulate possible moves the computer and player can make and compute score achieved
                //Set flag to true to simulate computer move, depth + 1
                int score = minimax(board,true,depth++, alpha, beta);
                //reset current move space, depth
                board[i] = '-';
                depth--;
                //if score < playerBestScore, store score as playerBestScore
                //playerBestScore = min(playerBestScore, score);
                if (score < playerBestScore)
                {
                    playerBestScore = score;
                }
                // alpha/beta pruning
                //alpha is the best value the maximizer currently can guarantee at that level or above
                //beta is the best value that the minimizer currently can guarantee at that level or below
                //if playerBestScore computed < beta score then change beta score to current playerBestScore
                if(playerBestScore<beta)
                {
                    beta=playerBestScore;
                }
                //if there already exists a better move available, stop searching branch
                if(beta<=alpha)
                {
                    break;
                }
            }
        }
        //return playerBestScore
        return playerBestScore;
    }
    //return 0 when successfully completed function
    return(0);
}

// best move for dumb minimax
int bestMove_dumb(char board[])
{
    //initialize bestScore with -2147483647, bestMoveIndex with -1, alpha with -2147483647, beta with 2147483647
    int bestScore = INT_MIN,bestMoveIndex = -1, alpha = INT_MIN, beta = INT_MAX;
    //loop 9 times and test move on board space if it is empty to compute optimal move for computer
    for(int i = 0; i < 9; i++)
    {
        //check if current move space is available
        if(board[i] != 'X' && board[i] != 'O')
        {
            //if pityCounter < 2, AI will be Smartest
            if(pityCounter < 2)
            {
                //test computer move on current empty space
                board[i] = 'O';
                //simulate possible moves the computer and player can make and compute score achieved
                //Set flag to false to simulate player move, depth to 1
                int score = minimax(board, false, 1,alpha,beta);
                //reset current move space
                board[i] = '-';
                //if score computed is higher than bestScore
                if(score > bestScore)
                {
                    //update bestMoveIndex for computer to make to (i)
                    bestMoveIndex = i;
                    //update bestScore to score computed
                    bestScore = score;
                }
            }
            //if pityCounter >= 2, AI will be dumb
            else
            {
                bestMoveIndex = getRandomMove();
            }
        }
    }
    //return bestMoveIndex
    return bestMoveIndex;
}

// function to get player move input + validate if input is valid, returns player move once valid (INT)
int getRandomMove()
{
    // initialize variables
    bool valid = false;
    int move = 0;

    while (!valid) // while player input is valid
    {
        //get random move index from 0 to 8
        move = rand() % 9;
        // validate if move selection is valid, meaning board space is empty
        if (board[move] != 'X' && board[move] != 'O')
        {
            // return valid move
            return move;
        }
    }
    return 0;
}

//writeToFile
int writeToFile(char result)
{
    if (result!='N')
    {
        //Open the file to read and write
        FILE *fp;
        char sresult[50];
        fp = fopen("Dumbminimax_result.txt", "a");
        fseek(fp,0,SEEK_SET);
        //Read last line in the file
                
        //If player lose, write lose into the file
        if (result == 'O')
        {
            pityCounter++;
            fputs("Lose\n", fp);
            fclose(fp);
        }

        // if player win, write Win into the file
        if (result == 'X')
        {
            fputs("Win\n", fp);
            fclose(fp);
        }
        // if no one won, write Tie into the file
        if (result =='T')
        {
            fputs("Tie\n", fp);
            fclose(fp);
        }
    }
}
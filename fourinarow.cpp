/*Description:
	This version of 4 in a row implements the sorting algorithm of negamax (a more elegant verion of the Minimax algorithm)
	In this unfinished verion, the algorithm itself and sorting are completed, while the hueristic function is still being implemented.
	Also, the visual representation has not been done yet. Lastly, the alpha  beta pruning will need to be implemented
*/

#include <iostream>
#include <stdio.h>

using namespace std;


int runHeuristics(int col,int movesPlayed,int copiedarray[7][6]);

//initialize game board and rowindicator
int board[7][6], rowindicator[6] = {0,0,0,0,0,0}; //col by row, row

//current test maxDepth of 2
int MaxDepth = 2,moves = 0;	//row goes 1-6, col from 1 to 7
int movesPlayed;

//checks to see if a column has been filled
bool Filled(int col){
		//if filled, return false
		if(rowindicator[col] > 6)return true;
		else return false;
}
//play a players piece
void play(int col,int boards[7][6],int moves){
	boards[col][rowindicator[col]] = 1;
	rowindicator[col]++;
	moves++;
}
//play the CPU's piece
void playCPU(int col,int boards[7][6],int moves){
	//assign zero to all spots that the CPU has played
	boards[col][rowindicator[col]] = 0;
	rowindicator[col]++;
	moves++;
}

//function that will supplement the heuristic function by checking if either player is one move from winning,
//and if so, not going into the recursive call.

// bool oneMovetoWin(int col){
//
// }



//analyzes the game board to see if its the CPU's turn
bool CPUturn(int board[7][6]){
	int countcpu = 0,countplayer = 0;
	for(int col = 0;col < 7;col++){
		for(int row = 0;row < 6;row++){
			if(board[col][row] == 1)countplayer++;
			else if(board[col][row] == 0)countcpu++;
		}
	}
		//player always plays first
		if(countcpu < countplayer)return true;
		else{
			return false;
		}
}


void copy(int newArray[7][6],int two_d_array[7][6]);

int dropin;
int lastScore=0;

// const int sign[2]={1,-1}   //0 is cpu, 1 is player
const int sign[2]={1,-1};   //0 is cpu (yellow), 1 is player (white)

int negamax(int depth, int board[7][6],int color){
	// int rowindicatorcopiedarray[6] = {0,0,0,0,0,0};
	int copiedarray[7][6];

	if(moves > 42 || depth >= MaxDepth){
		// cout << "lastScore is " << lastScore<<endl;
		return sign[color]*lastScore;
	}
	//check for the possibility of a win here


	//negative infinity
	int max = -1000;

	for(int col = 1;col <=7;col++){
		if(!Filled(col-1)){
			//copy array
			copy(copiedarray,board);

		  //if count is even, play the player, else play the cpu
			if(!CPUturn(copiedarray))play(col-1,copiedarray,movesPlayed);
			else playCPU(col-1,copiedarray,movesPlayed);
			movesPlayed++;
			//recurse
			int scoreCalculated = -negamax(depth+1,copiedarray,1-color);
			scoreCalculated = runHeuristics(col-1,movesPlayed,copiedarray);

			if(scoreCalculated > max){

				max = scoreCalculated;
				lastScore = max;
				// cout << "max is: " <<max <<endl;
				dropin = col-1;
			}
		}
	}

	return max;
}
//simply put, going to use a linear weighting function that will calculate
//the value/score as being MyMaterial-OpponentsMaterial. The way this works is
//my material will consist of the amount of 1 in a rows I have (score of 1)
//plus the amount of 2 in a rows and 3 in a rows subtracted from the players equivalent
int runHeuristics(int col,int movesPlayed,int copiedarray[7][6]){
	//play the cpu's piece at a random (column 2) spot
	int cpuscore = 0,playerscore = 0;
	//calculate cpu score for all cases
	for(int column = 1;column <= 7;column++){
		for(int row = 1;row <= 6;row++){
			/*1 in a row																						*/

			//1 filled, 1 empty on the right,diagonal
			if(copiedarray[column-1][row-1] == 0 && (copiedarray[column][row] == 100) && copiedarray[col+1][row+1] == 100 && copiedarray[col+2][row+2] == 100 && col <= 4 && row <= 3)cpuscore++;
			//1 filled, 1 empty on the left,diagonal
			if(copiedarray[column-1][row-1] == 0 && (copiedarray[column-2][row] == 100) && (copiedarray[column-3][row] == 100) && (copiedarray[column-4][row] == 100) && col>=4 && row <= 3)cpuscore++;

			//1 filled, 1 empty on the right, horizontal
			if(copiedarray[column-1][row-1] == 0 && (copiedarray[column][row-1] == 100) && (copiedarray[column+1][row-1] == 100) && (copiedarray[column+2][row-1] == 100) && col <=4)cpuscore++;
			//1 filled, 1 empty on the left, horizontal
			if((copiedarray[column-1][row-1] == 0) && (copiedarray[column-2][row-1] == 100) && (copiedarray[column-3][row-1] == 100) && (copiedarray[column-4][row-1] == 100) && col >=4)cpuscore++;

			//1 filled, 1 empty above, vertical
			if(copiedarray[column-1][row-1] == 0 && (copiedarray[column-1][row] == 100) && copiedarray[column-1][row+1] == 100 && copiedarray[column-1][row+2] == 100 && row <=3)cpuscore++;

			/*1 in a row																						*/

			/*2 in a row																						*/
			//diagonal, 2 filled, 1 empty on right
			if(copiedarray[column-1][row-1] == 0 && copiedarray[column][row] == 0 && copiedarray[column+1][row+1] == 100 && copiedarray[column+2][row+2] == 100 && row <= 3 && column <= 4)cpuscore+=3;
			//diagonal, 2 filled, 1 empty on left
			if(copiedarray[column-1][row-1] == 0  && copiedarray[column-2][row-2] == 0 && copiedarray[column-3][row-3] == 100 && copiedarray[column-4][row-4] == 100 &&  row <=3 && column >=4)cpuscore+=3;

			//horizontal, 2 filled, 1 empty on right
			if(copiedarray[column-1][row-1] == 0 && copiedarray[column][row-1] == 0 && copiedarray[column+1][row-1] == 100 && copiedarray[column+2][row-1] == 100 && column <= 4)cpuscore+=3;
			//horizontal, 2 filled, 1 empty on left
			if(copiedarray[col-4][row-1] == 100 && copiedarray[col-3][row-1] == 100 && copiedarray[column-2][row-1] == 100 && copiedarray[column-1][row-1] == 0 && column >=4)cpuscore+=3;

			//vertical, 2 filled, 1 empty above
			if(copiedarray[column-1][row-1] == 0 && copiedarray[column-1][row] == 0 && row <= 3 && copiedarray[column-1][row+1] == 100 && copiedarray[column-1][row+2] == 100)cpuscore+=3;
			/*2 in a row																						*/

		// 	/*3 in a row																						*/
		// 	//diagonal, 3 filled, 1 empty on right
			if(copiedarray[column-1][row-1] == 0 && copiedarray[column][row] == 0 && copiedarray[column+1][row+1] == 0 && copiedarray[column+2][row+2] == 100 && row <= 3 && column <= 4)cpuscore+=7;
			// diagonal, 3 filled, 1 empty on left
			if(copiedarray[column+2][row-1] == 0 && copiedarray[column+1][row] == 0 && copiedarray[column][row+1] == 0 && copiedarray[column-1][row+2] == 100 && col >=4 && row <= 3)cpuscore+=7;

			//horizontal, 3 filled, 1 empty on right
			if(copiedarray[column-1][row-1] == 0 && copiedarray[column][row-1] == 0 && copiedarray[column+1][row-1] == 0 && (copiedarray[column+2][row-1] == 100) && column <=4)cpuscore+=7;
			//horizontal, 3 filled, 1 empty on left
			if(copiedarray[column+2][row-1] == 0 && copiedarray[column+1][row-1] == 0 && copiedarray[column][row-1] == 0 && (copiedarray[column-1][row-1] == 100) && column >=4)cpuscore+=7;

			//vertical, 3 filled, 1 empty above
			if(copiedarray[column-1][row-1] == 0 && copiedarray[column-1][row] == 0  && copiedarray[column-1][row+1] == 0 && copiedarray[column-1][row+2] == 100 && row <= 3)cpuscore+=7;
		// }
	}
}

	//player score
	for(int column = 1;column <= 7;column++){
		for(int row = 1;row <= 6;row++){
			/*1 in a row																						*/

			//1 filled, 1 empty on the right,diagonal
			if(copiedarray[column-1][row-1] == 1 && (copiedarray[column][row] == 100) && copiedarray[col+1][row+1] == 100 && copiedarray[col+2][row+2] == 100 && col <= 4 && row <= 3)playerscore++;
			//1 filled, 1 empty on the left,diagonal
			if(copiedarray[column-1][row-1] == 1 && (copiedarray[column-2][row] == 100) && (copiedarray[column-3][row] == 100) && (copiedarray[column-4][row] == 100) && col>=4 && row <= 3)playerscore++;

			//1 filled, 1 empty on the right, horizontal
			if(copiedarray[column-1][row-1] == 1 && (copiedarray[column][row-1] == 100) && (copiedarray[column+1][row-1] == 100) && (copiedarray[column+2][row-1] == 100) && col <=4)playerscore++;
			//1 filled, 1 empty on the left, horizontal
			if((copiedarray[column-1][row-1] == 1) && (copiedarray[column-2][row-1] == 100) && (copiedarray[column-3][row-1] == 100) && (copiedarray[column-4][row-1] == 100) && col >=4)playerscore++;

			//1 filled, 1 empty above, vertical
			if(copiedarray[column-1][row-1] == 1 && (copiedarray[column-1][row] == 100) && copiedarray[column-1][row+1] == 100 && copiedarray[column-1][row+2] == 100 && row <=3)playerscore++;

			/*1 in a row																						*/

			/*2 in a row																						*/
			//diagonal, 2 filled, 1 empty on right
			if(copiedarray[column-1][row-1] == 1 && copiedarray[column][row] == 1 && copiedarray[column+1][row+1] == 100 && copiedarray[column+2][row+2] == 100 && row <= 3 && column <= 4)playerscore+=3;
			//diagonal, 2 filled, 1 empty on left
			if(copiedarray[column-1][row-1] == 1  && copiedarray[column-2][row-2] == 1 && copiedarray[column-3][row-3] == 100 && copiedarray[column-4][row-4] == 100 &&  row <=3 && column >=4)playerscore+=3;

			//horizontal, 2 filled, 1 empty on right
			if(copiedarray[column-1][row-1] == 1 && copiedarray[column][row-1] == 1 && copiedarray[column+1][row-1] == 100 && copiedarray[column+2][row-1] == 100 && column <= 4)playerscore+=3;
			//horizontal, 2 filled, 1 empty on left
			if(copiedarray[col-4][row-1] == 100 && copiedarray[col-3][row-1] == 100 && copiedarray[column-2][row-1] == 1 && copiedarray[column-1][row-1] == 1 && column >=4)playerscore+=3;

			//vertical, 2 filled, 1 empty above
			if(copiedarray[column-1][row-1] == 1 && copiedarray[column-1][row] == 1 && row <= 3 && copiedarray[column-1][row+1] == 100 && copiedarray[column-1][row+2] == 100)playerscore+=3;
			/*2 in a row																						*/

		// 	/*3 in a row																						*/
			//diagonal, 3 filled, 1 empty on right
			if(copiedarray[column-1][row-1] == 1 && copiedarray[column][row] == 1 && copiedarray[column+1][row+1] == 1 && copiedarray[column+2][row+2] == 100 && row <= 3 && column <= 4)playerscore+=7;
			// diagonal, 3 filled, 1 empty on left
			if(copiedarray[column+2][row-1] == 1 && copiedarray[column+1][row] == 1 && copiedarray[column][row+1] == 1 && copiedarray[column-1][row+2] == 100 && col >=4 && row <= 3)playerscore+=7;

			//horizontal, 3 filled, 1 empty on right
			if(copiedarray[column-1][row-1] == 1 && copiedarray[column][row-1] == 1 && copiedarray[column+1][row-1] == 1 && (copiedarray[column+2][row-1] == 100) && column <=4)playerscore+=7;
			//horizontal, 3 filled, 1 empty on left
			if(copiedarray[column+2][row-1] == 1 && copiedarray[column+1][row-1] == 1 && copiedarray[column][row-1] == 1 && (copiedarray[column-1][row-1] == 100) && column >=4)playerscore+=7;

			//vertical, 3 filled, 1 empty above
			if(copiedarray[column-1][row-1] == 1 && copiedarray[column-1][row] == 1  && copiedarray[column-1][row+1] == 1 && copiedarray[column-1][row+2] == 100 && row <= 3)playerscore+=7;
		// }
	}
}
			//score to be returned for move
			int val = 0;
			//heres the score for that specific piece
			if(!CPUturn(copiedarray))val = (playerscore)-cpuscore;
			else val = cpuscore - playerscore;

			cout << "value is: " << val << endl;
			//return this value to negamax
			return val;
}

//simply copies the game board to an auxillary board
void copy(int newArray[7][6],int two_d_array[7][6]){
	for (int i = 0; i < 7; i++){
		rowindicator[i] = 0;
		for (int j = 0; j < 6; j++){
      newArray[i][j] = two_d_array[i][j];
			if(two_d_array[i][j] == 1 ||two_d_array[i][j] == 0)rowindicator[i]++;

			}
		}
}

int main() {
	//prefill with a number not 1 or 0
	for(int col = 0;col < 7;col++)
		for(int row = 0;row < 6;row++){
			board[col][row] = 100;
		}

	//maximum amount of moves is 42
	for(int totmoves = 0; totmoves < 21;totmoves++){
		cout << "select where to drop" <<endl;
		int columnpicked = 0;
		cin >> columnpicked;
		//actually play to the real gameboard
		play(columnpicked,board,moves);
		cout << " total movees is: " <<moves<<endl;



		//color:0 is yellow,1 is white (0 is cpu, 1 is player)
		//find and generate the best score, select column associated with it
		int maxscore = negamax(0,board,0);
		cout << "negamax score " << maxscore << endl;
		cout << "col selected is: " << dropin;
		//play it there for the cpu
		playCPU(dropin,board,moves);
		cout << " total movees is: " <<moves<<endl;


	}
	return 0;
}

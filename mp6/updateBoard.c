/*
This program is the game of life. Uses 1-d arrays
to represent the game board.
For every element of the board, this program counts the number of live neighbors
and decide what it should be on the next step. It uses compareboard which compares 
the newly created board to the original board.
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
 int livenum = 0;
	int i, j;
	for (i = row - 1; i <= row + 1; i++){																	//count 3 rows, 3 columns
		if ( (i >= 0) && (i <= boardRowSize - 1) ){													//9 elements in total
			for (j = col - 1; j <= col + 1; j++){
				if ( (j >= 0) && (j <= boardColSize - 1)){
					if (*(board + i * boardColSize + j) == 1){
						livenum++;
					}
				}
			}
		}
	}
	livenum = livenum - *(board + row * boardColSize + col);							//finally subtract the result with the analyzed element itself
	return livenum;

}
/*
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
   
void updateBoard(int* board, int boardRowSize, int boardColSize){
 //variable initialization
  int a;
  int b;
  int c;
  int count;
  int totalsize = boardRowSize * boardColSize;
  int updatedboard[totalsize];
  for (a = 0; a < boardRowSize; a++) //called multiple times throughout the program. 
  {				     //used to update the whole board and repaint
    for (b = 0; b < boardColSize; b++)
    {
      count = countLiveNeighbor(board, boardRowSize, boardColSize, a, b);
      if (board[a * boardColSize + b] == 1)
      {
        if(count == 2 || count == 3)
        {
          updatedboard[a * boardColSize + b] = 1; //for number of live neighbors 2 <= x <= 3
        }
        else
        {
          updatedboard[a * boardColSize + b] = 0;
        }
      }
      else if (board[a * boardColSize + b] == 0)
      {
        if (count == 3) //bring cell to life
        {
          updatedboard[a * boardColSize + b] = 1;
        }
        else
        {
          updatedboard[a * boardColSize + b] = 0;
        }
      }
     
    }//b for loop
  }//a for loop
  for (c=0; c < totalsize; c++) //recursive call
  {
    board[c] = updatedboard[c];
  }
}//final bracket

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */ 
int aliveStable(int* board, int boardRowSize, int boardColSize){
    int alive = 1;
	int count = 0;
	int compareboard[boardRowSize * boardColSize];
	int i, j;
	for (i = 0; i <= boardRowSize - 1 ; i++){//duplicate the original board to the new board
		for (j = 0; j <= boardColSize - 1; j++){
			compareboard[i * boardColSize + j] = *(board + i * boardColSize + j);
		}
	}
	for (i = 0; i <= boardRowSize - 1; i++){//do the same thing again to get updated result in the new board
		for (j = 0; j <= boardColSize - 1; j++){
			count = countLiveNeighbor (board, boardRowSize, boardColSize, i, j);
			if (count < 2){
				compareboard[i * boardColSize + j] = 0;
			}
			if (count == 3){
				compareboard[i * boardColSize + j] = 1;
			}
			if (count > 3){
				compareboard[i * boardColSize + j] = 0;
			}
		}
    }
	for (i = 0; i <= boardRowSize - 1; i++){//compare the new board with old board
		for (j = 0; j <= boardColSize - 1; j++){
			if (compareboard[i * boardColSize + j] != *(board + i * boardColSize + j) ){//if there is one element different, return alive = 0
				alive = 0;
			}
		}
	}
	return alive;
}

				
				
			


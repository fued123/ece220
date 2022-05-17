#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

/*
This is the intro paragraph:
This code implements a maze solver using the recursive DFS algorithm.
This code also has a few functions, which simply verify that the solution
that the code proivdes is actually valid.
The first two numbers in out put are the dimensions of the maze (width*height).
The character '%' represents a wall of the maze.
The character 'S' represents the starting point of the maze.
The character 'E' represents the end point of the maze. 
*/

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
     int i, j, rows, cols;
    char c;
    FILE *reader = fopen(fileName, "r");//create a stream

    fscanf(reader, "%d %d", &cols, &rows);
    maze_t *mazecreated = malloc(sizeof(maze_t)); //allocate memory for the maze_t structure
    mazecreated -> height = rows;
    mazecreated -> width = cols;
    mazecreated -> cells = (char **)malloc(rows * sizeof(char *)); //allocate memory for the cells
    for (i = 0; i < rows; i++){
      mazecreated -> cells[i] = (char *)malloc(cols * sizeof(char));
    }
    for (i = 0; i < rows; i++){
      for (j = 0; j < cols; j++){
        c = fgetc(reader); //duplcate the maze in the file
        if (c != '\n'){
          mazecreated -> cells[i][j] = c;
        }
        else{ //if it's a newline character
            j--; //do not save, and loop once more
        }
        if (j >= 0){ //without this line there will be an error accessing cells[i][-1]
          if (mazecreated -> cells[i][j] == START){ //record the start point
            mazecreated -> startRow = i;
            mazecreated -> startColumn = j;
          }
          if (mazecreated -> cells[i][j] == END){ //record the end point
            mazecreated -> endRow = i;
            mazecreated -> endColumn = j;
          }
        }
      }
    }
    fclose(reader);//close the stream

    return mazecreated;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    int i;
    for(i = 0; i < maze->height; i++){
        free(maze->cells[i]); //unallocating the memory for each cell
    }
    free(maze->cells); //unallocating memory for each array
    free(maze); //unallocating the memory for the cell itself
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    int i, j;
    for(i = 0; i < maze->height; i++){
        for(j = 0; j < maze->width; j++){
            printf("%c", maze->cells[i][j]); //looping through and printing all the necessary cells based off of the height and the width values
        }
        printf("\n"); //printing new line
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.

    //false if out of bounds
    if (col < 0 || col >= maze -> width || row < 0 || row >= maze -> height){
      return 0;
    }
    //false if not an empty cell
    if (maze -> cells[row][col] == WALL || maze -> cells[row][col] == PATH || maze -> cells[row][col] == VISITED){
      return 0;
    }
    //true if reach the end
    if (col == maze -> endColumn && row == maze -> endRow){
      //restore the starting point before reaching the end (VERY IMPORTANT!!!)
      maze -> cells[maze -> startRow][maze -> startColumn] = START;
      return 1;
    }
    //set (col, row) as part of the solution path in the maze
    if (maze -> cells[row][col] != END){          //this part writes PATH at starting point, but will be restored at last
      maze -> cells[row][col] = PATH;
    }

    //true if left (or other direction) true
    if (solveMazeDFS(maze, col - 1, row)){
      return 1;
    }
    if (solveMazeDFS(maze, col, row - 1)){
      return 1;
    }
    if (solveMazeDFS(maze, col + 1, row)){
      return 1;
    }
    if (solveMazeDFS(maze, col, row + 1)){
      return 1;
    }
    //unmark (col, row) as part of solution and mark as visited
    if (maze -> cells[row][col] != START && maze -> cells[row][col] != END){
      maze -> cells[row][col] = VISITED;
    }

    return 0;
}

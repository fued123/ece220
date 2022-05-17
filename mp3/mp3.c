/*This is a code to print one row of 
Pascal's Triangle using two for loops
and iterating*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
  int row;

  printf("Enter a row index: ");
  scanf("%d",&row);

  // Write your code here
  unsigned long i, j, result; //defining all the variables

  for(j=0; j <= row; j++){  //defining the first number as 1
      
      result = 1;

      for(i=1; i <= j; i++){

      result = (result * (row + 1 - i) / i);
  }
    printf("%lu", result);   //outputting the row
} 
  return 0;
}

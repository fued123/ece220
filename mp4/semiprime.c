#include <stdlib.h>
#include <stdio.h>

/* There were a couple of bugs/errors in the original semiprime code with the first being in is_prime. The function
wasn't checking for separate test cases for when the integers were less than one or if they were equal to 2. This meant that
when ever "print_semiprime" looped back to the "is_prime" function the function would not be checking for the correct values
in order to print the semiprimes. In "print_semiprimes" the main issue was in the if/else statements because the function would
not call is_prime properly meaning that no values would be printed because there was no "is_print" calls. */


/*
 * is_prime: determines whether the provided number is prime or not
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number)
{
    int i;
    if (number <= 1){ //test case for all integers less than 1
        return 0;
    }
    if (number % 2 == 0 && number > 2) { //test case for 2
        return 0;
    }
    for (i = 3; i < number; i++) { //for each number smaller than it
        if (number % i == 0) { //check if the remainder is 0
            return 0;
        }
    }
    return 1;
}


/*
 * print_semiprimes: prints all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b){
    int i, j;
    int ret = 0;
    for (i = a; i <= b; i++) { //for each item in interval
        //check if semiprime
        for (j = 2; j < i-1; j++) {
            if ((i % j == 0) && is_prime(j) && is_prime(i/j)) { //for i to be semiprime, j must be a factor of i AND also prime, and i/j must also be prime
                if (ret == 1) {
                    printf(" ");
                    }
                    printf("%d ", i);
                    ret = 1; //setting flag in order to break from this inner loop
                    break;
                }
            }
        }
    printf("\n");
    return ret;

}

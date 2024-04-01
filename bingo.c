#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BINGO_SIZE 5
#define BINGO_MIDDLE 2
#define B 0
#define I 1
#define N 2
#define G 3
#define O 4

 /*This function is created to be used when we find a given number in a specific 
 column from the matrix. The element from the resulted one is marked with -1.
 We search in every bingo board.
 */
void colour_cell(int results[][BINGO_SIZE][BINGO_SIZE], int columns[][BINGO_SIZE],
                 int nr_of_tickets, int column, int nr) {
    int i, j;
    for (i = 0; i < nr_of_tickets; i++) {
        int current_column = columns[i][column];
        for (j = 0; j < BINGO_SIZE; j++) {
            if (results[i][j][current_column] == nr) {
                results[i][j][current_column] = -1;
            }
        }
    }
}

/*This function is created to be used in every swap case.
*/

void my_swap(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

/*This functions is used for every bingo and results board to swap
 a column with another one.
*/

void swap(int results[][BINGO_SIZE][BINGO_SIZE], int tickets[][BINGO_SIZE][BINGO_SIZE],
          int nr_of_tickets, int columns[][BINGO_SIZE], int col1, int col2) {
    int i, j;
    for (i = 0; i < nr_of_tickets ; i++) {
        for (j = 0; j < BINGO_SIZE; j++) {
            my_swap(&results[i][j][col1], &results[i][j][col2]);
            my_swap(&tickets[i][j][col1], &tickets[i][j][col2]);
        }

        int swap_col1, swap_col2;
        for (j = 0; j < BINGO_SIZE; j++) {
            if (columns[i][j] == col1) {
                swap_col1 = j; // Retin coloanele pe care se afla indicii cautati.
            }

            if (columns[i][j] == col2) {
                swap_col2 = j;
            }
        }

        my_swap(&columns[i][swap_col1], &columns[i][swap_col2]);
    }
}

/* This function is used when we have to shift a specific column with a 
specific positions from the bingo boars and also the results boards.
We initialise i with 3 and j with 4 so we begin shifting the last element
from a given column with the first one. Then we lower i, j, and we continue 
shifting with the number of positions that has remained.
*/

void shift(int results[][BINGO_SIZE][BINGO_SIZE], int tickets[][BINGO_SIZE][BINGO_SIZE],
           int nr_of_tickets, int column, int positions) {
    int ticket, k;
    for (k = 0; k < positions; k++) {
        int i = BINGO_SIZE - 2;
        int j = BINGO_SIZE - 1;

        do {
            for (ticket = 0; ticket < nr_of_tickets; ticket++) {
                my_swap(&results[ticket][i][column], &results[ticket][j][column]);
                my_swap(&tickets[ticket][i][column], &tickets[ticket][j][column]);
            }
    
            i--;
            j--;
        
        } while (i >= 0);
    } 
} 

/*This function verify if 2 numbers are ordered ascending, 
and it will be called in the 'sort_column' function.
*/

int asc_compare_function(int first, int second) {
    if (first > second) {
        return 1;
    }

    return -1;
}

/*This function verifies if 2 numbers are ordered descending, 
and it will be called in the 'sort_column' function.
*/

int dsc_compare_function(int first, int second) {
    if (first < second) {
        return 1;
    }

    return -1;
}

/*This function is used to sort a column ascending or descending in any bingo 
board, by case. Both 'asc_compare_function' and 'dsc_compare_function' 
are called if the condition is fulfill. The final results can be seen
in the bingo board and also in the resulted one.
*/

void sort_column(int tickets[][BINGO_SIZE][BINGO_SIZE], int results[][BINGO_SIZE][BINGO_SIZE],
                 int nr_of_tickets, int column, int (*compare_function)(int first, int second))
{

    //Compare function este un pointer catre o functie care intoarce int, la fel si asc si dsc
    int i, j, k;
    for (i = 0; i < nr_of_tickets; i++) {
        for (j = 0; j < BINGO_SIZE - 1; j++) {
            for (k = j + 1; k < BINGO_SIZE; k++) {
                if (compare_function(tickets[i][j][column], tickets[i][k][column]) > 0) {
                    my_swap(&tickets[i][j][column], &tickets[i][k][column]);
                    my_swap(&results[i][j][column], &results[i][k][column]);
                }
            }
        }
    }
}

/*The 'show' function is used to mark in the results boards the numbers
 that are found with #, and after the matrix with a specific number 
 to be printed.
*/

void show(int results[][BINGO_SIZE][BINGO_SIZE], int ticket_nr) {
    int i, j;
    for (i = 0; i < BINGO_SIZE; i++){
        for (j = 0; j < BINGO_SIZE; j++) {
            if (results[ticket_nr][i][j] == -1) {
                printf("# ");
            } else {
                printf("%d ", results[ticket_nr][i][j]);
            }
        }
        printf("\n");
    }
}

/*This function checks if a specific line from the bingo board is 
marked with "-1". If so we have to add 10 points to the player.
*/

void check_line(int ticket[BINGO_SIZE][BINGO_SIZE], int line, int *points) {
    int i;
    for (i = 0; i < BINGO_SIZE; i++) {
        if (ticket[line][i] != -1) {
            return;
        }
    }

    *points = *points + 10;
}

/*This function checks if a specific column from the bingo board is 
marked with "-1".If so we have to add 10 points to the player.
*/

void check_column(int ticket[BINGO_SIZE][BINGO_SIZE], int column, int *points) {
    int i;
    for (i = 0; i < BINGO_SIZE; i++) {
        if (ticket[i][column] != -1) {
            return;
        }
    }

    *points = *points + 10;
}

/*This function checks if the first diagonal from the bingo board is
 marked with "-1".If so we have to add 10 points to the player. 
*/

void check_first_diagonal(int ticket[BINGO_SIZE][BINGO_SIZE], int *points) {
    int i, j;
    for (i = 0; i < BINGO_SIZE; i++) {
        for (j = 0; j < BINGO_SIZE; j++) {
            if (i == j && ticket[i][j] != -1) {
                return;
            }
        }
    }

    *points = *points + 10;
}

/*This function checks if the second diagonal from the bingo board is
 marked with "-1".If so we have to add 10 points to the player. 
*/

void check_second_diagonal(int ticket[BINGO_SIZE][BINGO_SIZE], int *points) {
    int i, j;
    for (i = 0; i < BINGO_SIZE; i++) {
        for (j = 0; j < BINGO_SIZE; j++) {
            if (i + j == BINGO_SIZE - 1 && ticket[i][j] != -1) {
                return;
            }
        }
    }

    *points = *points + 10;
}

/*This function verifies the total points of every bingo board and shows the
 winner.In case that are 2 boards with the same points, 
it will be printed the one with the smallest ID.
*/

void check_winner(int results[][BINGO_SIZE][BINGO_SIZE], int nr_of_tickets, int *has_won) {
    int i, j;
    for (i = 0; i < nr_of_tickets; i++) {
        int points = 0;
        for (j = 0; j < BINGO_SIZE; j++) {
            check_line(results[i], j, &points);
        }
        for (j = 0; j < BINGO_SIZE; j++) {
            check_column(results[i], j, &points);
        }
        check_first_diagonal(results[i], &points);
        check_second_diagonal(results[i], &points);

        if (points != 0) {
            printf("%d\n", i);
            show(results, i);
            printf("%d\n", points);
            *has_won = 1;
            return;
        }
    }
}
 
int main()
{
    int nr_of_tickets;
    scanf("%d", &nr_of_tickets);

    int tickets[nr_of_tickets][BINGO_SIZE][BINGO_SIZE];
    int results[nr_of_tickets][BINGO_SIZE][BINGO_SIZE];
    int columns[nr_of_tickets][BINGO_SIZE];
    int i, j, k;
    for (i = 0; i < nr_of_tickets; i++) {
        for (j = 0; j < BINGO_SIZE; j++) {
            for (k = 0; k < BINGO_SIZE; k++) {
                 //The bingo board is read and the results one 
                 //is initialise with the same elements as the first board.
                scanf("%d", &tickets[i][j][k]);
                results[i][j][k] = tickets[i][j][k];
            }
        }

         //We mark the middle element from every bingo board.
        results[i][BINGO_MIDDLE][BINGO_MIDDLE] = -1;  
    }

    for (i = 0; i < nr_of_tickets; i++) {
        for (j = 0; j < BINGO_SIZE; j++) {
            columns[i][j] = j; // Retin coloanele.
        }
    }

    int nr_of_commands;
    //We read the number of commands that are given.
    scanf("%d", &nr_of_commands);

    int has_won = 0;
    for (i = 0; i < nr_of_commands; i++) {
        char buffer[25];
        scanf("%s", buffer);

        char *command;
        /*For each command we make a separation between the name of the command and its parameters.
        */
        command = strtok(buffer, "-");    
                                           
        int offset = strlen(command) + 1;
        
        /*We check on whitch column we are so we know where to search the number.
        */
        if (buffer[0] == 'B' || buffer[0] == 'I' || buffer[0] == 'N'    
            || buffer[0] == 'G' || buffer[0] == 'O') { 
            int current_column = -1; 
            switch (buffer[0]) { 
                case 'B':
                    current_column = B;
                    break;
                case 'I':
                    current_column = I;
                    break;
                case 'N':
                    current_column = N;
                    break;
                case 'G':
                    current_column = G;
                    break;
                case 'O':
                    current_column = O;
                    break;
                default:
                    break;
            }

            colour_cell(results, columns, nr_of_tickets, current_column, atoi(buffer + 2));
        } else if (strcmp(command, "SHOW") == 0) {                        //We compare with the function strcmp the 
            show(results, atoi(buffer + offset));                        // given string (command) and all the 
        } else if (strcmp(command, "ASC") == 0) {                      //possible special names(show/asc/dsc/shift/swap).
            sort_column(tickets, results, nr_of_tickets, atoi(buffer + offset), asc_compare_function);
        } else if (strcmp(command, "DSC") == 0) {
            sort_column(tickets, results, nr_of_tickets, atoi(buffer + offset), dsc_compare_function);
        } else if (strcmp(command, "SHIFT") == 0) {
            char *column = strtok(NULL, "-");          //We delimitate the string 
            char *positions = strtok(NULL, "-");       //from the column and the number of positions.
            shift(results, tickets, nr_of_tickets, atoi(column), atoi(positions));
        } else if (strcmp(command, "SWAP") == 0) {
            char *col1 = strtok(NULL, "-");           //We delimitate the string 
            char *col2 = strtok(NULL, "-");           //from the first and second column.
            swap(results, tickets, nr_of_tickets, columns, atoi(col1), atoi(col2));
        }
        /*After the execution of each command, we check 
        before moving on to the next iteration whether 
        a winning ticket has appeared;
        */
        check_winner(results, nr_of_tickets, &has_won);     
        if (has_won == 1) {                                
            return 0;                                       
        }  
    }

    if (has_won == 0) {
        printf("NO WINNER\n");
    }
    return 0;
}

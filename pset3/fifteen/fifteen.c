/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;


// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
void swap(int* num1, int* num2);
bool move(int tile);
bool won(void);
void get_index(int target, int array[DIM_MAX][DIM_MAX],int *row, int *col);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);


        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();


        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);


        // move if possible, else report illegality
        if (!move(tile) || tile > d * d - 1)
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int num = d * d - 1;

    //iterate through rows and columns
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d && num > 0; col++, num--)
        {
            if (num == 2 && d % 2 == 0)
            {
                board[row][col] = 1;
                board[row][col + 1] = 2;
                break;
            }
            board[row][col] = num;
        }
    }

}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    //iterate through rows and columns
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            int num = board[row][col];

            // avoid printing the zero
            if (num == 0)
            {
                // print 7 spaces instead
                printf("       ");
                continue;
            }

            printf("%d", num);

            // put (6) spaces according to number of digits
            // to be aligned perfecly
            printf("     ");
            if (num < 10)
                printf(" ");
        }
        printf("\n");
    }

    
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // get index
    int row = 0, col = 0;
    get_index(tile, board, &row, &col);

    // check positions if there is a 0
    bool right_zero = col < d - 1 && board[row][col + 1] == 0,
    left_zero = col > 0 && board[row][col - 1] == 0,
    up_zero = row > 0 && board[row -1][col] == 0,
    down_zero = row < d -1 && board[row + 1][col] == 0;
    
    // take decision
    if (right_zero)
    {
        swap(&board[row][col], &board[row][col + 1]);
        return true;
    }
    else if (left_zero)
    {
        swap(&board[row][col], &board[row][col - 1]);
        return true;
    }
    else if (up_zero)
    {
        swap(&board[row][col], &board[row - 1][col]);
        return true;
    }
    else if (down_zero)
    {
        swap(&board[row][col], &board[row + 1][col]);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // use this to check
    int num = 1;

    // iterate through all numbers  
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d && num < d*d; col++, num++)
        {
            if (num != board[row][col])
                return false;  // numbers are not sorted
        }
    }

    // getting out of loops means board is sorted
    return true;
}

/**
* swaps two integers
*/
void swap(int *num1, int *num2)
{
    int temp;

    temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

/**
* linear search for integer in 2D array.
* pass row and col integers to be assigned here.
* if target not found, returns nothing.
*/
void get_index(int target, int array[DIM_MAX][DIM_MAX],int *row, int *col)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (array[i][j] == target)
            {
                *row = i;
                *col = j;
                return;
            }
        }
    }
    return;
}
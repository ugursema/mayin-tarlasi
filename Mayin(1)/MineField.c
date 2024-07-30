#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <unistd.h>

#define MAX_SIZE 20
#define true 1
#define false 0

// Function declarations
void initializeBoard(int size, char** board, int* startRow, int* startCol, int* endRow, int* endCol);
void printBoard(int size, char** board, int currentRow, int currentCol, bool finishedOrNot);
void generateMines(int size, char** board);
void updateUnsafePoints(int size, char** board, int row, int col);
void playGame(int size, char** board, int startRow, int startCol, int endRow, int endCol);

int main() {
    int size;
    char** board;
    int startRow, startCol, endRow, endCol;

    // Get board size from the user
    printf("Enter the size of the board: ");
    scanf("%d", &size);

    // Check if the size is within the allowed range
    if (size < 2 || size > MAX_SIZE) {
        printf("Invalid board size. Please choose a size between 2 and %d.\n", MAX_SIZE);
        return 1;
    }

    // Allocate memory for the board
    board = (char**)malloc(size * sizeof(char*));
    int i;
    for (i = 0; i < size; i++) {
        board[i] = (char*)malloc(size * sizeof(char));
    }

    // Initialize the board, start, and end points
    initializeBoard(size, board, &startRow, &startCol, &endRow, &endCol);

    // Play the game
    playGame(size, board, startRow, startCol, endRow, endCol);

    // Free allocated memory
    for (i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);

    return 0;
}

void initializeBoard(int size, char** board, int* startRow, int* startCol, int* endRow, int* endCol) {
    // Initialize the board with safe points
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board[i][j] = '.';
        }
    }

    // Set start and end points
    *startRow = 0;
    *startCol = 0;
    *endRow = size - 1;
    *endCol = size - 1;

    // Generate mines on the board
    generateMines(size, board);

    // Update unsafe points based on mine placement
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == 'o') {
                updateUnsafePoints(size, board, i, j);
            }
        }
    }
}

void printBoard(int size, char** board, int currentRow, int currentCol, bool finishedOrNot) {
    system("cls"); // Clear the console screen
    int i, j;

    if(finishedOrNot == false){
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if (i == currentRow && j == currentCol) 
                    printf("P "); // Highlight the current position with 'P'
                else 
                    printf(". ");
            }
            printf("\n");
        }
        printf("\n\nPress 'R' to regenerate board\n");

    }else{
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if(board[i][j] == '$')
                    printf("\x1b[33m%c\x1b[0m ", board[i][j]);
                else if(board[i][j] == 'x')
                    printf("\x1b[31m%c\x1b[0m ", board[i][j]);
                else if(board[i][j] == 'o')
                    printf("%c ", board[i][j]);
                else
                    printf(". ");
            }
            printf("\n");
        }
    }
}

void generateMines(int size, char** board) {
    int mineCount = size * size / 20; // 5% of the board size

    srand(time(NULL));
    while (mineCount > 0) {
        
        int row = rand() % size;
        int col = rand() % size;

        // Check if the current position is already a mine or start/end point
        if (board[row][col] != 'o' && !(row == 0 && col == 0) && !(row == size - 1 && col == size - 1) && !(row == 1 && col == 0) && !(row == 0 && col == 1) && !(row == size - 2 && col == size - 1) && !(row == size - 1 && col == size - 2)) {
            board[row][col] = 'o';
            mineCount--;
        }
    }
}

void updateUnsafePoints(int size, char** board, int row, int col) {
    // Update unsafe points above and below the mine
    int i, j;
    for (i = row - 1; i <= row + 1; i++) {
        for (j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < size && j >= 0 && j < size && (i == row || j == col)){
                if(!(i == row && j == col))
                    if(board[i][j] != 'o')
                        board[i][j] = 'x';
                }
        }
    }
}

void playGame(int size, char** board, int startRow, int startCol, int endRow, int endCol) {
    int currentRow = startRow;
    int currentCol = startCol;
    int i, j;
    char** roadTaken;

    roadTaken = (char**)malloc(size * sizeof(char*));
    for (i = 0; i < size; i++) {
        roadTaken[i] = (char*)malloc(size * sizeof(char));
    }

    for(i = 0; i < size; ++i)
        for(j = 0; j < size; ++j)
            roadTaken[i][j] = board[i][j];
    roadTaken[0][0] = '$';

    while (1) {
        printBoard(size, board, currentRow, currentCol, false);

        // Get user input
        char move = getch();

        // Check the arrow key pressed
        switch (move) {
            case 72:
                if (currentRow > 0) {
                    currentRow--;
                    roadTaken[currentRow][currentCol] = '$';
                }
                break;
            case 80:
                if (currentRow < size - 1) {
                    currentRow++; 
                    roadTaken[currentRow][currentCol] = '$';
                }
                break;
            case 75:
                if (currentCol > 0) {
                    currentCol--;
                    roadTaken[currentRow][currentCol] = '$';
                }
                break;
            case 77:
                if (currentCol < size - 1) {
                    currentCol++;
                    roadTaken[currentRow][currentCol] = '$';
                }
                break;
            case 'r':
                // Generate a new board with mines placed differently
                initializeBoard(size, board, &startRow, &startCol, &endRow, &endCol);
                currentRow = startRow;
                currentCol = startCol;
                for(i = 0; i < size; ++i)
                    for(j = 0; j < size; ++j)
                        roadTaken[i][j] = board[i][j];
                roadTaken[0][0] = '$';
                break;
            default:
                break;
        }

        // Check if the player reached the finish point
        if (currentRow == endRow && currentCol == endCol) {
            roadTaken[size - 1][size - 1] = '$';
            printBoard(size, roadTaken, currentRow, currentCol, true);
            printf("Congratulations! You reached the finish point!\n");
            // Free allocated memory
            for (i = 0; i < size; i++) {
                free(roadTaken[i]);
            }
            free(roadTaken);
            sleep(5);
            break;
        }

        // Check if the player stepped on a mine
        if (board[currentRow][currentCol] == 'o') {
            printf("Sorry, you stepped on a mine! Game over.\n");
            sleep(5);
            break;
        }

        // Check if the player stepped on an unsafe point
        if (board[currentRow][currentCol] == 'x') {
            printf("Boom! You stepped on an unsafe point. Sent back to start.\n");
            sleep(1);
            currentRow = 0;
            currentCol = 0;
        }
    }
}

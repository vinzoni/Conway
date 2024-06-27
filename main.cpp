#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

// prima e uotima riga, prima e utlima colonna, non fanno parte dell'area disponibile per
// collocarvi celle. L'area effettiva è data da ROWS-2 x COLUMNS - 2
const int ROWS = 20;
const int COLUMNS = 20;

bool grid[ROWS][COLUMNS];
bool copy_grid[ROWS][COLUMNS];

bool empty_grid(bool grid[ROWS][COLUMNS]) {
    for (int i=0; i<ROWS; i++)
        for (int j=0; j<COLUMNS; j++)
            if (grid[i][j] == 1)
                return false;
    cout << "aaaa";
    return true;
}

bool stable_pattern(bool grid1[ROWS][COLUMNS], bool grid2[ROWS][COLUMNS]) {
    for (int i=0; i<ROWS; i++)
        for (int j=0; j<COLUMNS; j++)
            if (grid1[i][j] != grid2[i][j])
                return false;

    cout << "bbbb";
    return true;
}

void grid_setup() {
    for (int i=0; i<ROWS; i++)
        for (int j=0; j<COLUMNS; j++)
            grid[i][j] = 0;

    // block
//    grid[10][10] = 1;
//    grid[11][10] = 1;
//    grid[10][11] = 1;
//    grid[11][11] = 1;

    // blinker
//    grid[10][10] = 1;
//    grid[10][11] = 1;
//    grid[10][12] = 1;

    // toad
//    grid[10][10] = 1;
//    grid[10][11] = 1;
//    grid[10][12] = 1;
//    grid[9][9] = 1;
//    grid[9][10] = 1;
//    grid[9][11] = 1;

    // wrong glider
    grid[10][10] = 1;
    grid[11][11] = 1;
    grid[11][12] = 1;
    grid[11][10] = 1;
    grid[12][11] = 1;

    // glider
//    grid[2][2] = 1;
//    grid[3][3] = 1;
//    grid[3][4] = 1;
//    grid[4][2] = 1;
//    grid[4][3] = 1;

    // ???
//    grid[10][10] = 1;
//    grid[10][11] = 1;
//    grid[10][12] = 1;
//    grid[10][13] = 1;

}

void grid_draw() {
    for (int i=1; i<COLUMNS-1; i++)
        for (int j=1; j<ROWS-1; j++)
            if (grid[i][j] == 1) {
                move(i-1, (j-1)*2);
                printw("* ");
                }
}

int neighbours(int x, int y) {

    int count = 0;
    for (int i=x-1; i<x+2; i++)
        for (int j=y-1; j<y+2; j++) {
            if (i==x && j==y)
                continue;
            if (grid[i][j] == 1)
                count++;
        }
    return count;
}

void grid_generate() {

    for (int i=0; i<COLUMNS; i++)
        for (int j=0; j<ROWS; j++)
            copy_grid[i][j] = 0;

    for (int i=1; i<COLUMNS-1; i++)
        for (int j=1; j<ROWS-1; j++) {
            if (grid[i][j] == 0 && neighbours(i, j) == 3)
                copy_grid[i][j] = 1;
            else if (grid[i][j] == 1 && neighbours(i, j) < 2) {
                copy_grid[i][j] = 0;
            }
            else if (grid[i][j] == 1 && (neighbours(i, j) == 2 || neighbours(i, j) == 3))
                copy_grid[i][j] = 1;
            else if (grid[i][j] == 1 && neighbours(i, j) > 3)
                copy_grid[i][j] = 0;
            else
                copy_grid[i][j] = 0;
        }
}

void grid_update() {

    for (int i=0; i<COLUMNS; i++)
        for (int j=0; j<ROWS; j++)
            grid[i][j] = copy_grid[i][j];

}


int main()
{
    // initializes the screen
    // sets up memory and clears the screen
    initscr();

    grid_setup();

    curs_set(0); // invisible cursor
    int generations = 0;


    while (true)
    {
        clear();
        grid_draw();

        move(23, 1);
        printw("generation: %d", generations);
        move(23, 50);
        printw("press ctrl-c to stop. ");

        refresh();
        sleep(1);

        generations ++;
        grid_generate();

        if (empty_grid(grid))
            break;

        if (stable_pattern(grid, copy_grid))
            break;

        grid_update();
    }
    curs_set(1); // visible cursor

    // waits for user input, returns int value of that key
    move(23, 50);
    printw("press any key to exit.");
    getch();


    // deallocates memory and ends ncurses
    endwin();

    return 0;
}

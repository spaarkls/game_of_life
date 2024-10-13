#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25

void print_grid(int grid[HEIGHT][WIDTH]);
int count_neighbors(int grid[HEIGHT][WIDTH], int x, int y);
void step(int grid[HEIGHT][WIDTH], int new_grid[HEIGHT][WIDTH]);
int *read_file(int *error);
int yCoord(int y) { return ((y + HEIGHT) % HEIGHT); }
int xCoord(int x) { return ((x + WIDTH) % WIDTH); }
int stability(int grid[HEIGHT][WIDTH], int new_gird[HEIGHT][WIDTH]);
int is_dead(int grid[HEIGHT][WIDTH], int new_grid[HEIGHT][WIDTH]);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("You forgote to enter the speed..\nCorrect input is ./a.out speed < filename.txt\n");
        return 0;
    }
    int error = 1;
    int grid[HEIGHT][WIDTH] = {0};
    int new_grid[HEIGHT][WIDTH] = {0};
    int *input = read_file(&error);
    printf("%d", error);
    if (!error) {
        free(input);
        return 1;
    }
    for (int i = 0; i < 100; i += 2) {
        if (input[i] == -1) break;
        grid[input[i]][input[i + 1]] = 1;
    }
    free(input);
    rewind(stdin);
    float speed_game;
    speed_game = atof(argv[1]) * 1000;
    while (1) {
        print_grid(grid);
        step(grid, new_grid);
        if (!stability(grid, new_grid) || is_dead(grid, new_grid)) {
            printf("Life Over...\n");
            break;
        }
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                grid[i][j] = new_grid[i][j];
            }
        }
        printf("\n");
        napms(speed_game);
        system("clear");
    }
    return 0;
}

int *read_file(int *error) {
    int *arr = malloc(100 * sizeof(int));
    char line[100];
    int row = 0;
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (sscanf(line, "%d %d", &arr[row], &arr[row + 1]) != 2) {
            fprintf(stderr, "Error of read...\n");
            *error = 0;
            break;
        }
        row += 2;
    }
    arr[row] = -1;
    return arr;
}

void print_grid(int grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf(grid[i][j] ? "O " : ". ");
        }
        printf("\n");
    }
}

int count_neighbors(int grid[HEIGHT][WIDTH], int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            count += grid[yCoord(x + i)][xCoord(y + j)];
        }
    }
    return count;
}

void step(int grid[HEIGHT][WIDTH], int new_grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int neighbors = count_neighbors(grid, i, j);
            if (grid[i][j] == 1) {
                if (neighbors < 2 || neighbors > 3) {
                    new_grid[i][j] = 0;
                } else {
                    new_grid[i][j] = 1;
                }
            } else {
                if (neighbors == 3) {
                    new_grid[i][j] = 1;
                } else {
                    new_grid[i][j] = 0;
                }
            }
        }
    }
}

int stability(int grid[HEIGHT][WIDTH], int new_grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] != new_grid[i][j]) {
                return 1;
            }
        }
    }
    return 0;
}

int is_dead(int grid[HEIGHT][WIDTH], int new_grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] != 0 && new_grid[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}


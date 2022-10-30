#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define Height 25
#define Width 80
#define SIZE 5

int worlds[SIZE][Height][Width];
void initWorld(int world[Height][Width]);
void printWorld(int world[Height][Width]);
int getLiveCount(int world[Height][Width]);
void readPointNeighbors(int nb[][2], int x, int y);
int countLiveNeighbors(int world[Height][Width], int x, int y);
void nextGenerations(int world[Height][Width], int prev_world[Height][Width]);
void copyWorld(int src[Height][Width], int dect[Height][Width]);
int cmpWorlds(int world[Height][Width], int prev_world[Height][Width]);
int menu();

int main() {
    int world[Height][Width];
    int prev_world[Height][Width];
    int live_points = -1;
    int opt, count = 0;
    int cycledCount = 0;
    int speed = 10000;
    initWorld(world);
    speed = menu();
    do {
        printWorld(world);
        copyWorld(world, prev_world);
        copyWorld(world, worlds[cycledCount]);
        nextGenerations(world, prev_world);
        opt = cmpWorlds(world, prev_world);
        for (int i=0; i < cycledCount; i++)
            if (cmpWorlds(world, worlds[i]) == 1) {
                opt = 1;
                printf("Cycled structure\n");
                break;
            }
        live_points = getLiveCount(world);
        printf("Number of living cells: %d\n", live_points);
        if (cycledCount < SIZE) {
            cycledCount++;
            } else {
            cycledCount = 0;
        }
        count++;
        printf("Number of iterations: %d\n", count);
        usleep(speed);
    } while (live_points != 0 && opt < 0);
    printWorld(world);
    printf("Number of living cells: %d\n", live_points);
    printf("Number of iterations: %d\n", count);
    if (live_points == 0) {
        printf("All cells are dead\n");
    }
    if (opt == 1) {
        printf("Stable configuration has been achieved\n");
    }
    return 0;
}
int menu() {
    int speed;
    printf("Enter filename with start configuration:\n");
    freopen("/dev/tty", "r", stdin);
    printf("Enter game speed in seconds:\n");
    printf("|1| - 0.05 Seconds\n");
    printf("|2| - 0.5 Seconds\n");
    printf("|3| - 1 Second\n");
    printf("|4| - 5 Seconds\n");
    if (scanf("%d", &speed) == 1) {
        if (speed == 1) {
            speed = 50000;
        } else if (speed == 2) {
            speed = 500000;
        } else if (speed == 3) {
            speed = 1000000;
        } else if (speed == 4) {
            speed = 5000000;
        } else if (speed > 4) {
            printf("Error input!");
            exit(1);
        }
    } else {
        printf("Error input!");
        exit(1);
    }
    return speed;
}
void initWorld(int world[Height][Width]) {
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            scanf("%d", &world[i][j]);
        }
    }
}
void printWorld(int world[Height][Width]) {
    system("clear");
    for (int i = 0; i < Width; i++) {
        printf("-");
    }
    for (int i = 0; i < Height; i++) {
        printf("\n");
        for (int j = 0; j < Width; j++)
              if (world[i][j] == 1) {
                printf("#");
              } else { printf(" "); }
    }
    printf("\n");
    for (int i = 0; i < Width; i++) { printf("-"); }
    printf("\n");
}
int getLiveCount(int world[Height][Width]) {
    int count = 0;
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            if (world[i][j] == 1)  {
                count++;
            }
        }
    }
    return count;
}
void readPointNeighbors(int nb[][2], int x, int y) {
    int k = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i == x && j == y)
                continue;
            if (i < 0) {
                nb[k][0] = i + Height;
            } else if (i > Height - 1) { nb[k][0] = i - Height;
            } else { nb[k][0] = i; }
            if (j < 0) { nb[k][1] = j + Width;
            } else if (j > Width - 1) {
                nb[k][1] = j - Width;
            } else {
                nb[k][1] = j;
            }
            k++;
        }
    }
}
int countLiveNeighbors(int world[Height][Width], int x, int y) {
    int count = 0;
    int nb[8][2];
    int _x, _y;
    readPointNeighbors(nb, x, y);
    for (int i = 0; i < 8; i++) {
        _x = nb[i][0];
        _y = nb[i][1];
        if (_x < 0 || _y < 0) { continue; }
        if (_x >= Height || _y >= Width) { continue; }
        if (world[_x][_y] == 1) { count++; }
    }
    return count;
}
void nextGenerations(int world[Height][Width], int prev_world[Height][Width]) {
    int live_nb;
    int p;
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            p = prev_world[i][j];
            live_nb = countLiveNeighbors(prev_world, i, j);
            if (p == 0) {
                if (live_nb == 3) { world[i][j] = 1; }
            } else {
                if (live_nb < 2 || live_nb > 3) { world[i][j] = 0; }
            }
        }
    }
}
void copyWorld(int src[Height][Width], int dect[Height][Width]) {
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
             dect[i][j] = src[i][j];
        }
    }
}
int cmpWorlds(int world[Height][Width], int prev_world[Height][Width]) {
    int result = 1;
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
             if (world[i][j] != prev_world[i][j]) {
                 result = -1;
             }
        }
    }
    return result;
}

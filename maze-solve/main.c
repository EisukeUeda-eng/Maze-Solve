#include <stdio.h>
#include <stdlib.h>
#define MAX 100000

struct Roadpoint{
    int x;
    int y;
}X[256];

typedef struct stack{
    struct Roadpoint X[MAX];
    int top;
}stack;

void stackinit(stack *p);
void push(stack *p, int v, int w);
void pop(stack *p);
void show(stack *p, char *maze, int H, int W);
void printroad(int H, int W, int *maze, char *maze1);
void onemove(int x, int y, int H, int W, int *maze, char *maze1, stack *p);

int main(void) {
    int i, j, sx, sy, H, W;
    char c;
    stack Road;
    FILE *fp = fopen("maze1.txt", "r");
    if(fopen == NULL){
        return 1;
    }
    
    fscanf(fp, "%d %d", &H, &W);
    
    Road.top = -1;
    for(i = 0; i < MAX; i++){
        Road.X[i].x = 0;
        Road.X[i].y = 0;
    }
    
    int maze[H][W];
    char mazecharacter[H][W];
    
    i = 0;
    j = 0;
    
    while((c = fgetc(fp)) != EOF){
        if(c == 'S'){ // If the character is 'S' (start point)
            maze[i][j] = 2;
            j++;
        }
        else if(c == 'G'){ // If the character is 'G' (goal point)
            maze[i][j] = 3;
            j++;
        }
        else if(c == '.'){ // If the character is '.' (path)
            maze[i][j] = 0;
            j++;
        }
        else if(c == '#'){ // If the character is '#' (wall)
            maze[i][j] = 1;
            j++;
        }
        
        if(j == W){
            j = 0;
            i++;
        }
    }
    
    for(i = 0; i < W; i++){
        for(j = 0; j < H; j++){
            if(maze[j][i] == 2){  // Finding the start point 'S'
                sx = i;
                sy = j;
            }
        }
    }
    
    printf("Maze Answer\n");
    printf("\n");
    
    onemove(sx, sy, H, W, maze, mazecharacter, &Road);
    
    printf("\n");
    
    printf("Maze Answer(coordinate display)\n");
    printf("\n");
    
    show(&Road, mazecharacter, H, W);
    
    fclose(fp);
    
    return 0;
}


void printroad(int H, int W, int *maze, char *maze1) {
    int i, j;

    // Open the file for writing
    FILE *fp = fopen("maze_ans.txt", "w");
    if (fp == NULL) {
        printf("FILE error\n");
        return;
    }

    for (j = 0; j < H; j++) {
        for (i = 0; i < W; i++) {
            if (maze[j * H + i] == 0) { // Path
                maze1[j * H + i] = ' ';
                printf(" ");       // Print to console
                fprintf(fp, " ");  // Write to file
            }
            if (maze[j * H + i] == 1) { // Wall
                maze1[j * H + i] = '#';
                printf("#");
                fprintf(fp, "#");
            }
            if (maze[j * H + i] == 2) { // Start point
                maze1[j * H + i] = 'S';
                printf("S");
                fprintf(fp, "S");
            }
            if (maze[j * H + i] == 3) { // Goal point
                maze1[j * H + i] = 'G';
                printf("G");
                fprintf(fp, "G");
            }
            if (maze[j * H + i] == 4) { // Passed path
                maze1[j * H + i] = 'o';
                printf("o");
                fprintf(fp, "o");
            }
            if (maze[j * H + i] == 5) { // Dead-end
                maze1[j * H + i] = 'X';
                printf(" ");
                fprintf(fp, " ");
            }
        }
        printf("\n");       // Print newline to console
        fprintf(fp, "\n");  // Write newline to file
    }

    // Close the file
    fclose(fp);
}


void onemove(int x, int y, int H, int W, int *maze, char *maze1, stack *p){
    int nx, ny;
    
    push(p, x, y);
    
    if(x < 0 || x >= W || y < 0 || y >= H || maze[y * H + x] == 1){  // Out of bounds or a wall
        return;
    }
    
    if(maze[y * H + x] == 3){  // Reached the goal
        printroad(H, W, maze , maze1);
        return;
    }
    
    maze[y * W + x] = 4;  // Mark the current path as passed
    
    // Move one step up
    nx = x;
    ny = y - 1;
    if(ny >= 0){
        if(maze[ny * H + nx] != 1){
            if(maze[ny * H + nx] != 4){
                onemove(nx, ny, H, W, maze, maze1, p);
            }
        }
    }
    
    // Move one step down
    nx = x;
    ny = y + 1;
    if(ny < H){
        if(maze[ny * H + nx] != 1){
            if(maze[ny * H + nx] != 4){
                onemove(nx, ny, H, W, maze, maze1, p);
            }
        }
    }
    
    // Move one step right
    nx = x + 1;
    ny = y;
    if(nx < W){
        if(maze[ny * H + nx] != 1){
            if(maze[ny * H + nx] != 4){
                onemove(nx, ny, H, W, maze, maze1, p);
            }
        }
    }
    
    // Move one step left
    nx = x - 1;
    ny = y;
    if(nx >= 0){
        if(maze[ny * H + nx] != 1){
            if(maze[ny * H + nx] != 4){
                onemove(nx, ny, H, W, maze, maze1, p);
            }
        }
    }
    
    maze[y * H + x] = 5;  // Mark as dead-end
}

void push(stack *p, int v, int w){
    if(p->top >= MAX){  // Check stack overflow
        printf("stack error\n");
    }
    else{
        p->top++;
        p->X[p->top].x = v;
        p->X[p->top].y = w;
    }
}

void pop(stack *p){
    if(p->top <= -1){  // Check stack underflow
        printf("stack error\n");
    }
    else{
        p->X[p->top].x = 0;
        p->X[p->top].y = 0;
        p->top--;
    }
}

void show(stack *p, char *maze, int H, int W){
    int i;
    
    FILE *fp = fopen("maze_result.txt", "w");
    if(fopen == NULL){
        printf("FILE error\n");
    }
    
    for(i = 0; i <= p->top; i++){
        if(maze[p->X[i].y * H + p->X[i].x] != 'X'){  // If not a dead-end
            printf("%d %d\n", p->X[i].x, p->X[i].y);
            fprintf(fp, "%d %d\n", p->X[i].x, p->X[i].y);
            if(maze[p->X[i].y * H + p->X[i].x] == 'G'){  // Stop if the goal is reached
                break;
            }
        }
    }
    
    fclose(fp);
}

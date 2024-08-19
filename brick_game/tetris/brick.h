/// \file
#ifndef BRICK_H
#define BRICK_H

#include <stdbool.h>

#define ROWS 21
#define COLUMS 12
#undef	OK
#define OK 0
#define FAIL_TO_ALLOCATE 1
#define QUIT -2
#define FAIL_TO_MOVE 2
#define GAME_DIFICULTY 10

#define COLOR_ORANGE 8
#define COLOR_PURPLE 9 
#define COLOR_DARKBLUE 10
#define COLOR_DARK_BLACK 12
#define COLOR_LIGHT_YELLOW 13


#define WALL 7
#define SPACE 8

typedef enum {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Down,
    Action,
    Lose,
} UserAction_t;


typedef struct {
    int y;
    int x;
    int numberObject;
    int nextNumberObject;
    int countRotation;
} DynamicObject;

typedef struct{
    int timer;
    int command;
    int start;
} OtherVariables;


typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

GameInfo_t *updateCurrentState();
DynamicObject *getVariables();
OtherVariables *getOtherVar();

int checkBorders(int (*objects)[5][5],DynamicObject *var, int add_X,int add_Y);
int moveObject(int (*objects)[5][5],DynamicObject *var, int add_X,int add_Y);
int allocateMatrixMemory(int ***matrix);
int dropField(GameInfo_t *game);
int (*listObjects(void))[5][5];

void processStartLose(GameInfo_t *game,DynamicObject *var,OtherVariables *othervar,int (*objects)[5][5]);
void processDown(int (*objects)[5][5],DynamicObject *var,GameInfo_t *game,OtherVariables *othervar);
void saveObject(int (*objects)[5][5],DynamicObject *var,GameInfo_t *game);
void firstInit(GameInfo_t *game,DynamicObject *var,int (*objects)[5][5]);
void gameLoop(int (*objects)[5][5],DynamicObject *var,GameInfo_t *game);
void printObject(int (*objects)[5][5],int x, int y,int numberObject);
void controls(OtherVariables *othervar,GameInfo_t *game);
void userInput(UserAction_t action, bool hold);
void freeMatrixMemory(int **matrix,int i);
void countThePoints(GameInfo_t *game);
void changeObject(DynamicObject *var);
void printField(GameInfo_t *game);
void fillField(GameInfo_t *game);
void printTetrisIntro();
void printGameKeys();
void initCollors();
void clearWindow();
void printFrame();

void saveHighScore(int *highScore);
void readHighScore(int *highScore);

#endif
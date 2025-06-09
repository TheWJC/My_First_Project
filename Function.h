#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRID_SIZE 10
#define MAX_SNAKE_LENGTH 100
#define LIGHTGRAY {200,200,200,255}
#define RED {255,0,0,255}
#define GREEN {0,255,0,255}
#define BLUE {0,0,255,255}

/* 定义一个结构体来存储我们的坐标 */
typedef struct {
    int x, y;
} Point;

//定义一个匿名枚举常量
typedef enum
{
	UP,
	DOWN,
	LEFT,
	RIGHT
 }Direction;
 
/* 定义一个结构体来存储我们蛇的坐标，蛇身的长度 */
typedef struct {
    Point positions[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
} Snake;
//enum bool
//{
//	True,
//	False
//};错误定义，会与 stdbool库中的bool冲突
//正确定义
typedef enum
{
	FALSE,
	TRUE
}Bool;
/* 定义一个结构体来存储我们的食物坐标 */
typedef struct {
    Point position;
    int score;
    bool isActive;
} Food;

extern bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
extern void initSnake(Snake* snake);
extern void initFood(Food* food, Snake* snake);
extern void handleInput(Snake* snake);
extern void moveSnake(Snake* snake);
extern bool checkCollision(Snake* snake);
extern void render(SDL_Renderer* renderer, Snake* snake, Food* food, int score);
extern void resetGame(Snake* snake);



#endif

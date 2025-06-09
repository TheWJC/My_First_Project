#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#undef SDL_main
#include "Function.h"                                                                                     
 



int WinMain(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    // 初始化SDL
    if (!initSDL(&window, &renderer)) {
        return -1;
    }
    
    // 初始化随机数种子
    srand((unsigned int)time(NULL));   
    // 加载音乐
    Mix_Music* music = Mix_LoadMUS("C:\\1.mp3");
    if (music) {
        Mix_PlayMusic(music, -1); // -1表示循环播放
    } else {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    
    // 初始化游戏状态
    Snake snake;
    Food food;
    int score = 0;
    bool quit = false;
    
    initSnake(&snake);
    initFood(&food, &snake);
    
    // 游戏主循环
    while (!quit) {
        // 处理输入
        handleInput(&snake);
        
        // 移动蛇
        moveSnake(&snake);
        
        // 检查是否吃到食物
        if (snake.positions[0].x == food.position.x && 
            snake.positions[0].y == food.position.y && food.isActive) {
            snake.length++;
            score += food.score;
            food.isActive = false;
            initFood(&food, &snake);
        }
        
        // 检查碰撞
        if (checkCollision(&snake)) {
            if (music) Mix_HaltMusic();
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", 
                                   "You died! Press OK to quit.", window);
            break;
        }
        
        // 如果食物被吃掉，生成新食物
        if (!food.isActive) {
            initFood(&food, &snake);
        }
        
        // 渲染
        render(renderer, &snake, &food, score);
        
        // 延迟
        SDL_Delay(100);
    }
    
    // 清理资源
    if (music) Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

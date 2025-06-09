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
    
    // ��ʼ��SDL
    if (!initSDL(&window, &renderer)) {
        return -1;
    }
    
    // ��ʼ�����������
    srand((unsigned int)time(NULL));   
    // ��������
    Mix_Music* music = Mix_LoadMUS("C:\\1.mp3");
    if (music) {
        Mix_PlayMusic(music, -1); // -1��ʾѭ������
    } else {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    
    // ��ʼ����Ϸ״̬
    Snake snake;
    Food food;
    int score = 0;
    bool quit = false;
    
    initSnake(&snake);
    initFood(&food, &snake);
    
    // ��Ϸ��ѭ��
    while (!quit) {
        // ��������
        handleInput(&snake);
        
        // �ƶ���
        moveSnake(&snake);
        
        // ����Ƿ�Ե�ʳ��
        if (snake.positions[0].x == food.position.x && 
            snake.positions[0].y == food.position.y && food.isActive) {
            snake.length++;
            score += food.score;
            food.isActive = false;
            initFood(&food, &snake);
        }
        
        // �����ײ
        if (checkCollision(&snake)) {
            if (music) Mix_HaltMusic();
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", 
                                   "You died! Press OK to quit.", window);
            break;
        }
        
        // ���ʳ�ﱻ�Ե���������ʳ��
        if (!food.isActive) {
            initFood(&food, &snake);
        }
        
        // ��Ⱦ
        render(renderer, &snake, &food, score);
        
        // �ӳ�
        SDL_Delay(100);
    }
    
    // ������Դ
    if (music) Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

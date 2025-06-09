#include "Function.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#undef SDL_main

int i;
// ��ʼ��SDL
bool initSDL(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }
 
    // ��ʼ��SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }
 
    *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
 
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
 
    return true;
}
 
// ��ʼ����
void initSnake(Snake* snake) {
    snake->positions[0].x = 20;
    snake->positions[0].y = 0;
    snake->positions[1].x = 10;
    snake->positions[1].y = 0;
    snake->positions[2].x = 0;
    snake->positions[2].y = 0;
    snake->length = 3;
    snake->direction = RIGHT;
}
void resetGame(Snake* snake)
{
	initSnake(snake);
}
// ��ʼ��ʳ��
void initFood(Food* food, Snake* snake) {
    bool onSnake;
    do {
        onSnake = false;
        food->position.x = (rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
        food->position.y = (rand() % (SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE;
        
        // ���ʳ���Ƿ�������������
        for (i = 0; i < snake->length; i++) {
            if (food->position.x == snake->positions[i].x && 
                food->position.y == snake->positions[i].y) {
                onSnake = true;
                break;
            }
        }
    } while (onSnake);
    
    food->isActive = true;
    food->score = 10;
}
 
// ��������
void handleInput(Snake* snake) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            exit(0);
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_w:
                case SDLK_UP:
                    if (snake->direction != DOWN) {
                        snake->direction = UP;
                    }
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    if (snake->direction !=UP) {
                        snake->direction = DOWN;
                    }
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    if (snake->direction != RIGHT) {
                        snake->direction = LEFT;
                    }
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    if (snake->direction != LEFT) {
                        snake->direction = RIGHT;
                    }
                    break;
                case SDLK_ESCAPE:
                    exit(0);
                    break;
                case SDLK_r:
					resetGame(snake);
					break;//����R�����¿�ʼ��Ϸ 
            }
        }
    }
}
 
// �ƶ���
void moveSnake(Snake* snake) {
    // ������ͷλ��
    Point prevHead = snake->positions[0];
    
    // �ƶ�����
    for (i = snake->length - 1; i > 0; i--) {
        snake->positions[i] = snake->positions[i - 1];
    }
    
    // �ƶ���ͷ
    switch (snake->direction) {
        case UP:
            snake->positions[0].y -= GRID_SIZE;
            break;
        case DOWN:
            snake->positions[0].y += GRID_SIZE;
            break;
        case LEFT:
            snake->positions[0].x -= GRID_SIZE;
            break;
        case RIGHT:
            snake->positions[0].x += GRID_SIZE;
            break;
    }
    
    // ����Ƿ�Ե�ʳ��
    // ע�⣺������Ӧ������Ⱦѭ���н��У�����ֻ���ƶ��߼�
}
 
// �����ײ
bool checkCollision(Snake* snake) {
    // ���ǽ����ײ
    if (snake->positions[0].x < 0 || snake->positions[0].x >= SCREEN_WIDTH ||
        snake->positions[0].y < 0 || snake->positions[0].y >= SCREEN_HEIGHT) {
        return true;
    }
    
    // ���������ײ
    for (i = 1; i < snake->length; i++) {
        if (snake->positions[0].x == snake->positions[i].x && 
            snake->positions[0].y == snake->positions[i].y) {
            return true;
        }
    }
    
    return false;
}
 
// ��Ⱦ��Ϸ
void render(SDL_Renderer* renderer, Snake* snake, Food* food, int score) {
    // ����
    SDL_SetRenderDrawColor(renderer, 128, 138, 135, 255);
    SDL_RenderClear(renderer);
    
    // ������
    for (i = 0; i < snake->length; i++) {
        SDL_Rect rect = {
            snake->positions[i].x,
            snake->positions[i].y,
            GRID_SIZE,
            GRID_SIZE
        };
        
        // ��ͷ�ò�ͬ��ɫ
        if (i == 0) {
            SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, 255);
        }
        
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // �߿��ɫ
        SDL_RenderDrawRect(renderer, &rect);
    }
    
    // ����ʳ��
    if (food->isActive) {
        SDL_Rect rect = {
            food->position.x,
            food->position.y,
            GRID_SIZE,
            GRID_SIZE
        };
        SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // �߿��ɫ
        SDL_RenderDrawRect(renderer, &rect);
    }
    
    // ��ʾ����
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    SDL_Color color = LIGHTGRAY;
    SDL_Surface* surface = TTF_RenderText_Solid(NULL, scoreText, color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect textRect = {10, 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    
    // ������Ļ
    SDL_RenderPresent(renderer);
}


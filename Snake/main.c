#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // Pour utiliser rand() et realloc()
#include <time.h>

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Block;

Block* blocks = NULL;
int blockCount = 0;

static void DrawSquare(SDL_Renderer* renderer, int x, int y);
void spawnBlock(Block* blocks, int* blockCount);

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Carre Bougable", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int x = 400, y = 300;
    bool quit = false;
    SDL_Event event;
    Uint32 lastSpawnTime = SDL_GetTicks();
    Uint32 spawnInterval = 10000;

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastSpawnTime > spawnInterval) {
            spawnBlock(blocks, &blockCount);
            lastSpawnTime = currentTime;
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP: y -= 10; break;
                case SDLK_DOWN: y += 10; break;
                case SDLK_LEFT: x -= 10; break;
                case SDLK_RIGHT: x += 10; break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        DrawSquare(renderer, x, y);

        for (int i = 0; i < blockCount; i++) {
            SDL_Rect blockRect = { blocks[i].x, blocks[i].y, blocks[i].width, blocks[i].height };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &blockRect);
        }

        SDL_RenderPresent(renderer);
    }

    free(blocks);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

static void DrawSquare(SDL_Renderer* renderer, int x, int y) {
    SDL_Rect square = { x, y, 50, 50 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &square);
}

void spawnBlock(Block* blocks, int* blockCount) {
    Block newBlock;
    newBlock.x = rand() % 800;
    newBlock.y = rand() % 600;
    newBlock.width = 5;
    newBlock.height = 5;

    // Réallocation du tableau pour ajouter le nouveau bloc
    blocks = realloc(blocks, (*blockCount + 1) * sizeof(Block));
    blocks[*blockCount] = newBlock;
    (*blockCount)++;
}

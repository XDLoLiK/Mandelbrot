/**
 * @defgroup   MAIN main
 *
 * @brief      This file implements main.
 *
 * @author     Stanislav
 * @date       2022
 */

#include "mandelbrot.h"

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window  *mainWindow = SDL_CreateWindow("Mondelbrot Set",SDL_WINDOWPOS_CENTERED, 
                                               SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    assert(mainWindow != nullptr);
    SDL_SetWindowBordered(mainWindow, SDL_FALSE);
    SDL_Renderer *windowRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assert(windowRenderer != nullptr);
    SDL_Surface *windowSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    assert(windowSurface != nullptr);
    SDL_RenderClear(windowRenderer);

    int retValue = 0;

    if (argc < 2) {
        retValue = DrawMondelbrotAVX(windowSurface, windowRenderer);
    } else {
        if (strncmp("avx", argv[1], 3) == 0) {
            retValue = DrawMondelbrotAVX(windowSurface, windowRenderer);
        } else if (strncmp("sse", argv[1], 3) == 0) {
            retValue = DrawMondelbrotSSE(windowSurface, windowRenderer);
        } else {
            retValue = DrawMondelbrotAVX(windowSurface, windowRenderer);
        }
    }

    SDL_FreeSurface(windowSurface);
    SDL_DestroyRenderer(windowRenderer);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return retValue;
}

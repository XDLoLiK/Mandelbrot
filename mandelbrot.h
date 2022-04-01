/**
 * @defgroup   MANDELBROT mandelbrot
 *
 * @brief      This file implements mandelbrot.
 *
 * @author     Stanislav
 * @date       2022
 */

#ifndef MONDELBROT_H
#define MONDELBROT_H

#include "SDL2/SDL.h"

#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <inttypes.h>
#include <emmintrin.h>
#include <x86intrin.h>

typedef int32_t       RGBA;
typedef unsigned char BYTE;

typedef struct RGBAQUAD {
    BYTE red   = 0;
    BYTE green = 0;
    BYTE blue  = 0;
    BYTE alpha = 0;
} RGBAQUAD;

// SCREEN PARAMS
const int SCREEN_HEIGHT  = 600;
const int SCREEN_WIDTH   = 800;

// MONDELBROT CONSTS
const int   MAX_ITERATIONS = 256;
const float BORDER_RADIUS  = 100.f;

// SSE CONSTS
const __m128 BORDER_RADIUS_SSE = _mm_set_ps1(100.f);

// AVX CONSTS
const __m256 BORDER_RADIUS_AVX = _mm256_set1_ps(100.f);

/**
 * @brief      Draws a mondelbrot sse
 *
 * @param      WindowSurface   The window surface
 * @param      windowRenderer  The window renderer
 *
 * @return     Error code
 */
int DrawMondelbrotSSE(SDL_Surface *WindowSurface, SDL_Renderer *windowRenderer);

/**
 * @brief      Draws a mondelbrot avx
 *
 * @param      WindowSurface   The window surface
 * @param      windowRenderer  The window renderer
 *
 * @return     Error code
 */
int DrawMondelbrotAVX(SDL_Surface *WindowSurface, SDL_Renderer *windowRenderer);

/**
 * @brief      Calculates the mondelbrot sse.
 *
 * @param      pixelsArray  The pixels array
 * @param[in]  xOrigin      The x origin
 * @param[in]  yOrigin      The y origin
 * @param[in]  xDelta       The delta
 *
 * @return     The mondelbrot sse.
 */
int calculateMondelbrotSSE(RGBAQUAD *pixelsArray, float xOrigin, float yOrigin, float xDelta);

/**
 * @brief      Calculates the mondelbrot avx.
 *
 * @param      pixelsArray  The pixels array
 * @param[in]  xOrigin      The x origin
 * @param[in]  yOrigin      The y origin
 * @param[in]  xDelta       The delta
 *
 * @return     The mondelbrot avx.
 */
int calculateMondelbrotAVX(RGBAQUAD *pixelsArray, float xOrigin, float yOrigin, float xDelta);

/**
 * @brief      Updates screen
 *
 * @param      windowRenderer  The window renderer
 * @param      windowSurface   The window surface
 *
 * @return     0 :D
 */
int UpdateScreen(SDL_Renderer *windowRenderer, SDL_Surface *windowSurface);

#endif // MONDELBROT_H

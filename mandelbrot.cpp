/**
 * @defgroup   MANDELBROT mandelbrot
 *
 * @brief      This file implements mandelbrot.
 *
 * @author     Stanislav
 * @date       2022
 */

#include "mandelbrot.h"

int DrawMondelbrotSSE(SDL_Surface *windowSurface, SDL_Renderer *windowRenderer)
{
    float xCenter = -1.325f,
          yCenter = 0.f,
          xDelta  = 1.f / 800.f,
          yDelta  = 1.f / 800.f,
          scale   = 1.f;
    
    SDL_Event event  = {};
    time_t timeSpend = 0;
    size_t frames    = 0;

    for (bool running = true; running; SDL_PollEvent(&event), frames++) {
        if (event.type == SDL_QUIT ||
            event.key.keysym.sym == SDLK_ESCAPE) { running = false; }

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP)    { yCenter -= yDelta * 10.f; SDL_PollEvent(&event); }
            if (event.key.keysym.sym == SDLK_DOWN)  { yCenter += yDelta * 10.f; SDL_PollEvent(&event); }
            if (event.key.keysym.sym == SDLK_LEFT)  { xCenter -= xDelta * 10.f; SDL_PollEvent(&event); }
            if (event.key.keysym.sym == SDLK_RIGHT) { xCenter += xDelta * 10.f; SDL_PollEvent(&event); }

            if (event.key.keysym.sym == SDLK_q) { scale += xDelta * 10.f; SDL_PollEvent(&event); }
            if (event.key.keysym.sym == SDLK_e) { scale -= xDelta * 10.f; SDL_PollEvent(&event); }
        }
        
        time_t startTime = clock();
        SDL_LockSurface(windowSurface);

        for (int curY = 0; curY < SCREEN_HEIGHT; ++curY) {
            if (event.type == SDL_QUIT ||
                event.key.keysym.sym == SDLK_ESCAPE)   { running = false; }

            float xOrigin = ((            - 400.f) * xDelta) * scale + xCenter;
            float yOrigin = (((float)curY - 300.f) * yDelta) * scale + yCenter;

            for (int curX = 0; curX < SCREEN_WIDTH; curX += 4, xOrigin += xDelta * 4 * scale) {
                RGBAQUAD *pixelsArray = (RGBAQUAD *)windowSurface->pixels + SCREEN_WIDTH * curY + curX;
                calculateMondelbrotSSE(pixelsArray, xOrigin, yOrigin, xDelta * scale);
            }
        }

        SDL_UnlockSurface(windowSurface);
        UpdateScreen(windowRenderer, windowSurface);        
        timeSpend += clock() - startTime;

        if (timeSpend > CLOCKS_PER_SEC) {
            timeSpend = 0;
            fprintf(stderr, "\rFPS: %zd", frames);
            fflush(stderr);
            frames = 0;
        }
    }

    return 0;
}

int DrawMondelbrotAVX(SDL_Surface *windowSurface, SDL_Renderer *windowRenderer)
{
    float xCenter = -1.325f,
          yCenter = 0.f,
          xDelta  = 1.f / 800.f,
          yDelta  = 1.f / 800.f,
          scale   = 1.f;
    
    SDL_Event event  = {};
    time_t timeSpend = 0;
    size_t frames    = 0;

    for (bool running = true; running; SDL_PollEvent(&event), frames++) {
        if (event.type == SDL_QUIT ||
            event.key.keysym.sym == SDLK_ESCAPE) { running = false; }

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP)    { yCenter -= yDelta * 10.f; SDL_PollEvent(&event); }
            if (event.key.keysym.sym == SDLK_DOWN)  { yCenter += yDelta * 10.f; SDL_PollEvent(&event); }
            if (event.key.keysym.sym == SDLK_LEFT)  { xCenter -= xDelta * 10.f; SDL_PollEvent(&event); }
            if (event.key.keysym.sym == SDLK_RIGHT) { xCenter += xDelta * 10.f; SDL_PollEvent(&event); }

            if (event.key.keysym.sym == SDLK_q) { scale += xDelta * 10.f; SDL_PollEvent(&event); }
            if (event.key.keysym.sym == SDLK_e) { scale -= xDelta * 10.f; SDL_PollEvent(&event); }
        }
        
        time_t startTime = clock();
        SDL_LockSurface(windowSurface);

        for (int curY = 0; curY < SCREEN_HEIGHT; ++curY) {
            if (event.type == SDL_QUIT ||
                event.key.keysym.sym == SDLK_ESCAPE)   { running = false; }

            float xOrigin = ((            - 400.f) * xDelta) * scale + xCenter;
            float yOrigin = (((float)curY - 300.f) * yDelta) * scale + yCenter;

            for (int curX = 0; curX < SCREEN_WIDTH; curX += 8, xOrigin += xDelta * 8 * scale) {
                RGBAQUAD *pixelsArray = (RGBAQUAD *)windowSurface->pixels + SCREEN_WIDTH * curY + curX;
                calculateMondelbrotAVX(pixelsArray, xOrigin, yOrigin, xDelta * scale);
            }
        }

        SDL_UnlockSurface(windowSurface);
        UpdateScreen(windowRenderer, windowSurface);
        timeSpend += clock() - startTime;

        if (timeSpend > CLOCKS_PER_SEC) {
            timeSpend = 0;
            fprintf(stderr, "\rFPS: %zd", frames);
            fflush(stderr);
            frames = 0;
        }
    }

    return 0;
}

int UpdateScreen(SDL_Renderer *windowRenderer, SDL_Surface *windowSurface)
{
    SDL_Texture *windowTexture = SDL_CreateTextureFromSurface(windowRenderer, windowSurface);
    assert(windowTexture != nullptr);

    SDL_SetRenderTarget(windowRenderer, windowTexture);
    SDL_RenderCopy(windowRenderer, windowTexture, NULL, NULL);
    SDL_RenderPresent(windowRenderer);
    
    SDL_DestroyTexture(windowTexture);
    return 0;
}

int calculateMondelbrotSSE(RGBAQUAD *pixelsArray, float xOrigin, float yOrigin, float xDelta)
{
    __m128 xOriginSSE = _mm_add_ps(_mm_set_ps1(xOrigin), _mm_mul_ps(_mm_set_ps(3.f, 2.f, 1.f, 0.f), _mm_set_ps1 (xDelta)));
    __m128 yOriginSSE =            _mm_set_ps1(yOrigin);

    __m128 xCurrentSSE = xOriginSSE;
    __m128 yCurrentSSE = yOriginSSE;

    __m128i iterationsSSE = _mm_setzero_si128();

    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        __m128 xSquaredSSE  = _mm_mul_ps(xCurrentSSE, xCurrentSSE);
        __m128 ySquaredSSE  = _mm_mul_ps(yCurrentSSE, yCurrentSSE);
        __m128 xyProductSSE = _mm_mul_ps(xCurrentSSE, yCurrentSSE);
        
        __m128 radiusSquaredSSE = _mm_add_ps(xSquaredSSE, ySquaredSSE);
        
        __m128 comparisonSSE = _mm_cmple_ps(radiusSquaredSSE, BORDER_RADIUS_SSE);         
        int bitMask = _mm_movemask_ps(comparisonSSE);
        if (!bitMask) break;

        iterationsSSE = _mm_sub_epi32(iterationsSSE, _mm_castps_si128(comparisonSSE));

        xCurrentSSE = _mm_add_ps(_mm_sub_ps(xSquaredSSE,  ySquaredSSE),  xOriginSSE);
        yCurrentSSE = _mm_add_ps(_mm_add_ps(xyProductSSE, xyProductSSE), yOriginSSE);
    }

    for (int i = 0; i < 4; i++) {
        int32_t *iterationsPointerSSE = (int32_t *)&iterationsSSE;
        pixelsArray[i] = (iterationsPointerSSE[i] < MAX_ITERATIONS) ? RGBAQUAD { 255, 255, 255, 255 } : RGBAQUAD {};
    }

    return 0;
}

int calculateMondelbrotAVX(RGBAQUAD *pixelsArray, float xOrigin, float yOrigin, float xDelta)
{
    __m256 mulArray   = _mm256_set_ps(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);
    __m256 xOriginSSE = _mm256_add_ps(_mm256_set1_ps(xOrigin), _mm256_mul_ps(mulArray, _mm256_set1_ps (xDelta)));
    __m256 yOriginSSE =               _mm256_set1_ps(yOrigin);

    __m256 xCurrentSSE = xOriginSSE;
    __m256 yCurrentSSE = yOriginSSE;

    __m256i iterationsSSE = _mm256_setzero_si256();

    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        __m256 xSquaredSSE  = _mm256_mul_ps(xCurrentSSE, xCurrentSSE);
        __m256 ySquaredSSE  = _mm256_mul_ps(yCurrentSSE, yCurrentSSE);
        
        __m256 radiusSquaredSSE = _mm256_add_ps(xSquaredSSE, ySquaredSSE);
        
        __m256 comparisonSSE = _mm256_cmp_ps(radiusSquaredSSE, BORDER_RADIUS_AVX, _CMP_LE_OS);
        int64_t bitMask = _mm256_movemask_ps(comparisonSSE);
        if (!bitMask) break;

        iterationsSSE = _mm256_sub_epi32(iterationsSSE, _mm256_castps_si256(comparisonSSE));

        __m256 xyProductSSE = _mm256_mul_ps(xCurrentSSE, yCurrentSSE);

        xCurrentSSE = _mm256_add_ps(_mm256_sub_ps(xSquaredSSE,  ySquaredSSE),  xOriginSSE);
        yCurrentSSE = _mm256_add_ps(_mm256_add_ps(xyProductSSE, xyProductSSE), yOriginSSE);
    }

    for (int i = 0; i < 8; i++) {
        int32_t *iterationsPointerSSE = (int32_t *)&iterationsSSE;
        pixelsArray[i] = (iterationsPointerSSE[i] < MAX_ITERATIONS) ? RGBAQUAD { 255, 255, 255, 255 } : RGBAQUAD {};
    }

    return 0;
}

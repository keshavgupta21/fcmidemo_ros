#pragma once

#include <SDL2/SDL.h>

#include <fcmidemo/config.hpp>

class GUI {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *miTexture, *occTexture;
    unsigned int *miPixelData, *occPixelData;
    SDL_Rect miRect, occRect;

public:
    GUI(unsigned int* occFrameBuf, unsigned int* miFrameBuf);

    void updateWindow();

    ~GUI();
};
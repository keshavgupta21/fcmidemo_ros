#pragma once

#include <SDL2/SDL.h>

#include <fcmidemo/config.h>

class GUI {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *miTexture, *occTexture, *carTexture;
    unsigned int *miPixelData, *occPixelData, *carPixelData;
    SDL_Rect miRect, occRect, carRect;
    double carRectAng;

    static const int carSize = 10;

public:
    GUI(unsigned int* occFrameBuf, unsigned int* miFrameBuf);

    void setScanPose(int x, int y, float th);

    void updateWindow();

    ~GUI();
};
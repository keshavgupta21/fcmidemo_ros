#include <iostream>
#include <fcmidemo/gui.h>

GUI::GUI(unsigned int* occFrameBuf, unsigned int* miFrameBuf) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    }
    window = SDL_CreateWindow("MI Display", 100, 100, MAP_SIZE * 2, MAP_SIZE, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    }
    miTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, MAP_SIZE, MAP_SIZE);
    if (miTexture == nullptr) {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
    }
    occTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, MAP_SIZE, MAP_SIZE);
    if (occTexture == nullptr) {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
    }

    occRect.x = 0;
    occRect.y = 0;
    occRect.h = MAP_SIZE;
    occRect.w = MAP_SIZE;

    miRect.x = MAP_SIZE;
    miRect.y = 0;
    miRect.h = MAP_SIZE;
    miRect.w = MAP_SIZE;

    occPixelData = occFrameBuf;
    miPixelData = miFrameBuf;
}

void GUI::updateWindow() {
    SDL_UpdateTexture(miTexture, nullptr, miPixelData, MAP_SIZE * sizeof(unsigned int));
    SDL_UpdateTexture(occTexture, nullptr, occPixelData, MAP_SIZE * sizeof(unsigned int));
    SDL_RenderCopy(renderer, occTexture, nullptr, &occRect);
    SDL_RenderCopy(renderer, miTexture, nullptr, &miRect);
    SDL_RenderPresent(renderer);
}

GUI::~GUI() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
#include <iostream>
#include <fcmidemo/gui.h>
#include <fcmidemo/config.h>

GUI::GUI(unsigned int* occFrameBuf, unsigned int* miFrameBuf) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    }
    window = SDL_CreateWindow("MI Display", 100, 100, MAP_SIZE * 2, MAP_SIZE, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    }
    renderer = SDL_CreateRenderer(window, 2, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    }
    miTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, MAP_SIZE, MAP_SIZE);
    if (miTexture == nullptr) {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
    }
    SDL_SetTextureBlendMode(miTexture, SDL_BLENDMODE_BLEND);
    occTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, MAP_SIZE, MAP_SIZE);
    if (occTexture == nullptr) {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
    }
    SDL_SetTextureBlendMode(occTexture, SDL_BLENDMODE_BLEND);
    carTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, carSize, carSize);
    if (occTexture == nullptr) {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
    }
    SDL_SetTextureBlendMode(carTexture, SDL_BLENDMODE_BLEND);
    carPixelData = new unsigned int[carSize * carSize];
    for (int y = 0; y < carSize; y++) {
        for (int x = 0; x < carSize; x++) {
            carPixelData[y*carSize + x] = 0;
        }
    }
    for (int y = 0; y < carSize; y++) {
        for (int x = carSize/2 - 2; x < carSize/2 + 2; x++) {
            carPixelData[y*carSize + x] = 0x00ff00ff;
        }
    }

    occRect.x = 0;
    occRect.y = 0;
    occRect.h = MAP_SIZE;
    occRect.w = MAP_SIZE;

    miRect.x = MAP_SIZE;
    miRect.y = 0;
    miRect.h = MAP_SIZE;
    miRect.w = MAP_SIZE;

    carRect.x = 0;
    carRect.y = 0;
    carRect.w = carSize;
    carRect.h = carSize;

    occPixelData = occFrameBuf;
    miPixelData = miFrameBuf;
}

void GUI::setScanPose(int x, int y, float th) {
    carRect.x = x - carSize/2;
    carRect.y = y - carSize/2;
    carRectAng = 360.0 - th * 180.0/MATH_PI;
}

void GUI::updateWindow() {
    SDL_UpdateTexture(miTexture, nullptr, miPixelData, MAP_SIZE * sizeof(unsigned int));
    SDL_UpdateTexture(occTexture, nullptr, occPixelData, MAP_SIZE * sizeof(unsigned int));
    SDL_UpdateTexture(carTexture, nullptr, carPixelData, carSize * sizeof(unsigned int));
    SDL_RenderCopy(renderer, occTexture, nullptr, &occRect);
    SDL_RenderCopy(renderer, miTexture, nullptr, &miRect);
    SDL_RenderCopyEx(renderer, carTexture, nullptr, &carRect, carRectAng, nullptr, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
}

GUI::~GUI() {
    delete carPixelData;
    SDL_DestroyWindow(window);
    SDL_Quit();
}
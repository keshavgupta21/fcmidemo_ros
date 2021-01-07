#include <iostream>
#include <chrono>

#include <fcmidemo/fcmidriver.hpp>
#include <fcmidemo/fcmigui.hpp>
#include <fcmidemo/scanhandler.hpp>

int main() {
    FCMIDriver fcmi;
    ScanHandler scan(fcmi.getOccPtr());
    GUI gui(scan.getFrameBuf(), fcmi.getMIPtr());

    std::chrono::system_clock::time_point start = std::chrono::high_resolution_clock::now(), end;
    unsigned int frameCounter = 0;
    unsigned char t = 0;
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event) > 0) {
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }

        scan.updateScan(t++);
        fcmi.calcMI();
        gui.updateWindow();

        frameCounter++;
        if (frameCounter == 100) {
            end = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Last 100 frames FPS: " << 1000.0 * frameCounter / time << " fps" << std::endl;
            frameCounter = 0;
            start = std::chrono::high_resolution_clock::now();
        }
    }

    return 0;
}
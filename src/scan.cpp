#include <fcmidemo/scan.h>
#include <fcmidemo/cmap.h>

ScanHandler::ScanHandler(occdata_t* occGrid): occGrid(occGrid) {
    occFrameBuf = new unsigned int[MAP_SIZE * MAP_SIZE];
}

void ScanHandler::updateScan(unsigned char t) {
    unsigned char pix;
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            pix = (y == t) ? 100 : 10;
            occGrid[y*MAP_SIZE + x] = pix;
            occFrameBuf[y*MAP_SIZE + x] = CM_GREYS[pix];
        }
    }
}

unsigned int* ScanHandler::getFrameBuf() {
    return occFrameBuf;
}

ScanHandler::~ScanHandler() {
    delete occFrameBuf;
}
#include <iostream>

#include <sensor_msgs/LaserScan.h>

#include <fcmidemo/scan.h>
#include <fcmidemo/cmap.h>
#include <fcmidemo/config.h>

ScanHandler::ScanHandler(occdata_t* occGrid): occGrid(occGrid) {
    occFrameBuf = new unsigned int[MAP_SIZE * MAP_SIZE];
    resetOccGrid();
}

void ScanHandler::resetOccGrid() {
    unsigned char initPix = 1;
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            occGrid[y*MAP_SIZE + x] = initPix;
            occFrameBuf[y*MAP_SIZE + x] = CM_GREYS[initPix];
        }
    }
}

void ScanHandler::setResolution(float res) {
    mapRes = res;
}

void ScanHandler::setScanPose(int x, int y, float th) {
    scanX = x;
    scanY = y;
    scanTh = th;
}

void ScanHandler::updateScan(const sensor_msgs::LaserScan &scanData) {
    int finalX, finalY, pix, finalDist;
    float th = scanData.angle_min, p, s, del;
    for (float range: scanData.ranges) {
        finalDist = range/mapRes;
        for (float dist = 0; dist < finalDist + 1; dist += 1.0f) {
            // calculate final X and Y
            finalX = scanX + dist * std::sin(scanTh + th);
            finalY = scanY + dist * std::cos(scanTh + th);
            
            // clip them to inside the map
            finalX = (finalX >= MAP_SIZE) ? MAP_SIZE - 1 : ((finalX < 0) ? 0 : finalX);
            finalY = (finalY >= MAP_SIZE) ? MAP_SIZE - 1 : ((finalY < 0) ? 0 : finalY);

            // set those pixels empty
            p = occGrid[finalY*MAP_SIZE + finalX]/100.0;
            del = ((dist > finalDist - 1)) ? DEL_OCC : DEL_EMP;
            s = (del * p)/(1 + (del - 1) * p);
            pix = s * 100.0;
            occGrid[finalY*MAP_SIZE + finalX] = pix;
            occFrameBuf[finalY*MAP_SIZE + finalX] = CM_GREYS[pix];
        }

        th += scanData.angle_increment;
    }
}

unsigned int* ScanHandler::getFrameBuf() {
    return occFrameBuf;
}

ScanHandler::~ScanHandler() {
    delete occFrameBuf;
}
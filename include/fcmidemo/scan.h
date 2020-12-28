#pragma once

#include <fcmidemo/config.h>

class ScanHandler {
private:
    occdata_t *occGrid;
    unsigned int *occFrameBuf;
    int scanX, scanY;
    float mapRes, scanTh;

public:
    ScanHandler(occdata_t* occGrid);

    void resetOccGrid();

    void setResolution(float res);

    void setScanPose(int x, int y, float th);

    void updateScan(const sensor_msgs::LaserScan &scanData);

    unsigned int* getFrameBuf();

    ~ScanHandler();
};
#pragma once

#include <fcmidemo/config.hpp>

class ScanHandler {
private:
    occdata_t *occGrid;
    unsigned int *occFrameBuf;

public:
    ScanHandler(occdata_t* occGrid);

    void updateScan(unsigned char t);

    unsigned int* getFrameBuf();

    ~ScanHandler();
};
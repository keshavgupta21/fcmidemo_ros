#pragma once

#include <pynq_api.h>

#include <fcmidemo/config.h>
#include <fcmidemo/axigpio.h>

class FCMIDriver {
private:
    PYNQ_SHARED_MEMORY occTxBuf, miRxBuf;
    PYNQ_AXI_GPIO_CH state, reset, start, mapsz, chksm, cycnt, maxmi;
    PYNQ_AXI_DMA dma;
    unsigned int gpioVal;

public:
    FCMIDriver(const char *fname);

    occdata_t* getOccPtr() {return (occdata_t*) occTxBuf.pointer;}

    midata_t* getMIPtr() {return (midata_t*) miRxBuf.pointer;}

    midata_t getMaxMI();

    void calcMI();

    ~FCMIDriver();
};
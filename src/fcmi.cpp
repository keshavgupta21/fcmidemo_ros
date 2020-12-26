#include <iostream>

#include <fcmidemo/fcmi.h>

#include <fcmidemo/baseaddr.h>

FCMIDriver::FCMIDriver(char *fname) {
    PYNQ_loadBitstream(fname);
    PYNQ_allocatedSharedMemory(&occTxBuf, sizeof(occdata_t) * MAP_SIZE * MAP_SIZE, 1);
    PYNQ_allocatedSharedMemory(&miRxBuf, sizeof(midata_t) * MAP_SIZE * MAP_SIZE, 1);
    PYNQ_openAXIGPIOCH(&state, AXI_GPIO0_BASE, 1);
    PYNQ_openAXIGPIOCH(&reset, AXI_GPIO0_BASE, 2);
    PYNQ_openAXIGPIOCH(&start, AXI_GPIO1_BASE, 1);
    PYNQ_openAXIGPIOCH(&mapsz, AXI_GPIO1_BASE, 2);
    PYNQ_openAXIGPIOCH(&chksm, AXI_GPIO2_BASE, 1);
    PYNQ_openAXIGPIOCH(&cycnt, AXI_GPIO2_BASE, 2);
    PYNQ_openAXIGPIOCH(&maxmi, AXI_GPIO3_BASE, 1);
    PYNQ_openDMA(&dma, AXI_DMA_BASE);
    gpioVal = LOG_MAP_SIZE;
    PYNQ_writeAXIGPIOCH(&mapsz, &gpioVal);
}

void FCMIDriver::calcMI() {
    PYNQ_readAXIGPIOCH(&state, &gpioVal);
    if (gpioVal != 10) {
        std::cerr << "Failed MI calculation because PL not ready." << std::endl;
        return;
    }
    // assert start signal bit
    gpioVal = 1;
    PYNQ_writeAXIGPIOCH(&start, &gpioVal);
    gpioVal = 0;
    PYNQ_writeAXIGPIOCH(&start, &gpioVal);

    // start DMA write and wait for finish
    PYNQ_writeDMA(&dma, &occTxBuf, 0, sizeof(occdata_t) * MAP_SIZE * MAP_SIZE);
    PYNQ_waitForDMAComplete(&dma, AXI_DMA_WRITE);

    // start DMA read and wait for finish
    PYNQ_readDMA(&dma, &miRxBuf, 0, sizeof(midata_t) * MAP_SIZE * MAP_SIZE);
    PYNQ_waitForDMAComplete(&dma, AXI_DMA_READ);
}

midata_t FCMIDriver::getMaxMI() {
    PYNQ_readAXIGPIOCH(&maxmi, &gpioVal);
    return gpioVal;
}

FCMIDriver::~FCMIDriver() {
    PYNQ_closeDMA(&dma);
    PYNQ_closeAXIGPIOCH(&state);
    PYNQ_closeAXIGPIOCH(&reset);
    PYNQ_closeAXIGPIOCH(&start);
    PYNQ_closeAXIGPIOCH(&mapsz);
    PYNQ_closeAXIGPIOCH(&chksm);
    PYNQ_closeAXIGPIOCH(&cycnt);
    PYNQ_freeSharedMemory(&occTxBuf);
    PYNQ_freeSharedMemory(&miRxBuf);
}
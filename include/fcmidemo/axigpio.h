#pragma once

#include <pynq_api.h>

#define PYNQ_AXI_GPIO_CH1DATA_OFFSET 0x0
#define PYNQ_AXI_GPIO_CH1DATA_LEN 0x4
#define PYNQ_AXI_GPIO_CH1TRI_OFFSET 0x4
#define PYNQ_AXI_GPIO_CH1TRI_LEN 0x4
#define PYNQ_AXI_GPIO_CH2DATA_OFFSET 0x8
#define PYNQ_AXI_GPIO_CH2DATA_LEN 0x4
#define PYNQ_AXI_GPIO_CH2TRI_OFFSET 0xc
#define PYNQ_AXI_GPIO_CH2TRI_LEN 0x4

typedef struct axi_gpio_ch_struct {
    PYNQ_MMIO_WINDOW mmio_window;
    size_t dataOffset;
    size_t dataLen;
    size_t triOffset;
    size_t triLen;
} PYNQ_AXI_GPIO_CH;

// AXI GPIO API
int PYNQ_openAXIGPIOCH(PYNQ_AXI_GPIO_CH* gpio_ch, size_t base_address, size_t chnum);

int PYNQ_writeAXIGPIOCH(PYNQ_AXI_GPIO_CH* gpio_ch, unsigned int *data);

int PYNQ_readAXIGPIOCH(PYNQ_AXI_GPIO_CH* gpio_ch, unsigned int *data);

int PYNQ_closeAXIGPIOCH(PYNQ_AXI_GPIO_CH* gpio_ch);
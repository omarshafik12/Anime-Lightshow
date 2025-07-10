/*
To give credit this was orginally boilerplate code from: https://dronebotworkshop.com/esp32-i2s/ 
*/

// Include the I2S driver
#include <driver/i2s.h>

// GPIO Connections
int WS = 8;
int SCK = 18;
int DA = 15;

// Port & Buffer setup
#define I2S_PORT I2S_NUM_0
#define DMA_SAMPLES 512
#define DMA_BLOCKS 8

int16_t sBuffer[DMA_SAMPLES];


// Config & install
const i2s_config_t i2s_config = {
  .mode = I2S_MODE_MASTER | I2S_MODE_RX,
  .sample_rate = 44100,                           // CD-quality: 44 100 samples per second
  .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,   // Each *channel* sample is 16 bits
  .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,    // left channel only
  .communication_format = I2S_COMM_FORMAT_STAND_I2S,
  .intr_alloc_flags = 0,                          // Default interrupt priority
  .dma_buf_count = DMA_BLOCKS,
  .dma_buf_len = DMA_SAMPLES,
  .use_apll = false
};

// pin mapping
const i2s_pin_config_t pin_config = {
  .bck_io_num   = SCK,   // Bit-clock out
  .ws_io_num    = WS,    // Word-select / LRCLK
  .data_out_num = -1,    
  .data_in_num  = DA     // Mic data in
};

// i2s setup
void setup_i2s(){
  i2s_driver_install(I2S_PORT, &i2s_config, 0, nullptr);
  i2s_set_pin(I2S_PORT, &pin_config);
}

/*
Remaining Tasks:
1. implement the ability for DMA to transfer to PSRAM
2. Figure out how to lower interrupt amounts
3. Ensuring that data is feed at a continous and that there is headroom for delays or variances
4. Decide upon having a second mic to have lights sync with beat of song
5. Wav file conversion post TinyML acceptance
6. Completing memory allocation diagram
7. Testing various memory in ESP32, especially the SRAM and if it is able to withstand the memory demand of said program
*/
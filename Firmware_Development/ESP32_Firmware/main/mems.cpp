/*
To give credit this was orginally boilerplate code from: https://dronebotworkshop.com/esp32-i2s/ 

This file configures the Mems mic, takes the digital data and stores it in DMA buffers, and then finally lets it rest within the PSRAM 
*/

// Include the I2S driver
#include <driver/i2s.h>
#include "main.hpp"
#include <cstring> 

// GPIO Connections
int i2s_WS_pin = 8;
int i2s_SCK_pin = 18;
int i2s_DA_pin = 15;

// Port & Buffer setup
#define I2S_PORT I2S_NUM_0
#define DMA_SAMPLES 17640 // 200 ms of audio
#define DMA_BLOCKS 2 //32768 bytes total

int16_t sBuffer[DMA_SAMPLES];


// Config & install
const i2s_config_t i2s_config = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
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
  .bck_io_num   = i2s_SCK_pin,   // Bit-clock out
  .ws_io_num    = i2s_WS_pin,    // Word-select / LRCLK
  .data_out_num = -1,    
  .data_in_num  = i2s_DA_pin     // Mic data in
};

// i2s setup
void setup_i2s(){
  i2s_driver_install(I2S_PORT, &i2s_config, 0, nullptr);
  i2s_set_pin(I2S_PORT, &pin_config);
}

//sending data from sBuffer to Psram
void placeInPSRAM(uint8_t* psdRamBuffer) {
  size_t bytes_read = 0;
  const size_t chunk_size = DMA_SAMPLES * sizeof(int16_t);
  static size_t ps_index = 0; //write postion in PSRAM

  //Wait for buffer to have data and then copy it to sBuffer
  i2s_read(I2S_PORT, (void*)sBuffer, chunk_size, &bytes_read, portMAX_DELAY);

  //Copying data to PSRAM
  memcpy(psdRamBuffer + ps_index, sBuffer, bytes_read);
  ps_index += bytes_read;

  //Memory visualization
  Serial.println(ps_index);

  // Reset when full
  if (ps_index >= FIVE_SECONDS){
    //place function call of making audio data into WAV file or sending direct to TinyML

    Serial.println("PSRAM Resetting");
    ps_index = 0; 
  }
}

/*
Remaining Tasks:
Write the functions that transfer data out of buffers to PSRAM and then another function to take data out of PSRAM and delete what is there. 
1. implement the ability for DMA to transfer to PSRAM
2. Figure out how to lower interrupt amounts
3. Ensuring that data is feed at a continous and that there is headroom for delays or variances
4. Decide upon having a second mic to have lights sync with beat of song
5. Wav file conversion post TinyML acceptance
6. Completing memory allocation diagram
7. Testing various memory in ESP32, especially the SRAM and if it is able to withstand the memory demand of said program
*/
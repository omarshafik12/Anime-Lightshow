#include <Arduino.h>
#include <driver/i2s.h>

// how many bytes needed for exactly 5 seconds of audio:
static constexpr size_t FIVE_SECONDS = 441000;  
// total PSRAM, extra for some headroom, around 5.95 s of audio
static constexpr size_t PSRAM_TOTAL   = 524288;  

// mems.cpp functions
void setup_i2s();
void placeInPSRAM(uint8_t* psdRamBuffer);
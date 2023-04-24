/******************************************************************************
MXC6655.cpp
Library for MXC6655 accelerometer 
Bobby Schulz @ GEMS Sensing
8/1/2022
Julien Boriasse
4/24/2023

MXC6655 is a low cost MEMS accelerometer 

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef MXC6655_h
#define MXC6655_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#include <Wire.h>
#else
#include <Particle.h> // Not tested with Particle
#endif

class MXC6655
{
//   enum class Range: int
// 		{
// 			EIGHT_G = 2,
//             FOUR_G = 1,
//             TWO_G = 0
// 		};
  constexpr static int TWO_G = 0;

  // Addresses of registers
  const uint8_t REG_XAXIS = 0x03;
  const uint8_t REG_YAXIS = 0x05;
  const uint8_t REG_ZAXIS = 0x07;
  const uint8_t REG_TOUT = 0x09;

  public:
    // Constructor
    MXC6655(); 

    // Public methods
    int begin(int _ADR);
    int begin(void);
    
    float getAccel(uint8_t axis, uint8_t range = TWO_G); // Kept for backwards compatibility
    
    // Get acceleration from sensor for one axis
    float getAccel(uint8_t axis);

    // Get acceleration from sensor for all axes
    int updateAccelAll();

    // Set offsets
    void setOffsetX(float offset_x);
    void setOffsetY(float offset_y);
    void setOffsetZ(float offset_z);

    // Get offsets
    float getOffsetX(void);
    float getOffsetY(void);
    float getOffsetZ(void);

    // Get acceleration from attributes
    float getAccelX(void);
    float getAccelY(void);
    float getAccelZ(void);

    // Get temperature
    float getTemp();

  private:
    const int ADR = 0x15;
    float data[3] = {0, 0, 0};
    float offset[3] = {0, 0, 0};
    
};

#endif
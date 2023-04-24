/******************************************************************************
MXC6655.cpp
Library for MXC6655 accelerometer
Bobby Schulz @ GEMS Sensing
8/1/2022

MXC6655 is a low cost MEMS accelerometer

Distributed as-is; no warranty is given.
******************************************************************************/

#include <MXC6655.h>

/*
 * Constructor
 */
MXC6655::MXC6655()
{
}

/*
 * Initialize I2C and check for device
 */
int MXC6655::begin()
{
#if defined(ARDUINO) && ARDUINO >= 100
    Wire.begin();
#elif defined(PARTICLE)
    if (!Wire.isEnabled())
        Wire.begin(); // Only initialize I2C if not done already //INCLUDE FOR USE WITH PARTICLE
#endif
    Wire.beginTransmission(ADR);
    Wire.write(0x0D);                   // Write to control register
    Wire.write(0x00);                   // Make sure register is cleared - Self test off, FSR = 2g, Power down false
    int error = Wire.endTransmission(); // Write back I2C status
    if (error == 0)
    {
        unsigned long localTime = millis();
        uint8_t val = 0;
        while ((millis() - localTime) < 400)
        {                                // Max startup time is 400ms
            Wire.beginTransmission(ADR); // Wait for new data
            Wire.write(0x01);
            Wire.endTransmission();

            Wire.requestFrom(ADR, 1);
            val = Wire.read();
            if ((val & 0x01) == 0x01)
                break; // Break out of loop once data ready bit set
        }
    }
    return error; // return error state either way
}

/*
 * Get acceleration data from a given axis
 *
 * axis: 0 = X, 1 = Y, 2 = Z
 * range: Not used
 *
 * Returns: acceleration in g's
 */
float MXC6655::getAccel(uint8_t axis, uint8_t range)
{
    int val = 0; // Place data from desired access here
    Wire.beginTransmission(ADR);
    Wire.write(REG_XAXIS + axis * 2);   // Point to desired output register
    int error = Wire.endTransmission(); // Grab error from first write

    Wire.requestFrom(ADR, 2); // Read back axis values
    val = (Wire.read() << 8); // Read in high byte
    val = val | Wire.read();  // Read in low byte
    val = val >> 4;           // Shift out empty bits
    if (error == 0)
        return (float(val / 2048.0)) * 2 + offset[axis]; // Divide by 12 bit resolution, multiply by FSR, add offset //FIX! allow for multiple gain ranges
    else
        return 0;
}

/*
 * Get acceleration data from a given axis
 *
 * axis: 0 = X, 1 = Y, 2 = Z
 *
 * Returns: acceleration in g's
 */
float MXC6655::getAccel(uint8_t axis)
{
    int val = 0; // Place data from desired access here
    Wire.beginTransmission(ADR);
    Wire.write(REG_XAXIS + axis * 2);   // Point to desired output register
    int error = Wire.endTransmission(); // Grab error from first write

    Wire.requestFrom(ADR, 2); // Read back axis values
    val = (Wire.read() << 8); // Read in high byte
    val = val | Wire.read();  // Read in low byte
    val = val >> 4;           // Shift out empty bits
    if (error == 0)
        return (float(val / 2048.0)) * 2 + offset[axis]; // Divide by 12 bit resolution, multiply by FSR, add offset //FIX! allow for multiple gain ranges
    else
        return 0;
}

int MXC6655::updateAccelAll()
{
    int16_t dataRaw[3] = {0}; // X, Y, Z accel data, raw
    Wire.beginTransmission(ADR);
    Wire.write(REG_XAXIS);              // Point to first reg
    int error = Wire.endTransmission(); // Grab error from first write

    Wire.requestFrom(ADR, 6); // Get all bytes at once
    unsigned long localTime = millis();
    while (Wire.available() < 6 && (millis() - localTime) < 10)
        ; // Wait at most 10 ms for new data
    for (int i = 0; i < 3; i++)
    {
        dataRaw[i] = (Wire.read() << 8);       // Read in high byte
        dataRaw[i] = dataRaw[i] | Wire.read(); // Read in low byte
        dataRaw[i] = dataRaw[i] >> 4;          // Shift out empty bits

        // Data[i] = (float(DataRaw[i]/2048.0))*8; //Divide by 12 bit resolution, multiply by FSR = 8
        if (error == 0)
            data[i] = (float(dataRaw[i] / 2048.0)) * 2 + offset[i]; // Divide by 12 bit resolution, multiply by FSR, add offset //FIX! allow for multiple gain ranges
        else
            data[i] = 0; // If error has occored, set to zero
    }
    return error; // Return the error value regardless of state
}

// Set offsets
void MXC6655::setOffsetX(float offset_x) { offset[0] = offset_x; }
void MXC6655::setOffsetY(float offset_y) { offset[1] = offset_y; }
void MXC6655::setOffsetZ(float offset_z) { offset[2] = offset_z; }

// Get offsets
float MXC6655::getOffsetX(void) { return offset[0]; }
float MXC6655::getOffsetY(void) { return offset[1]; }
float MXC6655::getOffsetZ(void) { return offset[2]; }

// Get acceleration from attributes (updated by updateAccelAll() before calling)
float MXC6655::getAccelX(void) { return data[0]; }
float MXC6655::getAccelY(void) { return data[1]; }
float MXC6655::getAccelZ(void) { return data[2]; }

// Get temperature from sensor
float MXC6655::getTemp()
{
    Wire.beginTransmission(ADR);
    Wire.write(REG_TOUT);
    if (Wire.endTransmission() == 0)
    {
        Wire.requestFrom(ADR, 1);
        int8_t val = Wire.read();
        return (float(val) * 0.586) + 25.0;
    }
    return 0;
}

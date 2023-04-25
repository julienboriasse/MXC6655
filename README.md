# MXC6655
Arduino compatible interface for MXC6655 3 axis accelorometer 

## Code examples

Example using `getAccel()`.
```
#include "MXC6655.h"


MXC6655 accel;

void setup()  {

}

void loop() {
    if (accel.begin() != 0) {
        // each line is a new communication with the sensor
        float accX = accel.getAccel(0);
        float accY = accel.getAccel(1);
        float accZ = accel.getAccel(2);
        float temp = accel.getTemp();
    }
}
```

Example using `updateAccelAll()`.

```
#include "MXC6655.h"


MXC6655 accel;

void setup()  {

}

void loop() {
    if (accel.begin() != 0) {
        updateAccelAll(); // sensor accessed once
    }
    float accX = accel.getAccelX(0);
    float accY = accel.getAccelY(1);
    float accZ = accel.getAccelZ(2);
}
```
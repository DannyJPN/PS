#include "LPS25H.h"
#include <iostream>

int main(int argc, char** argv)
{
    double  temperature;

    LPS25H sensor("/dev/i2c-2");
    sensor.powerUp();

    //pressure = sensor.getPressure();
    //std::cout << "Pressure: " << pressure << " hPa" << std::endl;

    temperature = sensor.getTemperature();
    std::cout << temperature  << std::endl;
    
    return 0;
}
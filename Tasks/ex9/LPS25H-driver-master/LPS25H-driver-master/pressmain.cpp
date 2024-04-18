#include "LPS25H.h"
#include <iostream>

int main(int argc, char** argv)
{
    double pressure;

    LPS25H sensor("/dev/i2c-2");
    sensor.powerUp();

    pressure = sensor.getPressure();
    std::cout <<  pressure  << std::endl;

    //temperature = sensor.getTemperature();
    //std::cout << "Temperature: " << temperature << " °C" << std::endl;

    return 0;
}
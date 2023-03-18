#include <BH1750.h>

//Constants and Variables
const int LED_PIN = D7; //sets up the argon's LED to blink
BH1750 sensor(0x23, Wire);

double brightnessData = 0;
bool inTheLight = false; //sets up a simple check whether the sensor is in bright light
bool notification = true;

SYSTEM_THREAD(ENABLED);

void setup() 
{
    //Wire.begin(0x23);
    sensor.begin();
    sensor.set_sensor_mode(BH1750::forced_mode_high_res2); //configures the sensor
    pinMode(LED_PIN, OUTPUT);
}

void loop() 
{
    sensor.make_forced_measurement(); //takes a brightness measurement
    brightnessData = sensor.get_light_level();
    if (brightnessData > 10000) 
    {
        inTheLight = true;
        if (notification)
        {
            digitalWrite(LED_PIN, HIGH);
            Particle.publish("light_status", String(brightnessData), PRIVATE); //publishes an event with the brightness data
            Particle.publish("light_status", String(inTheLight), PRIVATE); //publishes an event saying if the sensor is in bright light 
            notification = false;
        }
    }
    else 
    {
        inTheLight = false;
        if (!notification)
        {
            Particle.publish("light_status", String(inTheLight), PRIVATE);
        }
        notification = true;
        digitalWrite(LED_PIN, LOW);        
    };
  delay(30000);
}
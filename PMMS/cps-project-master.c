#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPiI2C.h>

#define MAXTIMINGS 85

// set output pins for the relay & temp sensor (DHT11)
#define Motor_RelayPin  1
#define Pump_RelayPin   3
#define DHTPIN          2

int dht11_dat[5] = {0, 0, 0, 0, 0}; 

int temperature, fd, reading, level, w;

const char PCF8591 = 0x48;

int water_pump(){

    int reservoir_level = water_level(); // check the water level of plant container

    if(reservoir_level < 130){ // if level is below certain amt, pump will run
        digitalWrite(Pump_RelayPin, 1); // pump relay pin on
        printf("Pumping Water...\n");
        delay(3000)                     // pump only runs for 3 seconds, this is to avoid overflow, allow water level sensor to read water level.
        digitalWrite(Pump_RelayPin, 0); // pump relay pin off
    }
}
// the reference for code used I2C. The code was for a diff. sensor but I formatted it for the water level sensor.
// the code ran using the code which included wiringPiI2C, so i left it unchanged 
int water_level(){
    fd = wiringPiI2CSetup(PCF8591);

    while(1){
        reading = wiringPiI2CReadReg8(fd, PCF8591 + 1);

        level = reading - 24; // default val of water level sensor is 24, subtract 24 for reasier reading
        // printf("Water Level = %d\n", level); // used for testing
        break;
    }
    break(500)
    return level;
}

// follows Adeept code for the DHT11 sensor 
// values are taken and read by other function to execute main function of system
int read_dht11_dat(){
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint j = 0, i;
    float f;

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0; // array positions for dht11 values. also reads humidity but for project, I only utilized temp

    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18); 

    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40);
    pinMode(DHTPIN, INPUT);

    for(i = 0; i < MAXTIMINGS, i++){
        counter = 0;
        while(digitalRead(DHTPIN) == laststate){
            counter++;
            delayMicroseconds(1);
            if(counter == 255){
            break;
            }
        }
        laststate = digitalRead(DHTPIN);

        if(counter == 255)
            break;
    
        if((i >= 4) && (i % 2 ==0)){
            dht11_dat[j/8] << = 1;
            if(counter > 50)
                dht11_dat[j/8] |= 1;
            j++;
        }
    }

    if((j >= 15) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[3]) & 0xFF))){
        f = dht11_dat[2] * 9. / 5. + 32;
        printf("Temperature = %d.%d c (%.1f F)\n", dht11_dat[2], dht11_dat[3], f);

        int temp = dht11_dat[2];
        temp = (temp * 2) + 30;
        return temp;
    }
}

int main(void){

    if(wiringPiI2CSetup() == -1){ // ensure wiringPiI2CSetup
        printf("setup wiringPi failed!\n");
        return -1;
    }
    // set relay pins to output
    pinMode(Motor_RelayPin, OUTPUT);   
    pinMode(Pump_RelayPin, OUTPUT); 

    while(1){
        water_pump(); 
        temperature = read_dht11_dat();     // get temp val
        int water_reading = water_level();  // get plant water level

        // print statements
        printf("Current Temp: %d\n", temperature);
        printf("Current Water Level: %d\n\n", water_reading);

        int condition;
        int temp_val = temperature;

        if(temp_val > 85){ // 85 is the set value I chose, can be changed to fit needs
            condition = 1;
        }else{
            condition = 0;
        }
        // the readings are taken every 10 secs to avoid constant running
        switch(condition){
            case(1): // if temp > 85, fan wil run for 10 seconds and then code will run again to check temp
                temp_val = read_dht11_dat();
                digitalWrite(Motor_RelayPin, 1);
                printf("Fan is Running...\n");
                delay(10000);

            case(0): // once temp < 85, fan will turn off
                digitalWrite(Motor_RelayPin, 0);
                delay(10000);
        }
    }
    return 0;
}


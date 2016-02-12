
/*  Arduino sketch to run Blue Hour for installation in Baltimore, MD, USA
    Copyright 2016 New American Public Art
    Written by Brandon Stafford, brandon@rascalmicro.com */

/*  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <DmxSimple.h>
#include <string.h>

#define SENSOR_ONE 16
#define SENSOR_TWO 17
#define SENSOR_THREE 18
#define SENSOR_FOUR 19

#define TRUE 1
#define FALSE 0

#define TOO_SOON 2000

#define TOTAL_CUBES 3
#define TOP_CUBE 0
#define MIDDLE_CUBE 1
#define BOTTOM_CUBE 2

#define TOP_DMX_CHANNEL 4
#define MIDDLE_DMX_CHANNEL 7
#define BOTTOM_DMX_CHANNEL 10

#define MAX_FRAME 767.0

#define HOME_COLOR 0,0,255

#define DIT_RAMP 1
#define DIT_PEAK 500
#define DAH_RAMP 1
#define DAH_PEAK 2000

#define MORSE_LETTER_BREAK 1000
#define MORSE_WORD_BREAK 3000
#define SHY_ROBOT_LATENCY 20000

int red_trail[768] = {0};
int green_trail[768] = {0};
int blue_trail[768] = {0};

float frames[TOTAL_CUBES] = {0.0, 100.0, 200.0};
float targets[TOTAL_CUBES] = {0.0, 100.0, 200.0};
float homePositions[TOTAL_CUBES] = {0.0, 100.0, 200.0};

int eventTotal = 0;
char morse[50] = {0};
unsigned long shyRobotTime = 0;

void loadColorTrails() {
    for(int i=0; i<256; i++) {
        red_trail[i] = 0;
        green_trail[i] = 0;
        blue_trail[i] = 255;
    }
    for(int i=256; i<511; i++) {
        red_trail[i] = i-255;
        green_trail[i] = 0;
        blue_trail[i] = 511-i;
    }
    for(int i=511; i<768; i++) {
        red_trail[i] = 255;
        green_trail[i] = 0;
        blue_trail[i] = 0;
    }
}

void approachTargets() {
    for(int i=0; i<TOTAL_CUBES; i++) {
        frames[i] = frames[i] + (targets[i] - frames[i])/10.0;
    }
}

void decayTargets() {
    for(int i=0; i<TOTAL_CUBES; i++) {
        if(targets[i] > homePositions[i]) {
            targets[i] = max(0.0, targets[i] - 4.0);
        }
    }
}

char cubesStillChanging() {
    if (targets[0] - homePositions[0] > 1.0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

char tooSoon(unsigned long lastEvent) {
    if(millis() - lastEvent < TOO_SOON){
        return TRUE;
    } else {
        return FALSE;
    }
}

void checkSensors() {
    static unsigned long lastEvent = 0;

    if(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR)) {
        if(tooSoon(lastEvent)) {
            return; // do nothing, as we recently counted an event
        } else {
            eventTotal = eventTotal + 1;
            for(int i=0; i<TOTAL_CUBES; i++) {
                targets[i] = targets[i] + 150.0;
                /*if(targets[i] > MAX_FRAME) {
                    targets[i] = MAX_FRAME;
                }*/
            }
        }
    }
    lastEvent = millis(); // save for next time this function is called
}

void translateToMorse(int digit) {
    switch(digit) {
        case 0: strcat(morse, "00000"); break;
        case 1: strcat(morse, "10000"); break;
        case 2: strcat(morse, "11000"); break;
        case 3: strcat(morse, "11100"); break;
        case 4: strcat(morse, "11110"); break;
        case 5: strcat(morse, "11111"); break;
        case 6: strcat(morse, "01111"); break;
        case 7: strcat(morse, "00111"); break;
        case 8: strcat(morse, "00011"); break;
        case 9: strcat(morse, "00001"); break;
    }
}

void constructMorseString(int total) {
    memset(morse, '\0', sizeof(morse));
    if(total > 10) {
        translateToMorse(int(total/10));
        strcat(morse, "L");
    }
    translateToMorse(total % 10);
    strcat(morse, "W");
//    Serial.print(morse);
}

void shyRobotFleesToTheHeavens() {
    setTopCube(255, 255, 255);
    setMiddleCube(255, 255, 255);
    setBottomCube(255, 255, 255);
    delay(200);
    setBottomCube(0, 0, 255);
    delay(100);
    setMiddleCube(0, 0, 255);
    delay(50);
    setTopCube(0,0,255);
}

unsigned long shyRobot() {
    static int morseIndex = 0;
    constructMorseString(23);
    while(!(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR))) {
        if(morse[morseIndex] == '1') {
            dit();
        } else if(morse[morseIndex] == '0') {
            dah();
        } else if(morse[morseIndex] == 'L') {
            delay(MORSE_LETTER_BREAK);
        }
        morseIndex++;
        if(morse[morseIndex] == 'W') {
            morseIndex = 0;
            delay(MORSE_WORD_BREAK);
        }
    }
    shyRobotFleesToTheHeavens();
    return millis();
}

void dit() {
    // assume we start off blue
    int red = 0;
    int green = 0;
    for(int i=0; i<256; i++) {
        setTopCube(red, green, 255);
        red++;
        green++;
        delay(DIT_RAMP);
    }
    delay(DIT_PEAK);
    for(int i=0; i<256; i++) {
        setTopCube(red, green, 255);
        red--;
        green--;
        delay(DIT_RAMP);
    }
}

void dah() {
    // assume we start off blue
    int red = 0;
    int green = 0;
    for(int i=0; i<256; i++) {
        setTopCube(red, green, 255);
        red++;
        green++;
        delay(DAH_RAMP);
    }
    delay(2000);
    for(int i=0; i<256; i++) {
        setTopCube(red, green, 255);
        red--;
        green--;
        delay(DAH_RAMP);
    }
}

void setTopCube(int r, int g, int b) {
    DmxSimple.write(TOP_DMX_CHANNEL, r);
    DmxSimple.write(TOP_DMX_CHANNEL + 1, g);
    DmxSimple.write(TOP_DMX_CHANNEL + 2, b);
}

void setMiddleCube(int r, int g, int b) {
    DmxSimple.write(MIDDLE_DMX_CHANNEL, r);
    DmxSimple.write(MIDDLE_DMX_CHANNEL + 1, g);
    DmxSimple.write(MIDDLE_DMX_CHANNEL + 2, b);
}

void setBottomCube(int r, int g, int b) {
    DmxSimple.write(BOTTOM_DMX_CHANNEL, r);
    DmxSimple.write(BOTTOM_DMX_CHANNEL + 1, g);
    DmxSimple.write(BOTTOM_DMX_CHANNEL + 2, b);
}

void setup() {
    DmxSimple.usePin(6);

    /* DMX devices typically need to receive a complete set of channels
    ** even if you only need to adjust the first channel. You can
    ** easily change the number of channels sent here. If you don't
    ** do this, DmxSimple will set the maximum channel number to the
    ** highest channel you DmxSimple.write() to. */
    DmxSimple.maxChannel(18);
    pinMode(SENSOR_ONE, INPUT);
    pinMode(SENSOR_TWO, INPUT);
    pinMode(SENSOR_THREE, INPUT);
    pinMode(SENSOR_FOUR, INPUT);
    setTopCube(HOME_COLOR);
    setMiddleCube(HOME_COLOR);
    setBottomCube(HOME_COLOR);
    loadColorTrails();
    Serial.begin(9600);
    shyRobotTime = millis();
}

void loop() {
    checkSensors();
    decayTargets();
    approachTargets();
    Serial.print("Targets: [");
    Serial.print(targets[0]);
    Serial.print("]\n");
    Serial.print(eventTotal);
    if(cubesStillChanging()) {
        setTopCube(red_trail[int(frames[TOP_CUBE])], green_trail[int(frames[TOP_CUBE])], blue_trail[int(frames[TOP_CUBE])]);
        setMiddleCube(red_trail[int(frames[MIDDLE_CUBE])], green_trail[int(frames[MIDDLE_CUBE])], blue_trail[int(frames[MIDDLE_CUBE])]);
        setBottomCube(red_trail[int(frames[BOTTOM_CUBE])], green_trail[int(frames[BOTTOM_CUBE])], blue_trail[int(frames[BOTTOM_CUBE])]);
    }
    Serial.println("nothing");
    if(millis() - shyRobotTime > SHY_ROBOT_LATENCY) {
        Serial.println("It's robot time.");
        shyRobotTime = shyRobot();
        Serial.println("Run away! Run away!");
    }
    if(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR)) {
        shyRobotTime = millis();
    }
    delay(100);
}

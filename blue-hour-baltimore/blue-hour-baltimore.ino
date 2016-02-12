
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

#define INITIAL_COLOR 0,0,255

#define DIT_RAMP 1
#define DIT_PEAK 500
#define DAH_RAMP 1
#define DAH_PEAK 2000

int red_trail[768] = {0};
int green_trail[768] = {0};
int blue_trail[768] = {0};

float frames[TOTAL_CUBES] = {0.0, 100.0, 200.0};
float targets[TOTAL_CUBES] = {0.0, 100.0, 200.0};
float homePositions[TOTAL_CUBES] = {0.0, 100.0, 200.0};

int eventTotal = 0;

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

void translateToMorse(int digit, char *buf) {
/*    switch(digit) {
        case 0: strcat(buf, '00000');
        case 1: strcat(buf, '10000');
        case 2: strcat(buf, '11000');
        case 3: strcat(buf, '11100');
        case 4: strcat(buf, '11110');
        case 5: strcat(buf, '11111');
        case 6: strcat(buf, '01111');
        case 7: strcat(buf, '00111');
        case 8: strcat(buf, '00011');
        case 9: strcat(buf, '00001');
    } */
}

void constructMorseString() {
/*    var morse = '';
    morse = morse.concat(translateToMorse(eventTotal % 10));
    if(eventTotal > 10) {
        morse = 'L'.concat(morse);
        morse = translateToMorse(int(eventTotal/10)).concat(morse);
    }
    morse = morse.concat('W');
    return morse; */
}

void shyRobot() {
/*    console.log("Shy robot emerging.");
    var morse = constructMorseString();
    if ( typeof shyRobot.MorseIndex == 'undefined' ) {
        shyRobot.MorseIndex = 0;
    }
    if(morse[shyRobot.MorseIndex] == '1') {
        dit();
    } else {
        dah();
    }
    shyRobot.MorseIndex++;
    if(shyRobot.MorseIndex == morse.length) {
        shyRobot.MorseIndex = 0;
    } */
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
    setTopCube(INITIAL_COLOR);
    setMiddleCube(INITIAL_COLOR);
    setBottomCube(INITIAL_COLOR);
    loadColorTrails();
    Serial.begin(9600);
    /*shyRobotTimeoutID = setTimeout(shyRobot, 5000); */
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
    delay(100);
}

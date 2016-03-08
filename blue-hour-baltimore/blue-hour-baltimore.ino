
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
#include "blue-hour-color-trails.h"

#define SENSOR_ONE 16
#define SENSOR_TWO 17
#define SENSOR_THREE 18
#define SENSOR_FOUR 19

#define TRUE 1
#define FALSE 0

#define LOOP_PERIOD 100
#define FRAMES_TO_JUMP_ON_MOTION 200.0
#define CUBIC 3
#define CUBIC_EASING_DURATION_IN_MS 1000.0

// Larger divisor means slower decay
#define DECAY_DIVISOR 50.0

#define TOO_SOON 1000

#define TOTAL_CUBES 3
#define TOP_CUBE 0
#define MIDDLE_CUBE 1
#define BOTTOM_CUBE 2
#define LOWER_TWO_CUBES 3

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

#define FRAME_LOOP_START 6000
#define TARGET_MARGIN 1.0

float frames[TOTAL_CUBES] = {1000.0, 2500.0, 4000.0};
float targets[TOTAL_CUBES] = {1000.0, 2500.0, 4000.0};
float frame_snapshot[TOTAL_CUBES] = {1000.0, 2500.0, 4000.0};
float homePositions[TOTAL_CUBES] = {1000.0, 2500.0, 4000.0};

float millis_at_last_trigger = 0;

int eventTotal = 0;
char morse[50] = {0};
unsigned long shyRobotTime = 0;

float easing(int mode, int cube_index, float ms_since_trigger) {
    float b = frame_snapshot[cube_index];
    float c = targets[cube_index] - frame_snapshot[cube_index];
    float d = CUBIC_EASING_DURATION_IN_MS;
    float t = 0;
    float easing = 0;

    if(mode == CUBIC) {
        t = (ms_since_trigger/d) - 1; // scale time from [0-duration] to [-1 to 0]
        easing = c*t*t*t + c + b;
        /*if(cube_index == 2) {
            Serial.print("t: ");
            Serial.print(t);
            Serial.print("b: ");
            Serial.print(b);
            Serial.print(", e: ");
            Serial.print(easing);
            Serial.print(", ");
        }*/
        return easing;
    } else {
        Serial.println("Easing mode not set right.\n");
        return 0.0;
    }
}

void approachTargets() {
    float e;
    for(int i=0; i<TOTAL_CUBES; i++) {
        e = easing(CUBIC, i, millis() - millis_at_last_trigger);
        frames[i] = e;
    }
}

void decayTargets() {
    for(int i=0; i<TOTAL_CUBES; i++) {
        if(targets[i] > homePositions[i]) {
            targets[i] = max(0.0, targets[i] - targets[i]/DECAY_DIVISOR);
            frames[i] = targets[i]; // Here, we can make targets track frames directly
                                    // because decay is based on position in color trail, not time.
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

int checkSensors() {
    static unsigned long lastEvent = 0;

    if(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR)) {
        if(tooSoon(lastEvent)) {
            return TRUE; // do nothing, as we recently counted an event
        } else {
            eventTotal = eventTotal + 1;
            for(int i=0; i<TOTAL_CUBES; i++) {
                frame_snapshot[i] = frames[i]; // Record frames when jump occurs for use in easing
                if(targets[2] < TOTAL_FRAMES - FRAMES_TO_JUMP_ON_MOTION - 1) {
                    targets[i] = targets[i] + FRAMES_TO_JUMP_ON_MOTION;
                }
            }
            millis_at_last_trigger = millis(); // global variable for approachTargets(). Kind of sloppy.
            lastEvent = millis(); // save for next time this function is called
            return TRUE;
        }
    }
    return FALSE;
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
    delay(50);
    setTopCube(0, 0, 0);
    setMiddleCube(0, 0, 0);
    setBottomCube(255, 255, 255);
    delay(50);
    setTopCube(0, 0, 0);
    setMiddleCube(255, 255, 255);
    setBottomCube(50, 50, 50);
    delay(50);
    setTopCube(255, 255, 255);
    setMiddleCube(50, 50, 50);
    setBottomCube(10, 10, 10);
    delay(50);
    setTopCube(50, 50, 50);
    setMiddleCube(10, 10, 10);
    setBottomCube(0, 0, 0);
    delay(50);
    setTopCube(10, 10, 10);
    setMiddleCube(0, 0, 0);
    setBottomCube(0, 0, 0);
    delay(50);
    setTopCube(0, 0, 0);
    setMiddleCube(0, 0, 0);
    setBottomCube(0, 0, 0);
}

void fastBotFlashesYou() {
    setTopCube(255, 0, 0);
    setMiddleCube(255, 0, 0);
    setBottomCube(255, 0, 0);
    delay(50);
    setBottomCube(255, 255, 255);
    delay(50);
    setBottomCube(10, 10, 10);
    setMiddleCube(255, 255, 255);
    delay(50);
    setMiddleCube(10, 10, 10);
    setTopCube(255, 255, 255);   
}

unsigned long shyRobot() {
    static int morseIndex = 0;
    constructMorseString(23);
    while(!(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR))) {
        if(morse[morseIndex] == '1') {
            dit(TOP_CUBE);
        } else if(morse[morseIndex] == '0') {
            dah(TOP_CUBE);
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

void dit(int which_cubes) {
    // assume we start off blue
    int red = 0;
    int green = 0;
    for(int i=0; i<256; i++) {
        switch(which_cubes) {
            case TOP_CUBE:
                setTopCube(red, green, 255);
                break;
            case MIDDLE_CUBE:
                setMiddleCube(red, green, 255);
                break;
            case LOWER_TWO_CUBES:
                setMiddleCube(red, green, 255);
                setBottomCube(red, green, 255);
                break;
        }
        red++;
        green++;
        delay(DIT_RAMP);
    }
    delay(DIT_PEAK);
    for(int i=0; i<256; i++) {
        switch(which_cubes) {
            case TOP_CUBE:
                setTopCube(red, green, 255);
                break;
            case MIDDLE_CUBE:
                setMiddleCube(red, green, 255);
                break;
            case LOWER_TWO_CUBES:
                setMiddleCube(red, green, 255);
                setBottomCube(red, green, 255);
                break;
        }
        red--;
        green--;
        delay(DIT_RAMP);
    }
}

void dah(int which_cubes) {
    // assume we start off blue
    int red = 0;
    int green = 0;
    for(int i=0; i<256; i++) {
        switch(which_cubes) {
            case TOP_CUBE:
                setTopCube(red, green, 255);
                break;
            case MIDDLE_CUBE:
                setMiddleCube(red, green, 255);
                break;
            case LOWER_TWO_CUBES:
                setMiddleCube(red, green, 255);
                setBottomCube(red, green, 255);
                break;
        }
        red++;
        green++;
        delay(DAH_RAMP);
    }
    delay(DAH_PEAK);
    for(int i=0; i<256; i++) {
        switch(which_cubes) {
            case TOP_CUBE:
                setTopCube(red, green, 255);
                break;
            case MIDDLE_CUBE:
                setMiddleCube(red, green, 255);
                break;
            case LOWER_TWO_CUBES:
                setMiddleCube(red, green, 255);
                setBottomCube(red, green, 255);
                break;
        }
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
    Serial.begin(9600);
    shyRobotTime = millis();
}

void loop() {
    static int climbing_color_trail = 0;
    unsigned long loop_thus_far;
    signed long loop_remaining;
    static unsigned long looptimer;
    looptimer = millis(); // mark the beginning of the loop
    climbing_color_trail = checkSensors();
    if(climbing_color_trail == TRUE) {
        approachTargets();
        if(abs(targets[0] - frames[0]) < TARGET_MARGIN) { // We're close enough to our color targets.
            climbing_color_trail = FALSE;
        }
    } else {
        decayTargets();
    }
/*    Serial.print("Targets[0],[1],[2]: [");
    Serial.print(targets[0]);
    Serial.print("], [");
    Serial.print(targets[1]);
    Serial.print("], [");
    Serial.print(targets[2]);
    Serial.print("]\n");
    Serial.print("Event total: ");
    Serial.print(eventTotal);
    Serial.print("\n");*/
    Serial.print(millis());
    Serial.print(",");
    Serial.print(frames[0]);
    Serial.print(",");
    Serial.print(frames[1]);
    Serial.print(",");
    Serial.print(frames[2]);
    Serial.print(",");
    Serial.print(targets[2]);
    Serial.print("\n");
    if(cubesStillChanging()) {
        setTopCube(red_trail[int(frames[TOP_CUBE])], green_trail[int(frames[TOP_CUBE])], blue_trail[int(frames[TOP_CUBE])]);
        setMiddleCube(red_trail[int(frames[MIDDLE_CUBE])], green_trail[int(frames[MIDDLE_CUBE])], blue_trail[int(frames[MIDDLE_CUBE])]);
        setBottomCube(red_trail[int(frames[BOTTOM_CUBE])], green_trail[int(frames[BOTTOM_CUBE])], blue_trail[int(frames[BOTTOM_CUBE])]);
    }
    if(millis() - shyRobotTime > SHY_ROBOT_LATENCY) {
        Serial.println("It's robot time.");
        shyRobotTime = shyRobot();
        Serial.println("Run away! Run away!");
    }
    if(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR)) {
        shyRobotTime = millis(); // reset the shyRobot timer
    }

    /* See how long this loop has taken so far, and then delay so that we hit LOOP_PERIOD correctly. */
    loop_thus_far = millis() - looptimer;
    loop_remaining = LOOP_PERIOD - loop_thus_far;
    if(loop_remaining > 0) {
        /*Serial.println("Looptime:");
        Serial.println(loop_thus_far);*/
        delay(loop_remaining);
    } else {
        Serial.println("Loop blown!");
    }
}

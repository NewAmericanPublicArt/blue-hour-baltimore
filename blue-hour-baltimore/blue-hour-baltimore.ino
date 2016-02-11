
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

#define TOP_CUBE 2
#define MIDDLE_CUBE 1
#define BOTTOM_CUBE 0

int red_trail[768] = {0};
int green_trail[768] = {0};
int blue_trail[768] = {0};

float frames[3] = {0.0, 100.0, 200.0};
float targets[3] = {0.0, 100.0, 200.0};
float homePositions[3] = {0.0, 100.0, 200.0};

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
    for(int i=0; i<3; i++) {
        frames[i] = frames[i] + (targets[i] - frames[i])/10.0;
    }
}

void decayTargets() {
    for(int i=0; i<3; i++) {
        if(targets[i] > homePositions[i]) {
            targets[i] = targets[i] - 2.0;
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

void checkSensors() {
    static char oldMotion = FALSE;

    if(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR)) {
        if(oldMotion == TRUE) {
            return; // do nothing, as we're mid-motion
        } else { // oldMotion has ended, but we have new motion            
            oldMotion = TRUE;
            eventTotal = eventTotal + 1;
            for(int i=0; i<3; i++) {
                targets[i] = targets[i] + 150.0;
            }
        }
    } else {
        oldMotion = FALSE;
    }
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
/*    console.log('dit');
    if ( typeof dit.nextColor == 'undefined' ) {
        dit.nextColor = color('blue');
    }
    setIntervalX(function () {
        drawTopCube(dit.nextColor);
        dit.nextColor = lerpColor(dit.nextColor, color('white'), 0.5);
    }, 100, 5);
    setTimeout(function () {
        setIntervalX(function () {
            drawTopCube(dit.nextColor);
            dit.nextColor = lerpColor(dit.nextColor, color('blue'), 0.5);
        }, 100, 5);
    }, 700);
    shyRobotTimeoutID = setTimeout(shyRobot, 2200); // call shyRobot again after dit ends: 700 + 500 + 1000 = 2200 */
}

void dah() {
/*    console.log('dah');
    if ( typeof dah.nextColor == 'undefined' ) {
        dah.nextColor = color('blue');
    }
    setIntervalX(function () {
        drawTopCube(dah.nextColor);
        dah.nextColor = lerpColor(dah.nextColor, color('white'), 0.5);
    }, 100, 5);
    setTimeout(function () {
        setIntervalX(function () {
            drawTopCube(dah.nextColor);
            dah.nextColor = lerpColor(dah.nextColor, color('blue'), 0.5);
        }, 100, 5);
    }, 2000);
    shyRobotTimeoutID = setTimeout(shyRobot, 3000); // call shyRobot again after dah ends: 2000 + 500 + 1000 = 3500 */
}

void setTopCube(int r, int g, int b) {
    DmxSimple.write(4, r);
    DmxSimple.write(5, g);
    DmxSimple.write(6, b);
}

void setMiddleCube(int r, int g, int b) {
    DmxSimple.write(7, r);
    DmxSimple.write(8, g);
    DmxSimple.write(9, b);
}

void setBottomCube(int r, int g, int b) {
    DmxSimple.write(10, r);
    DmxSimple.write(11, g);
    DmxSimple.write(12, b);
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
    setTopCube(0,0,255);
    setMiddleCube(0,0,255);
    setBottomCube(0,0,255);
    loadColorTrails();
    Serial.begin(9600);

/*    drawTopCube(color(color_trails[R][int(frames[0])], color_trails[G][int(frames[0])], color_trails[B][int(frames[0])]));
    drawMiddleCube(color(color_trails[R][int(frames[1])], color_trails[G][int(frames[1])], color_trails[B][int(frames[1])]));
    drawBottomCube(color(color_trails[R][int(frames[2])], color_trails[G][int(frames[2])], color_trails[B][int(frames[2])]));
    shyRobotTimeoutID = setTimeout(shyRobot, 5000); */
}

void loop() {
/*    if(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR)) {
        Serial.println("Motion detected acccch");
        setTopCube(128,0,0);
    } else {
        Serial.println("xxxxxxx");
        setTopCube(0,0,128);
    }*/
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


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

/*upslope = Array.from(new Array(255), (x,i) => i);
everything = Array.from(new Array(255), (x,i) => 255);
nothing = Array.from(new Array(255), (x,i) => 0);
downslope = Array.from(new Array(255), (x,i) => 255 - i);
color_trails = [nothing.concat(upslope), nothing.concat(nothing), everything.concat(downslope)];

const R=0;
const G=1;
const B=2; */

int frames[3] = {0, 100, 200};
int targets[3] = {0, 100, 200};
int homePositions[3] = {0, 100, 200};

int eventTotal = 0;

void approachTargets(char *t, char *f) {
/*    f.forEach(function (item, index, array) {
        f[index] = item + (t[index] - item)/10; 
    });
    return f; */
}

void decayTargets(char *t, char *h) {
/*    t.forEach(function (item, index, array) {
        if (item > h[index]) {
            t[index] = item - 2;
        };
    });
    return t;*/
}

char cubesStillChanging() {
    if (targets[0] - homePositions[0] > 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

char sensorTriggered() {
    char motion = FALSE;

    if(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR)) {
        motion = TRUE;
    }
    /* need to move targets and count events here */
    /* if(motion) {
        targets.forEach(function (item, index, array) {
            targets[index] = item + 150;
        });
        eventTotal = eventTotal + 1;
    } */
    return motion;
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
    Serial.begin(9600);

/*    drawTopCube(color(color_trails[R][int(frames[0])], color_trails[G][int(frames[0])], color_trails[B][int(frames[0])]));
    drawMiddleCube(color(color_trails[R][int(frames[1])], color_trails[G][int(frames[1])], color_trails[B][int(frames[1])]));
    drawBottomCube(color(color_trails[R][int(frames[2])], color_trails[G][int(frames[2])], color_trails[B][int(frames[2])]));
    shyRobotTimeoutID = setTimeout(shyRobot, 5000); */
}

void loop() {
    int brightness = 125;
    int count;
    /* Simple loop to ramp up brightness */
    for (count = 0; count <= 255; count++) {
        if(digitalRead(SENSOR_ONE) || digitalRead(SENSOR_TWO) || digitalRead(SENSOR_THREE) || digitalRead(SENSOR_FOUR)) {
            Serial.println("Motion detected");
            DmxSimple.write(4, brightness + 110);
            DmxSimple.write(5, brightness + 110);
            DmxSimple.write(6, brightness);
        } else {
            Serial.println("xxxxxxx");
            DmxSimple.write(4, brightness);
            DmxSimple.write(5, brightness + 110);
            DmxSimple.write(6, brightness + 110);
        }
        /* Small delay to slow down the ramping */
        delay(50);
    }

/*    targets = decayTargets(targets, homePositions);
    frames = approachTargets(targets, frames);
    if(cubesStillChanging()) {
        clearTimeout(shyRobotTimeoutID); // shy robot hides when cubes are changing
        shyRobotTimeoutID = setTimeout(shyRobot, 5000); // motion stopped, so prepare shy robot
        console.log("Delay shy robot");
        drawTopCube(color(color_trails[R][int(frames[0])], color_trails[G][int(frames[0])], color_trails[B][int(frames[0])]));
        drawMiddleCube(color(color_trails[R][int(frames[1])], color_trails[G][int(frames[1])], color_trails[B][int(frames[1])]));
        drawBottomCube(color(color_trails[R][int(frames[2])], color_trails[G][int(frames[2])], color_trails[B][int(frames[2])]));           
    } */
}

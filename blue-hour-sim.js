upslope = Array.from(new Array(255), (x,i) => i);
everything = Array.from(new Array(255), (x,i) => 255);
nothing = Array.from(new Array(255), (x,i) => 0);
downslope = Array.from(new Array(255), (x,i) => 255 - i);
color_trails = [nothing.concat(upslope), nothing.concat(nothing), everything.concat(downslope)];

const R=0;
const G=1;
const B=2;

frames = [0, 100, 200];
targets = [0, 100, 200];
homePositions = [0, 100, 200];

eventTotal = 0;
shyRobotTimeoutID = 0;

/*const TOP = 0;
const MIDDLE = 1;
const BOTTOM = 2;*/

const SENSOR_ONE_X = 340;
const SENSOR_ONE_Y = 500;
const SENSOR_TWO_X = 440;
const SENSOR_TWO_Y = 550;
const SENSOR_THREE_X = 540;
const SENSOR_THREE_Y = 550;
const SENSOR_FOUR_X = 640;
const SENSOR_FOUR_Y = 500;

function drawTower() {
    fill('blue');
    var TOP_CUBE = rect(450, 150, 100, 100);
    var MIDDLE_CUBE = rect(450, 260, 100, 100);
    var BOTTOM_CUBE = rect(450, 370, 100, 100);
}

function drawSensors() {
    fill('white');
    ellipse(SENSOR_ONE_X, SENSOR_ONE_Y, 20, 20);
    ellipse(SENSOR_TWO_X, SENSOR_TWO_Y, 20, 20);
    ellipse(SENSOR_THREE_X, SENSOR_THREE_Y, 20, 20);
    ellipse(SENSOR_FOUR_X, SENSOR_FOUR_Y, 20, 20);
}

function drawTrail() {
    fill('BLACK');
    rect(0, 600, 1000, 200);

    stroke('WHITE');
    line(50 + frames[0], 600, 50 + frames[0], 650);
    line(50 + frames[1], 600, 50 + frames[1], 650);
    line(50 + frames[2], 600, 50 + frames[2], 650);

    stroke('RED');
    color_trails[R].forEach(function (item, index, array) {
        point(50 + index, 650 - item/10);
    });

    stroke('GREEN');
    color_trails[G].forEach(function (item, index, array) {
        point(50 + index, 650 - item/10);
    });

    stroke('BLUE');
    color_trails[B].forEach(function (item, index, array) {
        point(50 + index, 650 - item/10);
    });

    for (var step in color_trails[R]) {
        stroke(color(color_trails[R][step], color_trails[G][step], color_trails[B][step]));
        point(50 + int(step), 600);
    }
}

function approachTargets(t, f) {
    f.forEach(function (item, index, array) {
        f[index] = item + (t[index] - item)/10; 
    });
    return f;
}

function decayTargets(t, h) {
    t.forEach(function (item, index, array) {
        if (item > h[index]) {
            t[index] = item - 2;
        };
    });
    return t;
}

function drawTargetApproach() {
    stroke('WHITE');
    for(var i=100; i>0; i--) {
        var t = [[i], [i], [i]];
        var f = [[0], [0], [0]];
        f = approachTargets(t, f);
        point(i, 100-f[0]);
    }
}

function mouseClicked() {
    clearTimeout(shyRobotTimeoutID);
    shyRobotTimeoutID = setTimeout(shyRobot, 5000);

    var motion = false;
    eventTotal = eventTotal + 1;

    if(dist(mouseX, mouseY, SENSOR_ONE_X, SENSOR_ONE_Y) < 10){
        console.log('sensor 1 triggered');
        motion = true;
    }
    if(dist(mouseX, mouseY, SENSOR_TWO_X, SENSOR_TWO_Y) < 10){
        console.log('sensor 2 triggered');
        motion = true;
    }
    if(dist(mouseX, mouseY, SENSOR_THREE_X, SENSOR_THREE_Y) < 10){
        console.log('sensor 3 triggered');
        motion = true;
    }
    if(dist(mouseX, mouseY, SENSOR_FOUR_X, SENSOR_FOUR_Y) < 10){
        console.log('sensor 4 triggered');
        motion = true;
    }
    if(motion) {
        targets.forEach(function (item, index, array) {
            targets[index] = item + 150;
        });
    }
}

function translateToMorse(digit) {
    switch(digit) {
        case 0: return '00000';
        case 1: return '10000';
        case 2: return '11000';
        case 3: return '11100';
        case 4: return '11110';
        case 5: return '11111';
        case 6: return '01111';
        case 7: return '00111';
        case 8: return '00011';
        case 9: return '00001';
    }
}

function constructMorseString() {
    var morse = '';
    morse = morse.concat(translateToMorse(eventTotal % 10));
    if(eventTotal > 10) {
        morse = 'L'.concat(morse);
        morse = translateToMorse(int(eventTotal/10)).concat(morse);
    }
    morse = morse.concat('W');
    return morse;
}

function drawShyRobotData() {
    fill('white');
    textSize(20);
    text(eventTotal, 10, 750);
    text(constructMorseString(), 10, 775);
}

function shyRobot() {
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
    shyRobotTimeoutID = setTimeout(shyRobot, 1000);
}

function dit() {
    console.log('dit');
}

function dah() {
    console.log('dah');
}

function setup() {
    createCanvas(1000, 800);
    frameRate(10)
    noStroke(); 
    background(0);
    drawSensors();
    drawTower();
    shyRobotTimeoutID = setTimeout(shyRobot, 5000);
}

function draw() {
    drawTrail();
    targets = decayTargets(targets, homePositions);
    frames = approachTargets(targets, frames);
    drawTargetApproach();
    noStroke();
    fill(color(color_trails[R][int(frames[0])], color_trails[G][int(frames[0])], color_trails[B][int(frames[0])]));
    rect(450, 150, 100, 100);
    fill(color(color_trails[R][int(frames[1])], color_trails[G][int(frames[1])], color_trails[B][int(frames[1])]));
    rect(450, 260, 100, 100);
    fill(color(color_trails[R][int(frames[2])], color_trails[G][int(frames[2])], color_trails[B][int(frames[2])]));
    rect(450, 370, 100, 100);
    drawShyRobotData();
}
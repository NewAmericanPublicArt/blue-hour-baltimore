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

function mouseClicked() {
    var motion = false;

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

function setup() {
    createCanvas(1000, 800);
    frameRate(10)
    noStroke(); 
    background(0);
    drawSensors();
    drawTower();
}

function draw() {
    drawTrail();
    targets.forEach(function (item, index, array) {
        if (item > homePositions[index]) {
            targets[index] = item - 2;
        };
    });
    frames.forEach(function (item, index, array) {
        frames[index] = item + (targets[index] - item)/10; 
    });
    noStroke();
    fill(color(color_trails[R][int(frames[0])], color_trails[G][int(frames[0])], color_trails[B][int(frames[0])]));
    rect(450, 150, 100, 100);
    fill(color(color_trails[R][int(frames[1])], color_trails[G][int(frames[1])], color_trails[B][int(frames[1])]));
    rect(450, 260, 100, 100);
    fill(color(color_trails[R][int(frames[2])], color_trails[G][int(frames[2])], color_trails[B][int(frames[2])]));
    rect(450, 370, 100, 100);


}
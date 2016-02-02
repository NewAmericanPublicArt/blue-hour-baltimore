upslope = Array.from(new Array(255), (x,i) => i);
flat = Array.from(new Array(255), (x,i) => 0);
downslope = Array.from(new Array(255), (x,i) => 255 - i);
color_trails = [upslope, flat, downslope];
R=0;
G=1;
B=2;

current_step = 0;

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
    var SENSOR_ONE = ellipse(SENSOR_ONE_X, SENSOR_ONE_Y, 20, 20);
    var SENSOR_TWO = ellipse(SENSOR_TWO_X, SENSOR_TWO_Y, 20, 20);
    var SENSOR_THREE = ellipse(SENSOR_THREE_X, SENSOR_THREE_Y, 20, 20);
    var SENSOR_FOUR = ellipse(SENSOR_FOUR_X, SENSOR_FOUR_Y, 20, 20);
}

function drawTrail(current_step) {
    fill('BLACK');
    rect(100, 740, 255, 200);

    stroke('WHITE');
    line(100 + current_step, 600, 100 + current_step, 650);

    stroke('RED');
    color_trails[R].forEach(function (item, index, array) {
        point(100 + index, 650 - item/10);
    });

    stroke('GREEN');
    color_trails[G].forEach(function (item, index, array) {
        point(100 + index, 650 - item/10);
    });

    stroke('BLUE');
    color_trails[B].forEach(function (item, index, array) {
        point(100 + index, 650 - item/10);
    });

    for (var step in color_trails[R]) {
        stroke(color(color_trails[R][step], color_trails[G][step], color_trails[B][step]));
        point(100 + int(step), 600);
    }
}

function mouseClicked() {
    if(dist(mouseX, mouseY, SENSOR_ONE_X, SENSOR_ONE_Y) < 10){
        console.log('sensor 1 triggered');
    }
    if(dist(mouseX, mouseY, SENSOR_TWO_X, SENSOR_TWO_Y) < 10){
        console.log('sensor 2 triggered');
    }
    if(dist(mouseX, mouseY, SENSOR_THREE_X, SENSOR_THREE_Y) < 10){
        console.log('sensor 3 triggered');
    }
    if(dist(mouseX, mouseY, SENSOR_FOUR_X, SENSOR_FOUR_Y) < 10){
        console.log('sensor 4 triggered');
    }
}

function setup() {
    createCanvas(1000, 800);
    frameRate(10)
    noStroke(); 
    background(0);
    drawSensors();
}

function draw() {
    drawTower();
    drawTrail(current_step);
}
let distance, velocity, strength, temperature;
let tick_count;
let FREQ;
let radio, checkbox;

var VM_Mode = false;
var Strength_Flag = false;
var Temperature_Flag = false;

// Keep track of our socket connection
var socket;

function setup() {
    background(0);
    // // Start a socket connection to the server
    // // Some day we would run this server somewhere else
    socket = io.connect('http://localhost:8000');

    // create canvas
    createCanvas(650, 250);
    textSize(15);
    noStroke();

    mode = 0
    format_value = "Binary";
    FREQ = 30
    tick_count = 0;
    textAlign(CENTER);

    formatRadio = createRadio();
    formatRadio.option('Decimal');
    formatRadio.option('Hexadecimal');
    formatRadio.option('Binary');
    formatRadio.position(220, 220);
    
    checkbox_velocity = createCheckbox('', false);
    checkbox_velocity.changed(set_VM);
    checkbox_velocity.position(200, 169);

    checkbox_strength = createCheckbox('', false);
    checkbox_strength.changed(set_ST);
    checkbox_strength.position(358, 169);

    checkbox_temperature = createCheckbox('', false);
    checkbox_temperature.changed(set_TEMP);
    checkbox_temperature.position(510, 169);

    // create sliders
    distance = createSlider(-1048576, 1048576, 0);
    distance.position(20, 90);
    distance.style('transform: rotate(' + -90 + 'deg);');
    distance.style('color', '#ff0000');

    velocity = createSlider(-1048576, 1048576, 0);
    velocity.position(180, 90);
    velocity.style('transform: rotate(' + -90 + 'deg);');

    strength = createSlider(0, 16383, 0);
    strength.position(340, 90);
    strength.style('transform: rotate(' + -90 + 'deg);');

    temperature = createSlider(-819, 819, 0);
    temperature.position(500, 90);
    temperature.style('transform: rotate(' + -90 + 'deg);');

}

function draw() {
    background(180);

    tick_count += 1;

    const distance_value = distance.value();
    const velocity_value = velocity.value();
    const strength_value = strength.value();
    const temperature_value = temperature.value();

    noStroke();
    textFont('Helvetica');
    text(distance_value, 79, 20);
    text(velocity_value, 238, 20);
    text(strength_value, 398, 20);
    text(temperature_value, 558, 20);

    text('Distance', 75, 175);
    text('Velocity', 240, 175);
    text('Strength', 400, 175);
    text('Temperature', 565, 175);

    stroke(100);
    // line(x1, y1, x2, y2);
    line(160, 10, 160, 195);
    line(330, 10, 330, 195);
    line(470, 10, 470, 195);
    line(25, 195, 625, 195);
    
    const mode_value = set_Mode();
    const format_value = formatRadio.value();

    if (tick_count == FREQ) {
        // Send data every FREQ ticks
        send_data(distance_value,
            velocity_value,
            strength_value,
            temperature_value,
            mode_value,
            format_value);
        tick_count = 0;
    }
}

function set_Mode() {
    if (Strength_Flag & Temperature_Flag) {
        return 3;
    }
    if (Strength_Flag) {
        return 1;
    }
    if (Temperature_Flag) {
        return 2;
    }
    return 0
}

function set_VM() {
    if (this.checked()) {
        VM_Mode = true;
    } else {
        VM_Mode = false;
    }
}

function set_ST() {
    if (this.checked()) {
        Strength_Flag = true;
    } else {
        Strength_Flag = false;
    }
}
function set_TEMP() {
    if (this.checked()) {
        Temperature_Flag = true;
    } else {
        Temperature_Flag = false;
    }
}

// Function for sending to the socket
function send_data(_distance, _velocity, _strength, _temperature, _mode, _format) {
    // We are sending!
    var data = {
        VM: VM_Mode,
        mode: _mode,
        format: _format,
        dis: _distance,
        vel: _velocity,
        stre: _strength,
        temp: _temperature
    };
    // sends that object to the socket
    socket.emit('data', data);
}
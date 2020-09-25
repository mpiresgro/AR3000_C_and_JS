
var output;
var Common = require('./common');

module.exports = {
    encode_decimal_data: encode_decimal_data
};

function encode_decimal_data(data) {
    function add_signal_and_point(value, negative, point_position) {
        // Add '.' -> 46
        value.splice(point_position, 0, 46);
        if (negative) {
            // Add '-' -> 45
            value.unshift(45);
        }
        else {
            // Add ' ' -> 32
            value.unshift(32);
        }
        return value
    }

    output = [];

    // 'D' -> 32 
    var firstChar = 68;

    // ---------------------- Distance ------------------------
    var distance_negative;
    if (data.dis < 0) {
        data.dis *= -1;
        distance_negative = true;
    } else {
        distance_negative = false;
    }

    var distance = ('0000000' + data.dis).slice(-7).split('').map(Common.get_ascii);
    distance = add_signal_and_point(distance, distance_negative, 4);

    // ---------------------- Velocity ------------------------
    var velocity_negative;
    if (data.vel < 0) {
        data.vel *= -1;
        velocity_negative = true;
    } else {
        velocity_negative = false;
    }

    var velocity = ('0000000' + data.vel).slice(-7).split('').map(Common.get_ascii);
    velocity = add_signal_and_point(velocity, velocity_negative, 4);

    // ---------------------- Strength ------------------------
    var strength = ('00000' + data.stre).slice(-5).split('').map(Common.get_ascii);

    // ---------------------- Temperature ------------------------
    var temperature_negative;
    if (data.temp < 0) {
        data.temp *= -1;
        temperature_negative = true;
    } else {
        temperature_negative = false;
    }

    var temperature = ('000' + data.temp).slice(-3).split('').map(Common.get_ascii);
    temperature = add_signal_and_point(temperature, temperature_negative, 2);

    // ---------------------- Create Ouput ------------------------
    if (data.VM) {
        // Add ' ' -> 32
        velocity.push(32);
        output = velocity.concat(distance);
    }
    else {
        output = distance;
    }

    switch (data.mode) {
        case 0:
            // Nothing to be done here!
            break;
        case 1:
            output.push(32);
            output = output.concat(strength);
            break;
        case 2:
            output.push(32);
            output = output.concat(temperature);
            break;
        case 3:
            output.push(32);
            output = output.concat(strength);
            output.push(32);
            output = output.concat(temperature);
            break;
        default:
            console.log("Unidentified Mode!");
    }
    output.unshift(firstChar);
    output.push(Common.TERMINATOR);

    return output;
}


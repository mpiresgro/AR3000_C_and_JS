
var output;
var Common = require('./common');

module.exports = {
    encode_binary_data: encode_binary_data
};

function encode_binary_data(data) {
    function dec2bin(dec, nbits) {
        let num = (dec >>> 0).toString(2);
        if (dec < 0) {
            return num.slice(32 - nbits);
        } else {
            while (num.length < nbits) {
                num = '0' + num;
            }
            return num;
        }
    }

    output = [];

    // ---------------------- Distance ------------------------
    var distance = dec2bin(data.dis, 21)
    distance = distance.match(/.{1,7}/g);
    distance[0] = parseInt('1' + distance[0], 2);
    distance[1] = parseInt('0' + distance[1], 2);
    distance[2] = parseInt('0' + distance[2], 2);

    // ---------------------- Velocity ------------------------
    var velocity = dec2bin(data.vel, 21)
    velocity = velocity.match(/.{1,7}/g);
    velocity[0] = parseInt('1' + velocity[0], 2);
    velocity[1] = parseInt('0' + velocity[1], 2);
    velocity[2] = parseInt('0' + velocity[2], 2);

    // ---------------------- Strength ------------------------
    var strength = dec2bin(data.stre, 14)
    strength = strength.match(/.{1,7}/g);
    strength[0] = parseInt('0' + strength[0], 2);
    strength.pop() // removes last byte from strength

    // ---------------------- Temperature ------------------------
    var temperature = dec2bin(data.temp, 14)
    temperature = temperature.match(/.{1,7}/g);
    temperature[0] = parseInt('1' + temperature[0], 2);
    temperature[1] = parseInt('0' + temperature[1], 2);


    // ---------------------- Create Ouput ------------------------
    if (data.VM) {
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
            output = output.concat(strength);
            break;
        case 2:
            output = output.concat(temperature);
            break;
        case 3:
            output = output.concat(strength);
            output = output.concat(temperature);
            break;
        default:
            console.log("Unidentified Mode!");
    }
    output.push(Common.TERMINATOR);

    return output;
}

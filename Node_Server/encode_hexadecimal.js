
var output;
var Common = require('./common');

module.exports = {
    encode_hexadecimal_data: encode_hexadecimal_data
};

function encode_hexadecimal_data(data) {

    function decimal2Hex(decimal, nbits) {
        var hex_num;
        let max_map = {
            16: 0xFFFF,
            24: 0xFFFFFF,
        };
        if (decimal < 0) {
            decimal = max_map[nbits] + decimal + 1;
            hex_num = decimal.toString(16).toUpperCase();
        }
        else {
            hex_num = decimal.toString(16).toUpperCase();
            // Left padding with zeros
            while (hex_num.length < nbits / 4) {
                hex_num = '0' + hex_num;
            }
        }
        return hex_num;
    }
    output = [];

    // 'H' -> 72
    var firstChar = 72;
    distance = decimal2Hex(data.dis, 24).split('').map(Common.get_ascii);
    velocity = decimal2Hex(data.vel, 24).split('').map(Common.get_ascii);
    strength = decimal2Hex(data.stre, 16).split('').map(Common.get_ascii);
    temperature = decimal2Hex(data.temp, 16).split('').map(Common.get_ascii);

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
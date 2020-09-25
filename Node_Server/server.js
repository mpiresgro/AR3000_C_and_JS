var output;

function handle_data(data) {

    var decimal = require('./encode_decimal');
    var binary = require('./encode_binary');
    var hexadecimal = require('./encode_hexadecimal');

    output = [];
    switch (data.format) {
        case 'Decimal':
            output = decimal.encode_decimal_data(data);
            break;
        case 'Hexadecimal':
            output = hexadecimal.encode_hexadecimal_data(data);
            break;
        case 'Binary':
            output = binary.encode_binary_data(data);
            break;
        default:
            console.log("Unidentified Format!");
    }

    if (output != undefined) {
        // console.log("Sending > ", output.length - 1);
        console.log("Sending > ", output.toString());
    }
}

// ----------------------------------------- P5 JS CLIENT
// Using express: http://expressjs.com/
var express = require('express');
// Create the app
var app = express();

// Set up the server
// process.env.PORT is related to deploying on heroku
var server = app.listen(8000, listen);

// This call back just tells us that the server has started
function listen() {
    var host = server.address().address;
    var port = server.address().port;
    console.log('Server 1 is listening @ ' + port);
}

app.use(express.static('public'));

// WebSocket Portion
// WebSockets work with the HTTP server
var io = require('socket.io')(server);

// Register a callback function to run when we have an individual connection
// This is run for each individual user that connects
io.sockets.on('connection',
    // We are given a websocket object in our function
    function (socket) {

        console.log("We have a new client: " + socket.id);

        // When this user emits, client side: socket.emit('otherevent',some data);
        socket.on('data',
            function (data) {
                // Data comes in as whatever was sent, including objects
                handle_data(data);
                // Send it to all other clients
                socket.broadcast.emit('data', data);
                // This is a way to send to everyone including sender
                // io.sockets.emit('message', "this goes to everyone");
            }
        );
        socket.on('disconnect', function () {
            console.log("Client has disconnected");
        });
    }
);


// ----------------------------------------- C CLIENT

const port = 4000;
var net = require('net');
var server_2 = net.createServer(function (connection) {
    console.log('client connected');

    console.log("We have a new client: " + connection.id);

    connection.on('data', function (data) {
        // data = data.toString();
        // console.log('Client sended the folowing string: ' + data);
        // Creates a new Buffer containing the UTF-8 bytes of the string 'buffer'.
        const buf = Buffer.from(output)
        connection.write(buf);
        // console.log('Sended responst to client');
        // connection.end();
        // console.log('Disconnected the client.');
    });

    connection.on('close', function () {
        console.log('client disconnected');
    });
});

server_2.listen(port, function () {
    console.log('Server 2 is listening @ ' + port);
}); 

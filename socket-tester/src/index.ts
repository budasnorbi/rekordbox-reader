import Net from 'net';
import udp from "dgram";
import { OPCODES, TCP_OPCODES, UDP_OPCODES } from './odcodes';

const UDP_PORT = 54000;
const TCP_PORT = 54001;

// UDP SERVER
const udpServer = udp.createSocket("udp4");

udpServer.on("error", function (error) {
  console.log("Error: " + error);
  udpServer.close();
});

udpServer.on("message", function (buffer) {
    const opCode: UDP_OPCODES = buffer.readUint8(0);
    let value:number|undefined;

    if(opCode === OPCODES.D1_CHANNEL_FADER || opCode === OPCODES.D2_CHANNEL_FADER){
        // D1 D2 CHANNEL FADER
        value = buffer.readDoubleLE(1)
    } else if(opCode === OPCODES.D1_CFX || opCode === OPCODES.D2_CFX){
        // D1 D2 CFX
        value = buffer.readDoubleLE(1)
    } else if(opCode === OPCODES.D1_LOW_FILTER || opCode === OPCODES.D2_LOW_FILTER){
        // D1 D2 LOW FILTER
        value = buffer.readDoubleLE(1)
    } else if(opCode === OPCODES.D1_MID_FILTER || opCode === OPCODES.D2_MID_FILTER){
        // D1 D2 MID FILTER
        value = buffer.readDoubleLE(1)
    } else if(opCode === OPCODES.D1_HIGH_FILTER || opCode === OPCODES.D2_HIGH_FILTER){
        // D1 D2 HIGH FILTER
        value = buffer.readDoubleLE(1)
    } else if(opCode === OPCODES.D1_TRIM || opCode === OPCODES.D2_TRIM){
        // D1 D2 TRIM 
        value = buffer.readDoubleLE(1)
    } else if(opCode === OPCODES.D1_TEMPO || opCode === OPCODES.D2_TEMPO){
        // D1 D2 TEMPO 
        value = buffer.readDoubleLE(1)
    }else if(opCode === OPCODES.D1_CURRENT_TIME || opCode === OPCODES.D2_CURRENT_TIME){
        // D1 D2 CURRENT TIME 
        value = buffer.readDoubleLE(1)
    }

    console.log("[UDP]",opCode, value);
    
});

udpServer.bind(UDP_PORT,() => {
    console.log(`[UDP SERVER] listening on port ${UDP_PORT}`);
});

// TCP SERVER
const tcpServer = new Net.Server();

tcpServer.on('connection', function(socket) {
    console.log('A new connection has been established.');

    // The server can also receive data from the client by reading from its socket.
    socket.on('data', function(buffer) {
        const opCode: TCP_OPCODES = buffer.readUint8(0);
        let value:bigint| number|undefined;
    
        if(opCode === OPCODES.D1_IS_PLAYING || opCode === OPCODES.D2_IS_PLAYING){
            // D1 D2 IS PLAYING
            value = buffer.readUint8(1)
        } else if(opCode === OPCODES.D1_SONG_ID || opCode === OPCODES.D2_SONG_ID){
            // D1 D2 SONG ID
            value = buffer.readUint32LE(1);
        } 
    
        console.log("[TCP]",opCode, value);
    });

    // When the client requests to end the TCP connection with the server, the server
    // ends the connection.
    socket.on('end', function() {
        console.log('Closing connection with the client');
    });

    // Don't forget to catch error, for your own sake.
    socket.on('error', function(err) {
        console.log(`Error: ${err}`);
    });
});

tcpServer.listen(TCP_PORT, function() {
    console.log(`[TCP SERVER] listening on port ${TCP_PORT}`);
});
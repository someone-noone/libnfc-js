'use strict'

const {NFC, NFCDevice} = require('./index')

let nfc = new NFC();
console.log(nfc.listDevices())
nfc.close();

let nfcDevice = new NFCDevice();
nfcDevice.open("pn532_uart:/dev/tty.usbserial");

nfcDevice.poll((err, result) => {
    if (err)
        throw err;
    console.log(result);


    nfcDevice.transceive(Buffer.from([
        0x00, // Class
        0xa4, // INS
        0x04, // P1
        0x00, // P2
        0x07 // Le
    ]), (err, result) => {
        if (err)
            throw err;
        console.log(result)

        nfcDevice.release((err) => {
            if (err)
                throw err;
            console.log("device released");
        })
    });
})

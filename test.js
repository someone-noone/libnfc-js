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

    nfcDevice.release((err) => {
        if (err)
            throw err;
        console.log("device released");
    })
})

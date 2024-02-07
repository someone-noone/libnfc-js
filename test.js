'use strict'

const {NFC, NFCReader} = require('./index')

// Core API:
const nfc = new NFC();
const devices = nfc.listDevices();
if(!devices.length) {
    throw new Error(`No NFC devices found`);
}
console.log('Devices found: ', devices);

// Reader API:
const nfcReader = new NFCReader();
console.log('Connecting to first device...', devices[0]);
nfcReader.open(devices[0]); // or nfcReader.open(); to open the default device

// triggered if polling has failed
nfcReader.on('error', err => {
    throw err;
});

nfcReader.poll(async (card) => {
    console.log('Found NFC card!', card);
    // Put tranceive commands here...
    await nfcReader.release();
    console.log('Card released');
}).catch((e) => {
    console.log('Error during polling: ', e);
});
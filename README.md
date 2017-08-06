# libnfc-js
A simple libnfc API to interact with NFC tags.  

## Install
```javascript
    npm i -S libnfc-js
```

## System requirements

* Linux or macOS system (Windows support is not yet implemented)
* libnfc MUST be installed in the system:
  * macOS: brew install libnfc
  * linux(debian): apt-get install libnfc
  * this binding is looking for libnfc files in /usr/lib or /usr/local/lib
* Your NFC device MUST be configured in nfc.conf file (location depends on your libnfc installation)

Please ensure that you're able to communicate with your NFC device with nfc-list or nfc-poll commands, before you start using this package.

## Usage

See full example in test.js

#### List avaliable devices in the system

```javascript
const {NFC, NFCReader} = require('libnfc-js')

// Core API:
let nfc = new NFC();
console.log(nfc.listDevices())
nfc.close();
```

#### Open reader

```javascript
let nfcReader = new NFCReader();
nfcReader.open();
```

or (if you have more than 1 device in the system)

```javascript
let nfcReader = new NFCReader();
nfcReader.open("pn532_uart:/dev/tty.usbserial");
```

#### Polling cards and sending data

``` javascript
nfcReader.poll(); // polls for the next card
nfcReader.on('card', card => {
    // Sending data:
    let result = await nfcReader.transceive(Buffer.from([0]));
    console.log(result);

    await nfcReader.release();
    console.log('card released');

    nfcReader.poll(); // polls for the next card
});

// triggered if polling has failed
nfcReader.on('error', err => {
    throw err;
})
```

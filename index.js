'use strict'
const EventEmitter = require('events');
const binding = require('bindings')('nfc-binding');
const { promisify } = require('util');

class NFCReader extends EventEmitter {
    constructor() {
        super();
        this._nfc = new binding.NFCReaderRaw();
        this.transceive = promisify(this._nfc.transceive).bind(this._nfc);
        this.release = promisify(this._nfc.release).bind(this._nfc);
        this.pollAsync = promisify(this._nfc.poll).bind(this._nfc);
        this.close = this._nfc.close.bind(this._nfc);
        this.open = this._nfc.open.bind(this._nfc);
    }

    async poll(onCard) {
        let polling = true;
    
        while(polling) {
            try {
                const card = await this.pollAsync();
                await onCard(card);
            } catch (e) {
                if (e.message !== "NFC_ECHIP" && e.message !== "Unknown error") {
                    this.emit('error', e);
                    polling = false;
                }
            }
        }
    }
}

module.exports = {
    NFC: binding.NFC,
    NFCReader
}

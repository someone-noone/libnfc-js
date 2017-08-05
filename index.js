'use strict'
const EventEmitter = require('events');
const binding = require('bindings')('nfc-binding');
const Promise = require('bluebird');

class NFCReader extends EventEmitter {
    constructor() {
        super();
        this._nfc = new binding.NFCReaderRaw();
    }

    open() {
        return this._nfc.open();
    }

    close() {
        return this.close();
    }

    transceive(data, timeout) {
        if (timeout)
            return Promise.fromCallback(cb => this._nfc.transceive(data, timeout, cb));
        else
            return Promise.fromCallback(cb => this._nfc.transceive(data, cb));
    }

    release() {
        return Promise.fromCallback(cb => this._nfc.release(cb));
    }

    poll() {
        return Promise.fromCallback(cb => this._nfc.poll(cb))
            .then(card => this.emit('card', card))
            .catch(e => {
                if (e.message != "NFC_ECHIP")
                    this.emit('error', e)
                else
                    return this.poll();
            });
    }
}

module.exports = {
    NFC: binding.NFC,
    NFCReader
}

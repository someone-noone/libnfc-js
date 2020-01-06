'use strict';
const EventEmitter = require('events');
const binding = require('bindings')('nfc-binding');
const Promise = require('bluebird');

const NFC_PROPERTY = Object.freeze({
  NP_TIMEOUT_COMMAND: 0,
  NP_TIMEOUT_ATR: 1,
  NP_TIMEOUT_COM: 2,
  NP_HANDLE_CRC: 3,
  NP_HANDLE_PARITY: 4,
  NP_ACTIVATE_FIELD: 5,
  NP_ACTIVATE_CRYPTO1: 6,
  NP_INFINITE_SELECT: 7,
  NP_ACCEPT_INVALID_FRAMES: 8,
  NP_ACCEPT_MULTIPLE_FRAMES: 9,
  NP_AUTO_ISO14443_4: 10,
  NP_EASY_FRAMING: 11,
  NP_FORCE_ISO14443_A: 12,
  NP_FORCE_ISO14443_B: 13,
  NP_FORCE_SPEED_106: 14,
});

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
      return Promise.fromCallback(cb =>
        this._nfc.transceive(data, timeout, cb),
      );
    else return Promise.fromCallback(cb => this._nfc.transceive(data, cb));
  }

  release() {
    return Promise.fromCallback(cb => this._nfc.release(cb));
  }

  poll() {
    return Promise.fromCallback(cb => this._nfc.poll(cb))
      .then(card => this.emit('card', card))
      .catch(e => {
        if (e.message != 'NFC_ECHIP' && e.message != 'Unknown error')
          this.emit('error', e);
        else return this.poll();
      });
  }

  setProperty(property, enable) {
    return this._nfc.setProperty(property, enable);
  }
}

module.exports = {
  NFC: binding.NFC,
  NFCReader,
  NFC_PROPERTY,
};

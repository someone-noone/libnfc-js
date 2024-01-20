#include "nfc-transceive.h"
#include "tools.h"

using v8::Function;
using v8::Local;
using v8::Number;
using v8::Value;
using v8::Object;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;
using Nan::Error;
using Nan::NewBuffer;

using namespace std;

NFCTransceive::NFCTransceive(Callback *cb, nfc_device *device, uint8_t *data,
    const size_t &size, const int& timeout)
:AsyncWorker(cb), _pnd(device), _timeout(timeout), _has_error(false) {
    _data.assign((char*)data, size);
}

void NFCTransceive::Execute() {
    _recv_size = nfc_initiator_transceive_bytes(_pnd,
        (uint8_t *)_data.c_str(), _data.length(),
        _recv_data, MAX_RECV_SIZE, _timeout);

    if (_recv_size < 0) {
        _has_error = true;
        _error.assign(GetLibNFCError(_recv_size));
    }
}

void NFCTransceive::HandleOKCallback() {
    HandleScope scope;

    v8::Local<v8::Value> argv[] = {
        Nan::Null(), 
        Nan::Null()
    };

    if (!_has_error) {
        // Allocate a new buffer for JavaScript
        char* bufferCopy = new char[_recv_size];
        memcpy(bufferCopy, _recv_data, _recv_size);

        // Create a Node.js buffer from the copied data
        v8::Local<v8::Object> nodeBuffer = Nan::CopyBuffer(bufferCopy, _recv_size).ToLocalChecked();

        // Free the temporary buffer copy now that it's been copied into the Node.js buffer
        delete[] bufferCopy;

        argv[1] = nodeBuffer;
    } else {
        argv[0] = Error(_error.c_str());
    }

    callback->Call(2, argv);
}

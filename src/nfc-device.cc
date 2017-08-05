#include "nfc-device.h"
#include "nfc-poll.h"
#include "nfc-release.h"
#include "nfc-transceive.h"

using Nan::HandleScope;
using Nan::Callback;
using Nan::To;
using v8::Local;
using v8::Array;
using v8::String;
using v8::Function;
using v8::Number;

NAN_METHOD(NFCDevice::New) {
    HandleScope scope;
    assert(info.IsConstructCall());
    NFCDevice* device = new NFCDevice();
    device->Wrap(info.This());
    device->_opened = false;

    nfc_init(&device->_context);
    if (device->_context == NULL)
        return Nan::ThrowError("nfc_init() failed");

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NFCDevice::Close) {
    HandleScope scope;
    NFCDevice* device = ObjectWrap::Unwrap<NFCDevice>(info.This());

    if (device->_opened)
        nfc_close(device->_pnd);

    nfc_exit(device->_context);
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NFCDevice::Open) {
    HandleScope scope;
    NFCDevice* device = ObjectWrap::Unwrap<NFCDevice>(info.This());

    if (info.Length() > 0) {
        nfc_connstring devicePath;
        String::Utf8Value utfPath(info[0]->ToString());
        snprintf(devicePath, sizeof devicePath, "%s", *utfPath);
        device->_pnd = nfc_open(device->_context, devicePath);
    }
    else
        device->_pnd = nfc_open(device->_context, NULL);

    if (device->_pnd == NULL)
        return Nan::ThrowError("nfc_open() failed");

    if (nfc_initiator_init(device->_pnd) < 0)
        return Nan::ThrowError("nfc_initiator_init()");

    device->_opened = true;
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NFCDevice::Poll) {
    HandleScope scope;
    NFCDevice* device = ObjectWrap::Unwrap<NFCDevice>(info.This());

    Callback *callback = new Callback(info[0].As<Function>());
    AsyncQueueWorker(new NFCPoll(callback, device->_pnd));
}

NAN_METHOD(NFCDevice::Release) {
    HandleScope scope;
    NFCDevice* device = ObjectWrap::Unwrap<NFCDevice>(info.This());

    Callback *callback = new Callback(info[0].As<Function>());
    AsyncQueueWorker(new NFCRelease(callback, device->_pnd));
}

NAN_METHOD(NFCDevice::Transceive) {
    HandleScope scope;
    NFCDevice* device = ObjectWrap::Unwrap<NFCDevice>(info.This());

    uint8_t* data = (uint8_t *)node::Buffer::Data(info[0]);
    size_t size = node::Buffer::Length(info[0]);

    Callback *callback;
    int timeout = 1000;

    if (info[1]->IsFunction())
        callback = new Callback(info[1].As<Function>());
    else {
        timeout = To<int32_t>(info[1].As<Number>()).FromJust();
        callback = new Callback(info[2].As<Function>());
    }
    AsyncQueueWorker(new NFCTransceive(callback, device->_pnd, data, size, timeout));
}

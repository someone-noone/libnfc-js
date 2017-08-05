#include "nfc-device.h"
#include "nfc-poll.h"
#include "nfc-release.h"

using Nan::HandleScope;
using Nan::Callback;
using v8::Local;
using v8::Array;
using v8::String;
using v8::Function;

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
    Nan::HandleScope scope;
    NFCDevice* device = ObjectWrap::Unwrap<NFCDevice>(info.This());

    if (device->_opened)
        nfc_close(device->_pnd);

    nfc_exit(device->_context);
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NFCDevice::Open) {
    Nan::HandleScope scope;
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
    Nan::HandleScope scope;
    NFCDevice* device = ObjectWrap::Unwrap<NFCDevice>(info.This());

    Callback *callback = new Callback(info[0].As<Function>());
    AsyncQueueWorker(new NFCPoll(callback, device->_pnd));
}

NAN_METHOD(NFCDevice::Release) {
    Nan::HandleScope scope;
    NFCDevice* device = ObjectWrap::Unwrap<NFCDevice>(info.This());

    Callback *callback = new Callback(info[0].As<Function>());
    AsyncQueueWorker(new NFCRelease(callback, device->_pnd));
}

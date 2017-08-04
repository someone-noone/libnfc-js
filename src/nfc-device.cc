#include "nfc-device.h"

using Nan::HandleScope;

using v8::Local;
using v8::Array;
using v8::String;

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

    device->_opened = true;
    info.GetReturnValue().Set(info.This());
}

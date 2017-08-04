#include "nfc.h"

using Nan::HandleScope;

using v8::Local;
using v8::Array;
using v8::String;

NAN_METHOD(NFC::New) {
    HandleScope scope;
    assert(info.IsConstructCall());
    NFC* nfc = new NFC();
    nfc->Wrap(info.This());

    nfc_init(&nfc->_context);
    if (nfc->_context == NULL)
        return Nan::ThrowError("nfc_init() failed");

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NFC::Close) {
    Nan::HandleScope scope;
    NFC* nfc = ObjectWrap::Unwrap<NFC>(info.This());

    nfc_exit(nfc->_context);
    info.GetReturnValue().Set(info.This());
}

#define MAX_DEVICE_COUNT 32
NAN_METHOD(NFC::ListDevices) {
    Nan::HandleScope scope;
    NFC* nfc = ObjectWrap::Unwrap<NFC>(info.This());

    nfc_connstring connstrings[MAX_DEVICE_COUNT];
    size_t szDeviceFound = nfc_list_devices(nfc->_context, connstrings, MAX_DEVICE_COUNT);
    Local<Array> result = Nan::New<Array>(szDeviceFound);

    for (size_t i = 0; i < szDeviceFound; ++i)
        Nan::Set(result, i, Nan::New<String>(connstrings[i]).ToLocalChecked());

    info.GetReturnValue().Set(result);
}

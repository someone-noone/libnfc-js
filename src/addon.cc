#include <nan.h>
#include "nfc.h"
#include "nfc-reader.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Local;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_MODULE_INIT(InitAll) {
    Local<FunctionTemplate> tpl = New<FunctionTemplate>(NFC::New);
    tpl->SetClassName(Nan::New("NFC").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    SetPrototypeMethod(tpl, "close", NFC::Close);
    SetPrototypeMethod(tpl, "listDevices", NFC::ListDevices);

    Set(target, New("NFC").ToLocalChecked(), tpl->GetFunction());

    Local<FunctionTemplate> tpl2 = New<FunctionTemplate>(NFCReader::New);
    tpl2->SetClassName(Nan::New("NFCDevice").ToLocalChecked());
    tpl2->InstanceTemplate()->SetInternalFieldCount(1);

    SetPrototypeMethod(tpl2, "close", NFCReader::Close);
    SetPrototypeMethod(tpl2, "open", NFCReader::Open);
    SetPrototypeMethod(tpl2, "poll", NFCReader::Poll);
    SetPrototypeMethod(tpl2, "release", NFCReader::Release);
    SetPrototypeMethod(tpl2, "transceive", NFCReader::Transceive);
    SetPrototypeMethod(tpl2, "setProperty", NFCReader::SetProperty);

    Set(target, New("NFCReaderRaw").ToLocalChecked(), tpl2->GetFunction());
}

NODE_MODULE(addon, InitAll)

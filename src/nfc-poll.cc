#include "nfc-poll.h"

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

using namespace std;

NFCPoll::NFCPoll(Callback *cb, nfc_device *device)
:AsyncWorker(cb), _pnd(device), _has_error(false) {

}

void NFCPoll::Execute() {
    const uint8_t uiPollNr = 2;
    const uint8_t uiPeriod = 2;
    const nfc_modulation nmModulations[5] = {
        { .nmt = NMT_ISO14443A, .nbr = NBR_106 },
        { .nmt = NMT_ISO14443B, .nbr = NBR_106 },
        { .nmt = NMT_FELICA, .nbr = NBR_212 },
        { .nmt = NMT_FELICA, .nbr = NBR_424 },
        { .nmt = NMT_JEWEL, .nbr = NBR_106 },
    };
    const size_t szModulations = 5;
    int res = 0;

    if ((res = nfc_initiator_poll_target(_pnd, nmModulations, szModulations, uiPollNr, uiPeriod, &_nt))  < 0) {
        _has_error = true;
        _error.assign("nfc_initiator_poll_target() error");
        return;
    }

    if (res <= 0) {
        _has_error = true;
        _error.assign("no target found");
        return;
    }
}

const char* GetModulationType(const nfc_target &nt) {
    switch (nt.nm.nmt) {
        case NMT_ISO14443A:
            return "NMT_ISO14443A";
        case NMT_JEWEL:
            return "NMT_JEWEL";
        case NMT_ISO14443B:
            return "NMT_ISO14443B";
        case NMT_ISO14443BI:
            return "NMT_ISO14443BI";
        case NMT_ISO14443B2SR:
            return "NMT_ISO14443B2SR";
        case NMT_ISO14443B2CT:
            return "NMT_ISO14443B2CT";
        case NMT_FELICA:
            return "NMT_FELICA";
        case NMT_DEP:
            return "NMT_DEP";
        default:
            return "Unknown";
    }
}

const char *GetBaudRate(const nfc_target &nt) {
    switch (nt.nm.nbr) {
        case NBR_UNDEFINED:
            return "NBR_UNDEFINED";
        case NBR_106:
            return "NBR_106";
        case NBR_212:
            return "NBR_212";
        case NBR_424:
            return "NBR_424";
        case NBR_847:
            return "NBR_847";
    }
}

void NFCPoll::HandleOKCallback() {
    HandleScope scope;

    Local<Value> argv[] = {
        Null()
      , Null()
    };

    if(_has_error)
        argv[0] = Error(_error.c_str());
    else {
        Local<Object> obj = New<Object>();

        obj->Set(New("modulationType").ToLocalChecked(), New(GetModulationType(_nt)).ToLocalChecked());
        obj->Set(New("baudRate").ToLocalChecked(), New(GetBaudRate(_nt)).ToLocalChecked());
        argv[1] = obj;
    }

    callback->Call(2, argv);
}

#include "nfc-release.h"
#include <unistd.h>

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

NFCRelease::NFCRelease(Callback *cb, nfc_device *device)
:AsyncWorker(cb), _pnd(device) {

}

void NFCRelease::Execute() {
    while (nfc_initiator_target_is_present(_pnd, NULL) == 0)
        usleep(50*1000);
}

void NFCRelease::HandleOKCallback() {
    HandleScope scope;

    v8::Local<v8::Value> argv[] = {
        Nan::Null(), 
    };

    callback->Call(1, argv);
}

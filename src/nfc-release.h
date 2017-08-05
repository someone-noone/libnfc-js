#ifndef NFC_RELEASE__H
#define NFC_RELEASE__H

#include <nan.h>
#include <nfc/nfc.h>

class NFCRelease : public Nan::AsyncWorker {
    public:
        NFCRelease(Nan::Callback *cb, nfc_device *device);

        void Execute();
        void HandleOKCallback();
    private:
        nfc_device *_pnd;
};

#endif

#ifndef NFC_DEVICE__H
#define NFC_DEVICE__H

#include <nan.h>
#include <nfc/nfc.h>

class NFCDevice  : public Nan::ObjectWrap {
    public:
        static NAN_METHOD(New);
        static NAN_METHOD(Open);
        static NAN_METHOD(Close);
        static NAN_METHOD(Poll);
    private:
        nfc_context *_context;
        nfc_device *_pnd;
        bool _opened;
};

#endif

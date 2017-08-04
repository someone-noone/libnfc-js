#ifndef NFC_HELPER__H
#define NFC_HELPER__H

#include <nan.h>
#include <nfc/nfc.h>

class NFC : public Nan::ObjectWrap {
    public:
        static NAN_METHOD(New);
        static NAN_METHOD(Close);
        static NAN_METHOD(ListDevices);
    private:
        nfc_context *_context;
};

#endif

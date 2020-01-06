#ifndef NFC_DEVICE__H
#define NFC_DEVICE__H

#include <nan.h>
#include <nfc/nfc.h>

class NFCReader  : public Nan::ObjectWrap {
    public:
        static NAN_METHOD(New);
        static NAN_METHOD(Open);
        static NAN_METHOD(Close);
        static NAN_METHOD(Poll);
        static NAN_METHOD(Release);
        static NAN_METHOD(Transceive);
        static NAN_METHOD(SetProperty);
    private:
        nfc_context *_context;
        nfc_device *_pnd;
        bool _opened;
};

#endif

#ifndef NFC_TRANSCEIVE__H
#define NFC_TRANSCEIVE__H

#include <nan.h>
#include <nfc/nfc.h>
#include <string>

const size_t MAX_RECV_SIZE = 64*1024;

class NFCTransceive : public Nan::AsyncWorker {
    public:
        NFCTransceive(Nan::Callback *cb, nfc_device *device,
            uint8_t *data, const size_t& size, const int& timeout);

        void Execute();
        void HandleOKCallback();
    private:
        nfc_device *_pnd;
        std::string _data;
        int _timeout;
        bool _has_error;
        std::string _error;
        uint8_t _recv_data[MAX_RECV_SIZE];
        int _recv_size;
};

#endif

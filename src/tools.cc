#include "tools.h"
#include <nfc/nfc.h>

const char* GetLibNFCError(const int& error_code) {
    switch (error_code) {
        case NFC_EIO:
            return "NFC_EIO";
        case NFC_EINVARG:
            return "NFC_EINVARG";
        case NFC_EDEVNOTSUPP:
            return "NFC_EDEVNOTSUPP";
        case NFC_ENOTSUCHDEV:
            return "NFC_ENOTSUCHDEV";
        case NFC_EOVFLOW:
            return "NFC_EOVFLOW";
        case NFC_ETIMEOUT:
            return "NFC_ETIMEOUT";
        case NFC_EOPABORTED:
            return "NFC_EOPABORTED";
        case NFC_ENOTIMPL:
            return "NFC_ENOTIMPL";
        case NFC_ETGRELEASED:
            return "NFC_ETGRELEASED";
        case NFC_ERFTRANS:
            return "NFC_ERFTRANS";
        case NFC_EMFCAUTHFAIL:
            return "NFC_EMFCAUTHFAIL";
        case NFC_ESOFT:
            return "NFC_ESOFT";
        case NFC_ECHIP:
            return "NFC_ECHIP";
        default:
            return "Unknown error";
    }
}

#ifndef TOKEN_H
#define TOKEN_H

#include <QByteArray>

enum class TOKEN : int {
    TOKEN_ERROR = 0,
    TOKEN_ABORT = 1,
    TOKEN_READY = 2,

    // server reviced
    TOKEN_UPLOAD      = 10,
    TOKEN_DOWNLOAD    = 11,
    TOKEN_DELETE      = 12,

    // client recived
    TOKEN_UPLOADED    = 21,
    TOKEN_DELETED     = 22
};

inline TOKEN toToken(QByteArray data){ return *(TOKEN*)data.data(); }

#endif // TOKEN_H

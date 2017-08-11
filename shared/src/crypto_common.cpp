//
// Created by lol on 8/5/2017.
//
#include <crypto_common.h>
#include <openssl/err.h>

namespace m2
{
namespace crypto
{
namespace common
{
    CryptoError::CryptoError(const char *s) : std::runtime_error(s) {}
    CryptoError::CryptoError(const std::string &s) : std::runtime_error(s) {}

    OpenSSL_CryptoError::OpenSSL_CryptoError(const std::string &s) : CryptoError(s+makeOpenSSLErrString(false)) {}

    OpenSSL_CryptoError::OpenSSL_CryptoError(const char *s) : CryptoError(std::string(s)+makeOpenSSLErrString(false)) {

    }

    OpenSSL_CryptoError::OpenSSL_CryptoError(): CryptoError(makeOpenSSLErrString(true))
    {

    }

    std::string OpenSSL_CryptoError::   makeOpenSSLErrString(bool prefix) {
            std::string err = prefix ? "Error occured in OpenSSL context: " : "";
        char buf[1024];
        auto code = ERR_get_error();
        ERR_error_string(code, buf);
        return err + buf;
    }
}
}
}

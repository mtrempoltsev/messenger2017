//
// Created by lol on 8/5/2017.
//

#ifndef M2CRYPTO_COMMON_H
#define M2CRYPTO_COMMON_H

#include <boost/uuid/uuid.hpp>
#include <stdexcept>
#include <string>

namespace m2
{
namespace crypto
{
namespace common
{
    class CryptoError : public std::runtime_error {
    public:
        explicit CryptoError(const char *s);
        explicit CryptoError(const std::string &s);

    };

    class OpenSSL_CryptoError: public CryptoError {
    public:
        explicit OpenSSL_CryptoError(const char *s);
        explicit OpenSSL_CryptoError(const std::string &s);
        explicit OpenSSL_CryptoError();

        static std::string makeOpenSSLErrString(bool prefix);
    };

    class ICryptoProvider {
    public:
        virtual std::string encrypt(const std::string &) const = 0;
        virtual std::string decrypt(const std::string &) const = 0;
        virtual std::string encrypt_to_b64(const std::string &string) const = 0;
        virtual std::string decrypt_from_b64(const std::string &string) const = 0;

    };
}
}
}


#endif //M2CRYPTO_COMMON_H

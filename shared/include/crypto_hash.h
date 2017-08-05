//
// Created by lol on 8/5/2017.
//

#ifndef M2CRYPTO_HASH_H
#define M2CRYPTO_HASH_H

#include <string>
#include <openssl/sha.h>

namespace m2
{
namespace crypto
{
namespace common
{
    class IHashProvider {
    public:
        virtual size_t size() const = 0;
        virtual std::string hash(const std::string &) const = 0;
    };

    class OpenSSL_SHA1_HashProvider final : public IHashProvider {
    public:
        static constexpr size_t HashSize = 20;
        std::string hash(const std::string &string) const override;

        size_t size() const override;
    };
}
}
}

#endif //M2CRYPTO_HASH_H

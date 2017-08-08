//
// Created by lol on 8/5/2017.
//

#ifndef M2CRYPTO_SYM_H
#define M2CRYPTO_SYM_H

#include <openssl/aes.h>
#include <memory>
#include <openssl/evp.h>
#include "crypto_common.h"

namespace m2
{
namespace crypto
{
namespace common
{
    class OpenSSL_AES_CryptoProvider final : public common::ICryptoProvider {
    public:
        OpenSSL_AES_CryptoProvider(size_t bit_size, const std::string &key, const std::string &salt);

        std::string encrypt(const std::string &string) const override;
        std::string decrypt(const std::string &string) const override;
        size_t key_size() const;

        std::string encrypt_to_b64(const std::string &string) const override;

        std::string decrypt_from_b64(const std::string &string) const override;

    protected:
        std::unique_ptr<char[]> key_, iv_ ;
        size_t bit_size_;
        decltype(&EVP_aes_256_cbc) ctx_provider_;

        std::string act_(bool, const std::string &) const;
    };
}
}
}

#endif //M2CRYPTO_SYM_H

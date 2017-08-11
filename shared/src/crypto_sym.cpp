//
// Created by lol on 8/5/2017.
//
#include <openssl/ossl_typ.h>
#include <openssl/evp.h>
#include <memory>
#include <cstring>
#include <base64.h>
#include "../include/crypto_sym.h"

namespace m2
{
namespace crypto
{
namespace common
{
    std::string OpenSSL_AES_CryptoProvider::encrypt(const std::string &string) const {
        return act_(true, string);
    }

    std::string OpenSSL_AES_CryptoProvider::decrypt(const std::string &string) const {
        return act_(false, string);
    }


    std::string OpenSSL_AES_CryptoProvider::act_(bool encrypt, const std::string &string) const {
        std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx {
                EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free
        };
        const auto buf_size = std::max(string.size(), static_cast<const size_t>(2*key_size()));

        std::unique_ptr<unsigned char[]> buf { new unsigned char[buf_size+1] };
        int len = 0;
        std::string ciphertext;
        auto init_actor = encrypt ? EVP_EncryptInit_ex : EVP_DecryptInit_ex;
        auto updt_actor = encrypt ? EVP_EncryptUpdate : EVP_DecryptUpdate;
        auto finl_actor = encrypt ? EVP_EncryptFinal_ex : EVP_DecryptFinal_ex;

        if(!ctx)
            throw common::OpenSSL_CryptoError();

        if(1 != init_actor(ctx.get(), ctx_provider_(), nullptr,
                           (const unsigned char *)key_.get(), (const unsigned char *)iv_.get()))
            throw common::OpenSSL_CryptoError();

        if(1 != updt_actor(ctx.get(), buf.get(), &len, (const unsigned char *)(string.c_str()),
                           static_cast<int>(string.size())))
            throw common::OpenSSL_CryptoError();
        ciphertext.append((char*)buf.get(), len);

        if(1 != finl_actor(ctx.get(), buf.get(), &len))
            throw common::OpenSSL_CryptoError();
        ciphertext.append((char*)buf.get(),len);

        return ciphertext;
    }

    OpenSSL_AES_CryptoProvider::OpenSSL_AES_CryptoProvider(const size_t bit_size, const std::string &key,
                                                           const std::string &salt) :
        iv_(new char [bit_size / 2]), key_(new char [bit_size]), bit_size_(bit_size)

    {
        memset(key_.get(), 0, bit_size);
        memset(iv_.get(), 0, bit_size / 2);

        switch(bit_size_) {
            case 128:
                ctx_provider_ = &EVP_aes_128_cbc;
                break;
            case 192:
                ctx_provider_ = &EVP_aes_192_cbc;
                break;
            case 256:
                ctx_provider_ = &EVP_aes_256_cbc;
                break;
            default:
                throw common::CryptoError("bit_size can only have these values: 128, 192 and 256");
        }

        auto code = EVP_BytesToKey(ctx_provider_(), EVP_sha1(), (const unsigned char *)(salt.c_str()),
                       (unsigned char*)key.c_str(), static_cast<int>(key.size()), 1,
                                (unsigned char*)key_.get(),
                                (unsigned char*)iv_.get());
        if (code == -1)
            throw common::OpenSSL_CryptoError();
    }

    size_t OpenSSL_AES_CryptoProvider::key_size() const {
        return bit_size_ / 8;
    }

    std::string OpenSSL_AES_CryptoProvider::encrypt_to_b64(const std::string &string) const {
        return m2::base64::base64_encode(encrypt(string));
    }

    std::string OpenSSL_AES_CryptoProvider::decrypt_from_b64(const std::string &string) const {
        return decrypt(m2::base64::base64_decode(string));
    }
}
}
}


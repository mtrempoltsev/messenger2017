//
// Created by lol on 8/5/2017.
//

#include <cstdio>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <cstring>
#include <string>
#include <sstream>
#include "../include/crypto_pki.h"
#include "../include/crypto_hash.h"

namespace m2
{
namespace crypto
{
namespace common
{
    std::string OpenSSL_RSA_CryptoProvider::encrypt(const std::string &string) const {
        return actor_(true, string);
    }

    std::string OpenSSL_RSA_CryptoProvider::decrypt(const std::string &string) const {
        return actor_(false, string);
    }

    OpenSSL_RSA_CryptoProvider::OpenSSL_RSA_CryptoProvider(const std::string &key, bool is_public):
            key_(OpenSSL_RSA_CryptoProvider::from_string(key, is_public)),
            public_(is_public) {

    }

    OpenSSL_RSA_CryptoProvider::KeyContainer
    OpenSSL_RSA_CryptoProvider::from_string(const std::string &key, bool is_public) {
        KeyContainer cnt { nullptr, &RSA_free };
        RSA* rsa = nullptr;
        BIO *keybio ;
        keybio = BIO_new_mem_buf(key.c_str(), -1);
        if (keybio== nullptr)
            throw common::OpenSSL_CryptoError("Failed to create key BIO");
        if (is_public) {
            rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, nullptr, nullptr);
        } else
            rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, nullptr, nullptr);
        if (rsa == nullptr) {
            throw common::OpenSSL_CryptoError();
        }
        cnt.reset(rsa);
        return std::move(cnt);
    }

    std::string OpenSSL_RSA_CryptoProvider::actor_(bool encryption, const std::string &string) const {
        size_t rsa_size = RSA_size(key_.get());
        std::unique_ptr<unsigned char[]> buf { new unsigned char[2*rsa_size] };

        std::string res;
        size_t total_bytes_done = 0;
        auto actor =  encryption ?
                        (public_ ? &RSA_public_encrypt : &RSA_private_encrypt)
                      : (public_ ? &RSA_public_decrypt : &RSA_private_decrypt);

        do {
            auto bytes_done = actor(static_cast<int>(std::min(string.size(), rsa_size/2)),
                    (const unsigned char *)(string.c_str()+total_bytes_done), buf.get(),
                                    key_.get(), RSA_PKCS1_PADDING);

            if (bytes_done == -1) {
                throw common::OpenSSL_CryptoError("Error during public key encryption: ");
            }
            total_bytes_done += bytes_done;
            res.append((char*)buf.get(), bytes_done);
        } while((total_bytes_done < string.size() ^ encryption) == 0);
        return res;
    }

    std::pair<OpenSSL_RSA_CryptoProvider, OpenSSL_RSA_CryptoProvider>
    OpenSSL_RSA_CryptoProvider::make(int bit_size) {
        KeyContainer cnt { RSA_new(), &RSA_free };
        std::unique_ptr<BIGNUM, decltype(&BN_free)> bne { BN_new(), &BN_free };
        int ret = 0;
        unsigned long e = RSA_F4;
        ret = BN_set_word(bne.get(), e);
        if (ret != 1)
            throw common::OpenSSL_CryptoError("Unknown error when setting BN_set_word()");
        ret = RSA_generate_key_ex(cnt.get(), bit_size, bne.get(), nullptr);
        if (ret != 1)
            throw common::OpenSSL_CryptoError("Error generating RSA key");
        KeyContainer pub {RSAPublicKey_dup(cnt.get()), &RSA_free};
        return { OpenSSL_RSA_CryptoProvider { std::move(cnt), false } ,
                 OpenSSL_RSA_CryptoProvider { std::move(pub), true } };
    }

    boost::uuids::uuid OpenSSL_RSA_CryptoProvider::fingerprint() const {
        // a copy from http://www.boost.org/doc/libs/1_54_0/boost/uuid/name_generator.hpp
        std::string sha1 = OpenSSL_SHA1_HashProvider().hash(str_key());
        boost::uuids::uuid u;
        for (int i=0; i<4; ++i) {
            *(u.begin() + i*4+0) = static_cast<uint8_t>((sha1[i] >> 24) & 0xFF);
            *(u.begin() + i*4+1) = static_cast<uint8_t>((sha1[i] >> 16) & 0xFF);
            *(u.begin() + i*4+2) = static_cast<uint8_t>((sha1[i] >> 8) & 0xFF);
            *(u.begin() + i*4+3) = static_cast<uint8_t>((sha1[i] >> 0) & 0xFF);
        }
        // set variant
        // must be 0b10xxxxxx
        *(u.begin()+8) &= 0xBF;
        *(u.begin()+8) |= 0x80;

        // set version
        // must be 0b0101xxxx
        *(u.begin()+6) &= 0x5F; //0b01011111
        *(u.begin()+6) |= 0x50; //0b01010000

        return u;
    }

    std::string OpenSSL_RSA_CryptoProvider::str_key() const {
        const auto buf_size = static_cast<const size_t>(8 * RSA_size(key_.get()));
        std::unique_ptr<unsigned char[]> buf { new unsigned char[buf_size] };
        std::unique_ptr<BIO, decltype(&BIO_free)> keybio { BIO_new(BIO_s_mem()), &BIO_free };
        std::string res;
        RSA_print(keybio.get(), key_.get(), 0);
        while (BIO_read (keybio.get(), (void*)buf.get(), (int) buf_size) > 0)
            res.append((char*)buf.get());
        return res;
    }

    OpenSSL_RSA_CryptoProvider::OpenSSL_RSA_CryptoProvider(
            OpenSSL_RSA_CryptoProvider::KeyContainer &&key, bool is_public)
            : key_( std::move(key) ), public_(is_public) {

    }
}
}
}


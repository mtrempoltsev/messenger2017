#include <string>
#include <functional>
#include <memory>
#include <openssl/rsa.h>

#include "crypto_common.h"
#include "crypto_hash.h"

namespace m2
{
namespace crypto
{
namespace common
{
    class IAsymmetricCryptoProvider : public common::ICryptoProvider {
        virtual boost::uuids::uuid fingerprint() const = 0;
        virtual std::string str_key() const = 0;
    };

    class OpenSSL_RSA_CryptoProvider final : public IAsymmetricCryptoProvider {
    public:
        using KeyContainer = std::unique_ptr<RSA, decltype(&RSA_free)>;
        using CryptoFunction = std::function<void(const size_t, const unsigned char *, void *, unsigned char *)>;

        OpenSSL_RSA_CryptoProvider(const std::string &key, bool is_public);
        OpenSSL_RSA_CryptoProvider(KeyContainer &&key, bool is_public);

        std::string encrypt(const std::string &string) const override;
        std::string decrypt(const std::string &string) const override;
        std::string encrypt_to_b64(const std::string &string) const override;
        std::string decrypt_from_b64(const std::string &string) const override;


        std::unique_ptr<OpenSSL_RSA_CryptoProvider> get_public();

        bool is_public() const;
        bool is_private() const;


        static KeyContainer from_string(const std::string &key, bool is_public);
        static std::pair<std::unique_ptr<OpenSSL_RSA_CryptoProvider>, std::unique_ptr<OpenSSL_RSA_CryptoProvider>>
        make(int bit_size);
        static std::unique_ptr<OpenSSL_RSA_CryptoProvider> make_private(int bit_size);

        boost::uuids::uuid fingerprint() const override;

        std::string str_key() const override;

    protected:
        KeyContainer key_;
        bool public_;

        std::string actor_(bool encryption, const std::string &string) const;

        int get_padding(bool encryption) const;
    };
}
}
}

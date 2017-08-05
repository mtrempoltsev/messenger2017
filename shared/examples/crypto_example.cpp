#include <openssl/dsa.h>
#include <string>
#include <cstring>
#include "../include/crypto_sym.h"
#include "../include/crypto_pki.h"

int main() {
    // E
    const std::string to_encrypt { "A VERY LONG LINE" };
    m2::crypto::common::OpenSSL_RSA_CryptoProvider orca {
                    "-----BEGIN PUBLIC KEY-----\n"
                    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC7vbqajDw4o6gJy8UtmIbkcpnk\n"
                    "O3Kwc4qsEnSZp/TR+fQi62F79RHWmwKOtFmwteURgLbj7D/WGuNLGOfa/2vse3G2\n"
                    "eHnHl5CB8ruRX9fBl/KgwCVr2JaEuUm66bBQeP5XeBotdR4cvX38uPYivCDdPjJ1\n"
                    "QWPdspTBKcxeFbccDwIDAQAB\n"
                    "-----END PUBLIC KEY-----\n"
            , true
    };
    m2::crypto::common::OpenSSL_AES_CryptoProvider aes(192, "mypass", "mysalt");
    auto encrypted = aes.encrypt(to_encrypt);
    auto decrypted = aes.decrypt(encrypted);
    if (decrypted != to_encrypt)
        throw std::runtime_error("Lines for AES do not match");

    auto m = m2::crypto::common::OpenSSL_RSA_CryptoProvider::make(2048);
    encrypted = m.second.encrypt(to_encrypt);
    decrypted = m.first.decrypt(encrypted);
    auto finger = orca.fingerprint();
    if (decrypted != to_encrypt)
        throw std::runtime_error("Lines for RSA do not match");
    return 0;
}

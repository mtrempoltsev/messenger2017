#include "../include/crypto_hash.h"

namespace m2
{
namespace crypto
{
namespace common
{
    std::string OpenSSL_SHA1_HashProvider::hash(const std::string &string) const {
        unsigned char buf[ HashSize ];
        SHA1((const unsigned char *)(string.c_str()),
             string.size(),
             buf
        );
        return (char*)buf;
    }

    size_t OpenSSL_SHA1_HashProvider::size() const {
        return HashSize;
    }
}
}
}

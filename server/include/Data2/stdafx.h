
#ifndef M2_SERVER_STDAFX2_H_H
#define M2_SERVER_STDAFX2_H_H

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/functional/hash.hpp>


typedef boost::multiprecision::uint128_t uint128;

    namespace uuids
    {
    union type128
    {
        uint128 base;
        uint64_t part[2];
    };

    struct uuid
    {
        uint128 value;

        bool operator==(const uuid &other) const
        {
            return (value == other.value);
        }
    };
    struct KeyHasher
    {
        std::size_t operator()(const uuid &u) const
        {
            type128 bigNumber = {u.value};
           // bigNumber.base = u.value;
            std::size_t hash = 0;
            boost::hash_combine(hash, bigNumber.part[0]);
            boost::hash_combine(hash, bigNumber.part[1]);
            return hash;
        }
    };
}


#endif //M2_SERVER_STDAFX2_H_H

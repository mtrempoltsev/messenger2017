#ifndef M2_SERVER_STDAFX_H_H
#define M2_SERVER_STDAFX_H_H



#include <boost/enable_shared_from_this.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <sstream>
#include <fstream>
#include <iomanip>
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>




/****************************/

// make splitter macros '/'  '\\'

/****************************/

#define check(EXP)  if(!(EXP))
#define checkR(EXP) check(EXP) return
#define checkB(EXP) check(EXP) break
#define checkC(EXP) check(EXP) continue
#define checkT(EXP) check(EXP) throw

/****************************/

#define ifR(EXP)    if(EXP) return
#define ifB(EXP)    if(EXP) break
#define ifC(EXP)    if(EXP) continue
#define ifT(EXP)    if(EXP) throw

/****************************/

namespace indices {

    void MakeDir(const std::string& Path);

}

namespace uuids   {

    using uuid = boost::multiprecision::uint128_t ;

    uuid to_uuid(const boost::uuids::uuid &a);


    namespace misc {

        union uuid_split {
            uuid   Uid;
            size_t Part[2];
        };
    }

}

namespace std {

    template<>
    struct hash<uuids::uuid>
    {
        typedef size_t      result_type;
        typedef uuids::uuid argument_type;

        size_t
        operator()(const uuids::uuid& __p) const noexcept {

            size_t Size = 0;
            uuids::misc::uuid_split Split = {__p};
            boost::hash_combine(Size, Split.Part[1]);
            boost::hash_combine(Size, Split.Part[2]);

            return Size;
        }
    };
}

#endif //M2_SERVER_STDAFX_H_H



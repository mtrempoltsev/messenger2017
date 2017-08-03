#ifndef M2_SERVER_USER_H
#define M2_SERVER_USER_H

#include "../stdafx.h"


namespace m2   {
namespace data {
namespace user {

    /**
     *
     */
    class AUser
    {
    public:



    public: /***************| Construction |***************/
        AUser();

        AUser(uuids::uuid Uid, const std::string&  PubKey);
        AUser(uuids::uuid Uid,       std::string&& PubKey);

        AUser(std::istream& is);



    public: /***************| Interface |***************/

        void Serialaize(std::ostream& os);

        bool IsValid();

    protected: /************| Members |***************/

        std::string public_key;
        uuids::uuid uid;

    public:
        const std::string&  PublicKey() const;

        uuids::uuid Uid() const;

    public: /***************| Operators |***************/

        std::ostream& operator<<(std::ostream& os);

        operator bool();
    };



}}}


#endif //M2_SERVER_USER_H

#ifndef M2_SERVER_USER_H
#define M2_SERVER_USER_H

#include "Data/stdafx.h"


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

        //AUser(std::istream& is);

    public: /***************| Interface |***************/

        //void Serialaize(std::ostream& os);

        ///check if the user isn't empty
        bool IsValid() const;

    protected: /************| Members |***************/

        std::string public_key;
        uuids::uuid uid;

    public:

        const std::string& PublicKey() const;

        uuids::uuid Uid() const;

    public: /***************| Operators |***************/

        //std::ostream& operator<<(std::ostream& os);

        operator bool() const;
        operator std::string() const;   // convert to a public Key
        operator uuids::uuid() const;   // convert to a uid
    };



}}}


#endif //M2_SERVER_USER_H

#ifndef M2_SERVER_SESSIONS_H
#define M2_SERVER_SESSIONS_H


#include "unordered_map"
#include "../stdafx.h"
#include "session.h"

namespace m2      {
namespace data    {
namespace sessions{

    using namespace server;


    class ASessions
    {
        using LSessions = std::unordered_map<uuids::uuid, Session*>;

    public:
        ASessions() = default;

    public: /***************| Interface |***************/

        Session* GetSession(const uuids::uuid& Uid);
        bool     AddSession(const uuids::uuid& Uid, Session* ptr);
        void  DeleteSession(const uuids::uuid& Uid);

    protected: /************| Members |***************/

        LSessions sessions;

    };


}}}

#endif //M2_SERVER_SESSIONS_H

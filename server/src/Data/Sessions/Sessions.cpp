#include "Data/Sessions/Sessions.h"

using namespace m2::data::sessions;


Session*
ASessions::GetSession(const uuids::uuid& Uid) {
    if (sessions.count(Uid))
        return sessions.at(Uid);
    return nullptr;
}

bool
ASessions::AddSession(const uuids::uuid& Uid, Session* ptr) {
    if (sessions.count(Uid))
        return false;
    sessions[Uid] = ptr;
    return true;
}

void
ASessions::DeleteSession(const uuids::uuid& Uid)
{ sessions.erase(Uid); }

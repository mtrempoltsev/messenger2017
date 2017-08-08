#include "Data/Sessions/Sessions.h"

using namespace m2::data::sessions;


sessionWPtr
ASessions::GetSession(const uuids::uuid& Uid) {
    if (sessions.count(Uid))
        return sessions.at(Uid);
    return sessionWPtr();
}

bool
ASessions::AddSession(const uuids::uuid& Uid, sessionWPtr ptr) {
    if (sessions.count(Uid))
        return false;
    sessions[Uid] = ptr;
    return true;
}

void
ASessions::DeleteSession(const uuids::uuid& Uid)
{ sessions.erase(Uid); }

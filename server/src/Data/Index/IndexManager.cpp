
#include "IndexManager.h"

using namespace indices;

AIndexManager::AIndexManager(const std::string& Path)
    : path(Path)
{ LoadFromDisk(); }

AIndexManager::AIndexManager(std::string&& Path)
    : path(std::move(Path))
{ LoadFromDisk(); }

AIndexManager::~AIndexManager()
{ StoreOnDisk(); }

/**********************************************************/

#define  STR(V) std::to_string(V)
#define LINE    "\n"

void AIndexManager::LoadFromDisk() {
    std::ifstream in(path);
    checkR(in.is_open())
        StoreOnDisk();

    int size;
    in >> size;

    uuids::uuid Uid;
    for(; in >> Uid; Uid = 0)
        uids.emplace(Uid);

    //cmp size uids.size()
}

void AIndexManager::StoreOnDisk() {
    std::ofstream os(path);
    checkR(os.is_open());
    // send error

    os << STR(uids.size());
    os << LINE;

    for (auto& i : uids) {
        os << i.str();
        os << LINE;
    }
}

/**********************************************************/

void AIndexManager::Add(uuids::uuid Uid)
{ uids.insert(Uid); }

void AIndexManager::Remove(uuids::uuid Uid)
{ uids.erase(Uid); }

bool AIndexManager::IsContains(uuids::uuid Uid)
{ return uids.find(Uid) != uids.end(); }

/**********************************************************/

std::string AIndexManager::Path() const
{ return path; }

const AIndexManager::LUids& AIndexManager::Uids() const
{ return uids; }

/**********************************************************/

bool AIndexManager::operator[](uuids::uuid Uid)
{ return IsContains(Uid); }


#ifndef M2_SERVER_INDEXMANAGER_HPP
#define M2_SERVER_INDEXMANAGER_HPP

#include <unordered_set>
#include <boost/unordered_set.hpp>
#include <list>
#include "Data/stdafx.h"

namespace indices {

    /**
     * Class, binding to a folder, where it storage a index
     *
     * >>
     *
     * 5        - count of positions
     * UID1
     * UID2
     * UID3
     *
     */
    template< typename _Tp = uuids::uuid    // indexing type
            , typename _Hs = std::hash<_Tp> // hash function
    >class TIndexManager
    //        : boost::noncopyable
    {
    public:

        typedef std::unordered_set<_Tp, _Hs> LUids;

    public: /***************| Construction |***************/

        TIndexManager(const std::string& Path)
            : root(separate_root(Path))
            , path(Path)
            , flag(0)
        { LoadFromDisk(); }

        TIndexManager(std::string&& Path)
                : root(separate_root(Path))
                , path(std::move(Path))
                , flag(0)
        { LoadFromDisk(); }

        ~TIndexManager()
        { StoreOnDisk(); }

    public: /***************| Interface |***************/

        void LoadFromDisk() {
            std::ifstream in(path);
            checkR(in.is_open())
                StoreOnDisk();

            in >> flag;

            _Tp Uid;
            for(; in >> Uid; Uid = _Tp())
                uids.emplace(Uid);
        }

        void StoreOnDisk () const {
            MakeDir(root);
            std::ofstream os(path);
            checkR(os.is_open());
            // send error

            os << flag;
            os << "\n";

            for (auto& i : uids) {
                os << i;
                os << "\n";
            }
        }

        void Add       (const _Tp& Uid)
        { uids.insert(Uid); }

        void Remove    (const _Tp& Uid)
        { uids.erase (Uid); }

        bool IsContains(const _Tp& Uid) const {
            for (auto& i : uids)
                if (i == Uid)
                    return true;
            return false;
        }

    protected: /************| Members |***************/

        std::string root;
        std::string path;

        uuids::uuid flag;
        LUids       uids;

    public:

        std::string  Path() const { return path; }
        const LUids& Uids() const { return uids; }
        uuids::uuid& Flag()       { return flag; }
        uuids::uuid  Flag() const { return flag; }

    protected:

        std::string separate_root(const std::string& p)
        { return p.substr(0, p.find_last_of('/')); }

    public: /***************| operators |***************/

        bool operator[](const _Tp& Uid) const
        { return IsContains(Uid); }
    };

    using AIndexManager = TIndexManager<>;
}

#endif //M2_SERVER_INDEXMANAGER_HPP

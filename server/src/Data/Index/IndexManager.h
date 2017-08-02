#ifndef M2_SERVER_INDEXMANAGER_H
#define M2_SERVER_INDEXMANAGER_H

#include <unordered_set>
#include <boost/unordered_set.hpp>
#include <list>
#include "../stdafx.h"

namespace indices {

    /**
     * 5        - count of positions
     * UID1
     * UID2
     * UID3
     * UID4
     * UID5
     *
     *************************
     *
     */
    class AIndexManager
    //        : boost::noncopyable
    {
    public:

        typedef boost::unordered_set<uuids::uuid> LUids;

    public: /***************| Construction |***************/

        AIndexManager(const std::string&  Path);
        AIndexManager(      std::string&& Path);

        ~AIndexManager();

    public: /***************| Interface |***************/

        void LoadFromDisk();
        void StoreOnDisk ();

        void Add   (uuids::uuid Uid);
        void Remove(uuids::uuid Uid);

        bool IsContains(uuids::uuid Uid);

    protected: /************| Members |***************/

        std::string path;

        LUids uids;

    public:

        std::string Path() const;

        const LUids& Uids() const;

    public: /***************| operators |***************/

        bool operator[](uuids::uuid Uid);

    };


}

#endif //M2_SERVER_INDEXMANAGER_H

#ifndef M2_SERVER_DIALOGSYSTEM_H
#define M2_SERVER_DIALOGSYSTEM_H

#include <string>

#include "../stdafx.h"

#include "../Index/CashManager.hpp"
#include "../Index/IndexManager.hpp"


namespace m2     {
namespace data   {
namespace dialog {

    using namespace indices;

    /**
     * A base class for dialogs
     */
    template <class Hand_t                  // handling type (child)
            , class Huid_t  = uuids::uuid   // child id type
            , class Uuid_t  = uuids::uuid   // self  id type
            , class Index_t = TIndexManager<Huid_t>
    > class TDialogSystem
    {
    public:

        using Str_t     =          std::string;
        using Cash_t    =          TCashManager<Hand_t>;
        using Cuids_t   = typename Index_t::LUids;
        using MPtr      = typename Cash_t::MPtr;
        using CPtr      = typename Cash_t::CPtr;

    public: /************| Construction |***************/

        TDialogSystem(const Str_t & Root
                    , const Str_t & IndexName
                    , const size_t& CashLength
                    , const Uuid_t& Uid = Uuid_t()
        )   : root (Root)
            , index(Root + IndexName)
            , uid  (Uid)
            , cash (CashLength)
        {}

    public: /************| Interface |***************/

        void Add(const Uuid_t& key) {

        }

        virtual MPtr Get(const Huid_t& key)       = 0; // How Does It Work????
        virtual CPtr Get(const Huid_t& key) const = 0; // It's a template!!!

        virtual bool IsContains(const Huid_t& key) const
        { return index[key]; }

    protected: /*********| Memebers |***************/

                Uuid_t  uid;
                Str_t   root;
                Index_t index;
        mutable Cash_t  cash;

    public:

        const Huid_t & Uid  () const { return uid;          }
        const Str_t  & Root () const { return root;         }
        const Cuids_t& Index() const { return index.Uids(); }

        size_t CashLength() const { return cash.CashLength(); }

    public:

        void SetCashLength(size_t New) { cash.SetCashLength(New); }

    public: /************| Operators |***************/

        MPtr operator()(const Huid_t& rs)       { return Get(rs);        }
        CPtr operator()(const Huid_t& rs) const { return Get(rs);        }
        bool operator[](const Huid_t& rs) const { return IsContains(rs); }
        bool operator==(const Uuid_t& rs) const { return uid == rs;      }
    };


}}}

#endif //M2_SERVER_DIALOGSYSTEM_H

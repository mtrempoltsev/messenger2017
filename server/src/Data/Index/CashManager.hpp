#ifndef M2_SERVER_CASHMANAGER_H
#define M2_SERVER_CASHMANAGER_H

#include <list>
#include <algorithm>
#include "../stdafx.h"



namespace indices {



    /**
     * SHARED_PTR ONLY!
     */
    template<typename _Tp>
    class TCashManager
        //: boost::noncopyable
    {
    public:

        typedef std::shared_ptr<_Tp> LPtr;
        typedef std::list<LPtr>      LCash;

    public: /***************| Construction |***************/

        TCashManager(size_t CashLength = 25)
            : cash_length(CashLength)
        {}

    public: /***************| Interface |***************/

        template<typename ...Args>
        LPtr Add(const Args&... El) {
            auto ptr = std::make_shared<_Tp>(El...);
            cash.push_back(ptr);
            return ptr;
        }

       LPtr Add(LPtr ptr) {
           cash.push_back(ptr);
           return ptr;
       }

        template <typename Key, typename Cmp>
        LPtr Get(Key key, Cmp cmp) {
            for (auto i : cash)
                ifR(cmp(*i, key)) i;
            return nullptr;
        };

        template <typename Key>
        LPtr Get(Key key) {
            for (auto i : cash)
                ifR(*i == key) i;
            return nullptr;
        };

        template <typename Key, class Cmp>
        void Remove(Key key, Cmp cmp) {
            auto itr = cash.begin();
            auto end = cash.end  ();
            while (itr != end) {
                if (cmp(**itr, key))
                    itr = cash.erase(itr);
                else ++itr;
            }
        };

        template <typename Key>
        void Remove(Key key) {
            auto itr = cash.begin();
            auto end = cash.end  ();
            while (itr != end) {
                if (**itr == key)
                    itr = cash.erase(itr);
                else ++itr;
            }
        };

        void Normalize() {
            auto itr = cash.begin();
            auto end = cash.end  ();
            while (itr != end && cash.size() > cash_length)
                itr = cash.erase(itr);
        }

        template <typename Key, class Cmp>
        bool IsContains(const Key& rs, Cmp cmp)
        { return Get(rs, cmp) ? true : false; }

        template <typename Key>
        bool IsContains(const Key& rs)
        { return Get(rs) ? true : false; }

        void Clear()
        { cash.clear(); }

    protected: /************| Members |***************/

        LCash  cash;
        size_t cash_length;

    public:

        const LCash& Cash() const
        { return cash; }

        size_t CashLength() const
        { return cash_length; }

        size_t Size() const
        { return cash.size(); }

    public:

        void SetCashLength(size_t Length) {
            cash_length = Length;
            Normalize();
        }


    public: /***************| operators |***************/

        template <typename _Tq>
        LPtr operator[](const _Tq& rs)
        { return Get(rs); }
    };

}

#endif //M2_SERVER_CASHMANAGER_H

/*
TCashManager<ADialog>::Cmp <uint128>


function<bool (const ADialog &, const uint128 &)>(ADialog&, TCashManager<ADialog>::LCash&)

 */
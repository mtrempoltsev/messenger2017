#ifndef M2_SERVER_CASHMANAGER_H
#define M2_SERVER_CASHMANAGER_H

#include <list>
#include <algorithm>
#include "Data/stdafx.h"



namespace indices {



    /**
     * SHARED_PTR ONLY!
     */
    template<typename _Tp>
    class TCashManager
        //: boost::noncopyable
    {
    public:

        using MPtr  =       std::shared_ptr<_Tp>;
        using CPtr  = const std::shared_ptr<_Tp>;
        using LCash =       std::list<MPtr>;

    public: /***************| Construction |***************/

        TCashManager(const size_t& CashLength = 25)
            : cash_length(CashLength)
        {}

    public: /***************| Interface |***************/

        template<typename ...Args>
        MPtr Add(const Args&... El) {
            auto ptr = std::make_shared<_Tp>(El...);
            cash.push_back(ptr);
            clamp();
            return ptr;
        }

       MPtr Add(MPtr ptr) {
           cash.push_back(ptr);
           clamp();
           return ptr;
       }

        template <typename Key, typename Cmp>
        MPtr Get(Key key, Cmp cmp) const {
            for (auto i : cash)
                ifR(cmp(*i, key)) i;
            return nullptr;
        }

        template <typename Key>
        MPtr Get(Key key) const {
            for (auto i : cash)
                ifR(*i == key) i;
            return nullptr;
        }

        template <typename Key, class Cmp>
        void Remove(Key key, Cmp cmp) {
            auto itr = cash.begin();
            auto end = cash.end  ();
            while (itr != end) {
                if (cmp(**itr, key))
                    itr = cash.erase(itr);
                else ++itr;
            }
        }

        template <typename Key>
        void Remove(Key key) {
            auto itr = cash.begin();
            auto end = cash.end  ();
            while (itr != end) {
                if (**itr == key)
                    itr = cash.erase(itr);
                else ++itr;
            }
        }

        void Normalize() {
            auto itr = cash.begin();
            auto end = cash.end  ();
            while (itr != end && cash.size() > cash_length)
                itr = cash.erase(itr);
        }

        template <typename Key, class Cmp>
        bool IsContains(const Key& rs, Cmp cmp) const
        { return Get(rs, cmp) ? true : false; }

        template <typename Key>
        bool IsContains(const Key& rs) const
        { return Get(rs) ? true : false; }

        void Clear()
        { cash.clear(); }

    protected: /************| Members |***************/

        LCash  cash;
        size_t cash_length;

    public:

        const LCash& Cash()       const { return cash;        }
              size_t CashLength() const { return cash_length; }
              size_t Size()       const { return cash.size(); }

    public:

        void SetCashLength(size_t Length) {
            cash_length = Length;
            Normalize();
        }

    protected: /************|  |***************/

        void clamp() {
            if(cash.size() > cash_length)
                cash.pop_front();
        }

    public: /***************| Tterators |***************/

        typename LCash::iterator begin()
        { return cash.begin(); }

        typename LCash::iterator end()
        { return cash.end(); }

        typename LCash::const_iterator begin() const
        { return cash.begin(); }

        typename LCash::const_iterator end() const
        { return cash.end(); }

    public: /***************| operators |***************/

        template <typename _Tq>
        MPtr operator()(const _Tq& rs) const { return Get(rs); }

        template <typename _Tq>
        bool operator[](const _Tq& rs) const
        { return IsContains(rs); }
    };

}

#endif //M2_SERVER_CASHMANAGER_H

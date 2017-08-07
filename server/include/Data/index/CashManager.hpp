#ifndef M2_SERVER_CASHMANAGER_H
#define M2_SERVER_CASHMANAGER_H

#include <list>
#include <algorithm>
#include "Data/stdafx.h"



namespace indices {



    /**
     * A simple templated class cashing shared pointers on required type
     *
     * >>
     *
     * NOTE: back is the most newest elements
     * NOTE: when cash is owerflow head elements are removeing
     * NOTE: @_Tp are wrapped in std::shared_ptr
     *
     * TODO: merge functions with comparator with clear
     */
    template<typename _Tp>
    class TCashManager
        //: boost::noncopyable
    {
    public:

        using MPtr  =       std::shared_ptr<_Tp>;
        using CPtr  = const std::shared_ptr<_Tp>;
        using LList =       std::list<MPtr>;

    public: /***************| Construction |***************/

        TCashManager(const size_t& CashLength = 25)
            : cash_length(CashLength)
        {}

    public: /***************| Interface |***************/

    /***********************| Add |***********************/

        /// create an element with @args in cash and take pointer
        template<typename ...Args>
        MPtr Add(const Args&... args) {
            auto ptr = std::make_shared<_Tp>(args...);
            cash.push_back(ptr);
            clamp();
            return ptr;
        }

        /// push std::shared_ptr to cash
        MPtr Add(MPtr ptr) {
           cash.push_back(ptr);
           clamp();
           return ptr;
       }

    /***********************| Get |***********************/

        /// return first element == key or nullptr
        template <typename Key, typename Cmp> MPtr Get(Key key, Cmp cmp)       { return get(key, cmp); }
        template <typename Key, typename Cmp> CPtr Get(Key key, Cmp cmp) const { return get(key, cmp); }

        /// return first element cmp(element, key) == true
        template <typename Key> MPtr Get(Key key)       { return get(key); }
        template <typename Key> CPtr Get(Key key) const { return get(key); }


    /***********************| Remove |***********************/

        /// remove all elements cmp(element, key) == true
        template <typename Key, class Cmp>
        void Remove(Key key, Cmp cmp) {
            typename LList::iterator itr, end;
            itr = cash.begin();
            end = cash.end();
            while (itr != end) {
                if (cmp(**itr, key))
                    itr = cash.erase(itr);
                else ++itr;
            }
        }

        /// remove all elements == @key
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

        /// remove all elements from the cash
        void Clear()
        { cash.clear(); }

    /***********************| Normalize |***********************/

        /// clamp element count to @cash_length
        void Normalize() {
            auto itr = cash.begin();
            auto end = cash.end  ();
            while (itr != end &&
                cash.size() > cash_length )
            {
                itr = cash.erase(itr);
            }
        }

    /***********************| Check |***********************/

        /// does the element contains in cash
        template <typename Key, class Cmp>
        bool IsContains(const Key& rs, Cmp cmp) const
        { return Get(rs, cmp) != nullptr; }

        /// does the element contains in the cash
        template <typename Key>
        bool IsContains(const Key& rs) const
        { return Get(rs) != nullptr; }



    protected: /************| Members |***************/

        mutable LList  cash;            /// list of pointers
                size_t cash_length;     /// max cash length

    public:

        const
        std::list<MPtr>& Cash() const
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

    protected: /************|  |***************/

        /// pop latest in time element
        void clamp() {
            if(cash.size() > cash_length)
                cash.pop_front();
        }

        template <typename Key, typename Cmp>
        MPtr get(Key key, Cmp cmp) const {
            auto itr = cash.begin();
            auto end = cash.end();
            for (; itr != end; ++itr)
                if (cmp(**itr, key)) {
                    //move back
                    auto tmp = *itr;
                    cash.push_back(*itr);
                    cash.erase(itr);
                    return *itr;
                }
            return nullptr;
        }

        template <typename Key>
        MPtr get(Key key) const {
            auto itr = cash.begin();
            auto end = cash.end();
            for (; itr != end; ++itr)
                if(**itr == key) {
                    // move back
                    auto tmp = *itr;
                    cash.push_back(*itr);
                    cash.erase(itr);
                    return *itr;
                }
            return nullptr;
        }

    public: /***************| Tterators |***************/
        //std::list<std::shared_ptr<_Tp>>::iterator
        auto begin()
        { return cash.begin(); }

        //std::list<std::shared_ptr<_Tp>>::iterator
        auto end()
        { return cash.end(); }


        //std::list<std::shared_ptr<_Tp>>::const_iterator
        auto begin() const
        { return cash.begin(); }

        //std::list<std::shared_ptr<_Tp>>::const_iterator
        auto end() const
        { return cash.end(); }

    public: /***************| operators |***************/
        /// get element
        template <typename _Tq>
        MPtr operator()(const _Tq& rs) const
        { return Get(rs); }

        /// check element
        template <typename _Tq>
        bool operator[](const _Tq& rs) const
        { return IsContains(rs); }
    };

}

#endif //M2_SERVER_CASHMANAGER_H
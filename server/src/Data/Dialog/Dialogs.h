#ifndef M2_SERVER_DIALOGS_H
#define M2_SERVER_DIALOGS_H


#include "UserDialogs.h"



/********
 *
 * Creation -> try to detect index file
 *              ** create index
 *              ** log in
 *              #########
 *          -> load index to memory
 *
 *
 *
 * ******
 * >> make cash cleaning
 */


namespace m2     {
namespace data   {
namespace dialog {



    /**
     *
     *
     */
    class ADialogs
        : boost::enable_shared_from_this<ADialogs>
        //, boost::noncopyable
    {
    public:

        typedef TCashManager<AUserDialogs> LDialogCash;
        typedef AIndexManager::LUids       LUsers;

    public: /***************| Construction |***************/

        ADialogs(const std::string&  Root);
        ADialogs(      std::string&& Root);

    public: /***************| Interface |***************/

        // if it doesn't exist, it creates it
        AUserDialogs::ptr GetDialog(const AUser& User);

        bool IsContains(const AUser& User);

    protected: /************| Members |***************/

        std::string   root;
        AIndexManager index;
        LDialogCash   cash;

    public:

        size_t CashLength() const;
        LUsers Users() const;

    public:

        void SetCashLength(size_t NewLength);

    protected: /************|  |***************/

        AUserDialogs::ptr create_dialogs(uuids::uuid Uid);

    public: /***************| Operators |***************/

        AUserDialogs::ptr operator[](const AUser& User);
        bool              operator()(const AUser& User);
    };

}}}


#endif //M2_SERVER_DIALOGS_H

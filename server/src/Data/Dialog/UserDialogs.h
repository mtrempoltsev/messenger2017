#ifndef M2_SERVER_USERDIALOGS_H
#define M2_SERVER_USERDIALOGS_H

#include <unordered_set>
#include "../User/User.h"
#include "Dialog.h"


/*******
 *
 * Creation -> try to read an index
 *              ** create an empty one
 *              ** log in
 *              #########
 *          -> load it to memory
 *          #########
 *
 * Dialog   -> try to find it
 *              ** create Dialog
 *          -> create Dialog class
 *          -> return shared pointer
 *          #########
 *
 * Create   -> add it to database (disk)
 *          -> add it to set
 *          ->
 */


namespace m2     {
namespace data   {
namespace dialog {

    using namespace user;


    /**
     *
     *
     */
    class AUserDialogs
        : boost::enable_shared_from_this<AUserDialogs>
        //, boost::noncopyable
    {
    public:
        typedef std::shared_ptr<AUserDialogs> ptr;
        typedef TCashManager<ADialog>         LDialogCash;
        typedef AIndexManager::LUids          LChatSet;

    protected: /************| Construction |***************/

        AUserDialogs(const std::string&  Root, uuids::uuid Uid);

    public:

        AUserDialogs(const AUserDialogs& ) = default;

        static ptr Create(const std::string&  Root, uuids::uuid Uid);
        static ptr Create(      std::string&& Root, uuids::uuid Uid);

    public: /***************| Interface |***************/

        ADialog::ptr GetDialog(uuids::uuid Uid);

        bool IsContains(uuids::uuid Uid);

    protected: /************| Members |***************/

        uuids::uuid   uid;
        std::string   root;
        LDialogCash   dialogs;
        AIndexManager index;

    public:

              uuids::uuid  Uid()        const { return uid;                 }
        const std::string& Root()       const { return root;                }
        const LChatSet&    Chats()      const { return index.Uids();        }
              size_t       CashLength() const { return dialogs.CashLength();}

    public:

        void SetCashLength(size_t NewLength);

    protected: /************|  |***************/

        ADialog::ptr load_dialog(uuids::uuid Uid);

    public: /***************| Operators |***************/

        ADialog::ptr operator[](uuids::uuid Uid);
        bool         operator()(uuids::uuid Uid);

        bool operator==(const uuids::uuid& Uid) const;

    };


}}}

#endif //M2_SERVER_USERDIALOGS_H

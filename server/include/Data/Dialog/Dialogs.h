#ifndef M2_SERVER_DIALOGS_H
#define M2_SERVER_DIALOGS_H


#include "UserDialogs.h"



namespace m2     {
namespace data   {
namespace dialog {



    /**
     *
     */
    class ADialogs
            : public TDialogSystem<AUserDialogs>
    {
    public:

        ADialogs(const std::string&  Root);

    public:

        /**
         * @return pointer on a UserDialog
         */
        std::shared_ptr<AUserDialogs>
        Get(const uuids::uuid& User)       override ;

        /**
         * @return pointer on a UserDialog or nullptr
         */
        const
        std::shared_ptr<AUserDialogs>
        Get(const uuids::uuid& User) const override ;


        /**
         * combine 2 Get functions
         * @param Sender - user
         * @param Sendee - contact
         * @return pointer on a Dialog
         */
        std::shared_ptr<ADialog>
        Get(const uuids::uuid& Sender, const uuids::uuid& Sendee);

        /**
         * @return pointer on a dialog on nullptr
         */
        const
        std::shared_ptr<ADialog>
        Get(const uuids::uuid& Sender, const uuids::uuid& Sendee) const;

    protected:

        /**
         * @param Uid - uid of user
         * @return pointer on a  UserDialog or nullptr
         */
        std::shared_ptr<AUserDialogs>
        create_dialogs(uuids::uuid Uid);

    };

}}}


#endif //M2_SERVER_DIALOGS_H

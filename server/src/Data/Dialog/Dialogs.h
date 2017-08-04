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

        using MPPtr =       std::shared_ptr<ADialog>;
        using CPPtr = const std::shared_ptr<ADialog>;

    public:

        ADialogs(const std::string&  Root);

    public:

        // return UserDialog, if it doesn't exist - creates
        MPtr Get(const uuids::uuid& User)       override ;
        // return UserDialog, if it doesn't exist - nullptr
        CPtr Get(const uuids::uuid& User) const override ;

        // combine 2 Get functions
        MPPtr Get(const uuids::uuid& Sender, const uuids::uuid& Sendee);
        CPPtr Get(const uuids::uuid& Sender, const uuids::uuid& Sendee) const;

    protected:

        MPtr create_dialogs(uuids::uuid Uid);

    };

}}}


#endif //M2_SERVER_DIALOGS_H

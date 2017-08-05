#ifndef M2_SERVER_USERDIALOGS_H
#define M2_SERVER_USERDIALOGS_H


#include "../User/User.h"
#include "Dialog.h"


namespace m2     {
namespace data   {
namespace dialog {

    using namespace user;


    /**
     *
     */
    class AUserDialogs
        : public TDialogSystem<ADialog>
    {
    public:

        AUserDialogs(const std::string&  Root, uuids::uuid Uid);

    public:

        MPtr Get(const uuids::uuid& Uid)       override;
        CPtr Get(const uuids::uuid& Uid) const override;

    protected:

        MPtr create_dialog(uuids::uuid Uid);
    };


}}}

#endif //M2_SERVER_USERDIALOGS_H

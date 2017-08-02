
#include <iomanip>

#include "Dialogs.h"


using namespace m2::data::dialog;

ADialogs::ADialogs(const std::string& Root)
    : root(Root)
    , users(root)
    , dialogs(256)
{}

ADialogs::ADialogs(std::string&& Root)
    : root(std::move(Root))
    , users(root)
    , dialogs(256)
{}

ADialogs::ptr ADialogs::Create(const std::string& Root)
{
    return ptr(new ADialogs(Root));
}

ADialogs::ptr ADialogs::Create(std::string&& Root)
{
    return ptr(new ADialogs(std::move(Root)));
}

/****************************|  |****************************/

AUserDialogs::ptr ADialogs::GetDialog(const AUser& User) {
    uuids::uuid Uid = User.Uid();

    checkR(users[Uid]) create_dialogs(Uid);

    auto ptr = dialogs[Uid];
    ifR(ptr) ptr;

    return AUserDialogs::Create(root + Uid.str() + "/", Uid);
}

ADialogs::LUsers ADialogs::Users() const
{ return users.Uids(); }

/****************************|  |****************************/

size_t ADialogs::CashLength() const
{ return dialogs.CashLength(); }

void ADialogs::SetCashLength(size_t NewLength)
{  dialogs.SetCashLength(NewLength); }

/****************************|  |****************************/

AUserDialogs::ptr ADialogs::create_dialogs(uuids::uuid Uid) {
    auto tmp = AUserDialogs::Create(root  + Uid.str() + "/", Uid);
    checkR(tmp) tmp;

    dialogs.Add(tmp);
    users  .Add(Uid);
    return tmp;
}

/****************************|  |****************************/

AUserDialogs::ptr
ADialogs::operator[](const AUser& User)
{ return GetDialog(User); }




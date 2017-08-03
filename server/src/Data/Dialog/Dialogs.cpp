
#include <iomanip>

#include "Dialogs.h"


using namespace m2::data::dialog;

ADialogs::ADialogs(const std::string& Root)
    : root(Root)
    , index(root + "Index")
    , cash(256)
{}

ADialogs::ADialogs(std::string&& Root)
    : root(std::move(Root))
    , index(root + "Index")
    , cash(256)
{}

/****************************|  |****************************/

AUserDialogs::ptr ADialogs::GetDialog(const AUser& User) {
    uuids::uuid Uid = User.Uid();

    // new user
    checkR(index[Uid]) create_dialogs(Uid);

    // from cash
    auto ptr = cash[Uid];
    ifR(ptr) ptr;

    // from disk
    return AUserDialogs::Create(root + Uid.str() + "/", Uid);
}

bool ADialogs::IsContains(const AUser& User)
{ return index[User]; }

/****************************|  |****************************/

size_t ADialogs::CashLength() const
{ return cash.CashLength(); }

ADialogs::LUsers ADialogs::Users() const
{ return index.Uids(); }

void ADialogs::SetCashLength(size_t NewLength)
{  cash.SetCashLength(NewLength); }

/****************************|  |****************************/

AUserDialogs::ptr ADialogs::create_dialogs(uuids::uuid Uid) {
    auto tmp = AUserDialogs::Create(root  + Uid.str() + "/", Uid);
    checkR(tmp) tmp;

    cash.Add(tmp);
    index  .Add(Uid);
    return tmp;
}

/****************************|  |****************************/

AUserDialogs::ptr
ADialogs::operator[](const AUser& User)
{ return GetDialog(User); }

bool ADialogs::operator()(const AUser& User)
{ return IsContains(User); }




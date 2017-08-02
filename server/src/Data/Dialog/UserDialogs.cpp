
#include "UserDialogs.h"

using namespace m2::data::dialog;

AUserDialogs::AUserDialogs(const std::string& Root, uuids::uuid Uid)
    : root (Root)
    , index(Root + "Index")
    , dialogs(256)
    , uid(Uid)
{}


AUserDialogs::ptr
AUserDialogs::Create(const std::string& Root, uuids::uuid Uid)
{ return ptr(new AUserDialogs(Root, Uid)); }

AUserDialogs::ptr
AUserDialogs::Create(std::string&& Root, uuids::uuid Uid)
{ return ptr(new AUserDialogs(std::move(Root), Uid)); }

/****************************|  |****************************/

ADialog::ptr AUserDialogs::GetDialog(uuids::uuid Uid) {
    auto ptr = ADialog::ptr();

    // cash
    if (index[Uid]){
        ptr = dialogs[Uid];
        checkR(ptr) ptr;
    }

    // disk
    ptr = load_dialog(Uid);
    checkR(ptr) ptr;
    dialogs.Add(ptr);
    return ptr;
}

bool AUserDialogs::IsContains(uuids::uuid Uid)
{ return index[Uid]; }

/****************************|  |****************************/

const std::string& AUserDialogs::Root() const
{ return root; }

AUserDialogs::LChatSet&
AUserDialogs::Chats() const
{ return index.Uids(); }

uuids::uuid AUserDialogs::Uid() const
{ return uid; }

size_t AUserDialogs::CashLength() const
{ return dialogs.CashLength(); }

void AUserDialogs::SetCashLength(size_t NewLength)
{ dialogs.SetCashLength(NewLength); }

/****************************|  |****************************/

ADialog::ptr AUserDialogs::load_dialog(uuids::uuid Uid)
{ return ADialog::Create(root + Uid.str() + "/", Uid); }

/****************************|  |****************************/

ADialog::ptr AUserDialogs::operator[](uuids::uuid Uid)
{ return GetDialog(Uid); }

bool AUserDialogs::operator==(const uuids::uuid& Uid) const
{ return uid == Uid; }

bool AUserDialogs::operator()(uuids::uuid Uid)
{ return IsContains(Uid); }




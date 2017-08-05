
#include <iomanip>

#include "Dialogs.h"


using namespace m2::data::dialog;


ADialogs::ADialogs(const std::string&  Root)
        : TDialogSystem(Root, "Index", 256)
{}

/****************************|  |****************************/

ADialogs::MPtr
ADialogs::Get(const uuids::uuid& User) {

    // from cash
    if (index[User]) {
        auto ptr = cash(User);
        ifR(ptr) ptr;
    } else create_dialogs(User);

    // from disk
    return cash.Add(root + User.str() + "/", User);
}

ADialogs::CPtr
ADialogs::Get(const uuids::uuid& User) const {
    checkR(index[User]) nullptr;

    // from cash
    auto ptr = cash(Uid());
    ifR(ptr) ptr;

    //from disk
    return cash.Add(root + User.str() + "/", User);
}

/****************************|  |****************************/

ADialogs::MPtr
ADialogs::create_dialogs(uuids::uuid Uid) {
    AUserDialogs tmp(root  + Uid.str() + "/", Uid);
          index.Add(Uid);
    return cash.Add(tmp);
}

ADialogs::MPPtr
ADialogs::Get(const uuids::uuid& Sender, const uuids::uuid& Sendee) {
    auto user_dialog = Get(Sender);
    checkR(user_dialog) nullptr;
    return (*user_dialog)(Sendee);
}

ADialogs::CPPtr
ADialogs::Get(const uuids::uuid& Sender, const uuids::uuid& Sendee) const {
    auto user_dialog = Get(Sender);
    checkR(user_dialog) nullptr;
    return (*user_dialog)(Sendee);
}

/****************************|  |****************************/

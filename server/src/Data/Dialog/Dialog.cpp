#include "Data/Dialog/Dialog.h"

using namespace m2::data::dialog;



ADialog::ADialog(const std::string& Root, uuids::uuid Uid)
    : TDialogSystem(Root, "Index", 256, Uid)
{}

/**********************************************************/

ADialog::MPtr
ADialog::AddMessage(const std::string& time, const std::string& text) {
    auto tmp = AMessage(root, uid.str(), time, text);
          index.Add(time);
    return cash.Add(tmp);
}

void ADialog::DeleteMessage(const std::string& time) {
    checkR(index[time]);
    index.Remove(time);

    AMessage msg(root, uid.str(), time);
    cash.Remove(msg);
    msg.Remove();
}

ADialog::MPtr
ADialog::Get(const std::string& time) {
    checkR(index[time]) nullptr;

    // from cash
    AMessage tmp(root, uid.str(), time); //TODO::make key compare
    auto ptr = cash(tmp);
    ifR (ptr) ptr;

    // from disk
    return cash.Add(tmp);
}

ADialog::CPtr
ADialog::Get(const std::string& time) const {
    checkR(index[time]) nullptr;

    // from cash
    AMessage tmp(root, uid.str(), time); //TODO::make key compare
    return cash(tmp);
}

/**********************************************************/

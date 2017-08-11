#include "Data/Dialog/Dialog.h"

using namespace m2::data::dialog;



ADialog::ADialog(const std::string& Root, uuids::uuid Uid)
    : TDialogSystem(Root, "Index", 256, Uid)
{}

/**********************************************************/

uuids::uuid
ADialog::AddMessage(const std::string& text) {
    auto& Id = ++index.Flag();
    auto tmp = AMessage(root, Id, text);
    index.Add(Id);
    return Id;
}

void ADialog::DeleteMessage(const uuids::uuid& Id) {
    checkR(index[Id]);
    index.Remove(Id);

    AMessage msg(root, Id);
    cash.Remove(msg);
    msg.Remove();
}

ADialog::MPtr
ADialog::Get(const uuids::uuid& Id) {
    checkR(index[Id]) nullptr;

    // from cash
    AMessage tmp(root, Id); //TODO::make key compare
    auto ptr = cash(tmp);
    ifR (ptr) ptr;

    // from disk
    return cash.Add(tmp);
}

ADialog::CPtr
ADialog::Get(const uuids::uuid& Id) const {
    checkR(index[Id]) nullptr;

    // from cash
    AMessage tmp(root, Id); //TODO::make key compare
    return cash(tmp);
}

/**********************************************************/

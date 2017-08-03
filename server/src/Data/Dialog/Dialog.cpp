#include "Dialog.h"

using namespace m2::data::dialog;



ADialog::ADialog(const std::string& Root, uuids::uuid Uid)
    : root(Root)
    , uid (Uid)
    , index(root + "_Index")
    , messages(25)
{}

ADialog::ADialog(std::string&& Root, uuids::uuid Uid)
    : root(std::move(Root))
    , uid (Uid)
    , index(root + "_Index")
    , messages(25)
{}

/**********************************************************/

ADialog::ptr ADialog::Create(const std::string& Root, uuids::uuid Uid)
{ return ptr(new ADialog(Root, Uid)); }

ADialog::ptr ADialog::Create(std::string&& Root, uuids::uuid Uid)
{ return ptr(new ADialog(std::move(Root), Uid)); }

/**********************************************************/

void ADialog::AddMessage(const std::string& time, const std::string& text) {
    messages.Add(AMessage(root, uid.str(), time, text));
    index.Add(time);
}

void ADialog::DeleteMessage(const std::string& time) {
    checkR(index[time]);
    index.Remove(time);

    AMessage msg(root, uid.str(), time);
    messages.Remove(msg);
    msg.Remove();
}

/**********************************************************/

bool ADialog::operator==(const uuids::uuid& Uid) const
{ return uid == Uid; }


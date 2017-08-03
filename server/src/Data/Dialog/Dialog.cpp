#include "Dialog.h"

using namespace m2::data::dialog;



ADialog::ADialog(const std::string& Root, uuids::uuid Uid)
    : root(Root)
    , uid (Uid)
{}

ADialog::ADialog(std::string&& Root, uuids::uuid Uid)
    : root(std::move(Root))
    , uid (Uid)
{}


ADialog::ptr ADialog::Create(const std::string& Root, uuids::uuid Uid)
{ return ptr(new ADialog(Root, Uid)); }

ADialog::ptr ADialog::Create(std::string&& Root, uuids::uuid Uid)
{ return ptr(new ADialog(std::move(Root), Uid)); }


uuids::uuid ADialog::Uid() const
{ return uid; }

const std::string& ADialog::Root() const
{ return root; }


bool ADialog::operator==(const uuids::uuid& Uid) const
{ return uid == Uid; }


#include "Data/Message/Message.h"

using namespace m2::data::message;


AMessage::AMessage(const std::string& Dir
                 , const uuids::uuid& Id
                 , const std::string& Text
)   : bChanged(true)
    , dir (Dir)
    , id  (Id)
    , text(Text)
{}

AMessage::AMessage(const std::string& FileName)
    : bChanged(false)
{
    init_meta(FileName);
    load_text(FileName);
}

AMessage::AMessage(const std::string& Dir
                 , const uuids::uuid& Id
)   : bChanged(false)
    , dir   (Dir)
    , id    (Id)
{ load_text(path()); }

AMessage::~AMessage()
{ if (bChanged) Storage(); }

//**********************************************************/

void AMessage::Storage() {
    indices::MakeDir(dir);
    std::ofstream os(path());
    os << std::noskipws;
    checkR(os.is_open());

    os << text;
}

void AMessage::Remove()
{ std::remove(path().c_str()); }

//**********************************************************/

void AMessage::SetId(const uuids::uuid&  New)
{ on_meta_change(); id = New; }

void AMessage::SetText(const std::string& New)
{ on_text_chande(); text   = New; }

void AMessage::SetText(std::string&& New)
{ on_text_chande(); text   = std::move(New); }

//**********************************************************/

void AMessage::init_meta(const std::string& FileName) {
    auto border = FileName.find_first_of('/');
    id.assign(FileName.substr(border + 1));
    dir = FileName.substr(0, border);
}

void AMessage::load_text(const std::string& FileName) {
    std::ifstream in(FileName);
    in >> std::noskipws;
    checkR(in.is_open());
    in >> text;
}

//**********************************************************/

void AMessage::on_meta_change()
{ bChanged = true; std::remove(path().c_str()); }

void AMessage::on_text_chande()
{ bChanged = true; }

//**********************************************************/

std::string AMessage::path()
{ return dir + id.str(); }

bool AMessage::operator==(const AMessage& rs) const {
    return    dir == rs.dir
        &&     id == rs.id
        &&   text == rs.text;
}
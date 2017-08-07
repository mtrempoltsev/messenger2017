#include "Data/Message/Message.h"

using namespace m2::data::message;


AMessage::AMessage(const std::string& Dir
                 , const std::string& Author
                 , const std::string& Time
                 , const std::string& Text
)   : bChanged(true)
    , dir   (Dir)
    , author(Author)
    , time  (Time)
    , text  (Text)
{}

AMessage::AMessage(const std::string& FileName)
    : bChanged(false)
{
    init_meta(FileName);
    load_text(FileName);
}

AMessage::AMessage(const std::string& Dir
                 , const std::string& Author
                 , const std::string& Time
)   : bChanged(false)
    , dir   (Dir)
    , author(Author)
    , time  (Time)
{ load_text(path()); }

AMessage::~AMessage()
{ if (bChanged) Storage(); }

/**********************************************************/

void AMessage::Storage() {
    indices::MakeDir(dir);
    std::ofstream os(path());
    os << std::noskipws;
    checkR(os.is_open());

    os << text;
}

void AMessage::Remove()
{ std::remove(path().c_str()); }

/**********************************************************/

void AMessage::SetAuthor(const std::string& New)
{ on_meta_change(); author = New; }

void AMessage::SetTime(const std::string& New)
{ on_meta_change(); time   = New; }

void AMessage::SetText(const std::string& New)
{ on_text_chande(); text   = New; }

/**********************************************************/

void AMessage::SetAuthor(std::string&& New)
{ on_meta_change(); author = std::move(New); }

void AMessage::SetTime(std::string&& New)
{ on_meta_change(); time   = std::move(New); }

void AMessage::SetText(std::string&& New)
{ on_text_chande(); text   = std::move(New); }

/**********************************************************/

void AMessage::init_meta(const std::string& FileName) {
    auto begin  = FileName.find_first_of('/');
    auto border = FileName.find_first_of('^');
    dir    = FileName.substr(0    , begin );
    time   = FileName.substr(begin, border);
    author = FileName.substr(border + 1   );
}

void AMessage::load_text(const std::string& FileName) {
    std::ifstream in(FileName);
    in >> std::noskipws;
    checkR(in.is_open());

    in >> text;
}

/**********************************************************/

void AMessage::on_meta_change()
{ bChanged = true; std::remove(path().c_str()); }

void AMessage::on_text_chande()
{ bChanged = true; }

/**********************************************************/

std::string AMessage::path()
{ return dir + time + '^' + author; }

bool AMessage::operator==(const AMessage& rs) const {
    return    dir == rs.dir
        && author == rs.author
        &&   time == rs.time
        &&   text == rs.text;
}

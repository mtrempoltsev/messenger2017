
#include "Data/index/FileHandler.h"


using namespace indices;


AFileHandler::AFileHandler(
        const std::string& Root
        , size_t cash_length
)
    : root(Root)
    , cash(50)
{}

AFileHandler::AFileHandler(
        std::string&& Root
        , size_t cash_length
)
    : root(Root)
    , cash(50)
{}

/**********************************************************/

AFileHandler::LCFilePtr
AFileHandler::ReadFile(const std::string& FileName)
{
    auto ptr = cash.Get(FileName,
        [](const LFile& l, const LName& r)
        { return l.first == r; });
    ifR(ptr) ptr;

    ptr = read_from_disk(FileName);
    checkR(ptr) nullptr;
    cash.Add(ptr);
    return ptr;
}

AFileHandler::LCFilePtr
AFileHandler::WriteFile(
        const std::string& FileName,
        const std::string& Data
){
    // remove from cash
    cash.Remove(FileName,
        [](const LFile& l, const LName& r)
        { return l.first == r; });

    // write on disk
    MakeDir(root); // to save own ass
    std::string path(root + FileName);
    std::ofstream os(path);
    os << std::noskipws;
    checkR(os.is_open()) nullptr;

    os << Data;

    // write to cash
    return cash.Add(FileName, Data);
}

int AFileHandler::RemoveFile(const std::string& FileName) {
    cash.Remove(FileName,
        [](const LFile& l, const LName& r)
        { return l.first == r; });

    auto path = root + FileName;
    return std::remove(path.c_str());
}

/**********************************************************/

AFileHandler::LFilePtr
AFileHandler::read_from_disk(const std::string& FileName)
{
    std::ifstream in(root + FileName);
    in >> std::noskipws;
    checkR(in.is_open()) nullptr;

    auto ptr = std::make_shared<LFile>();
    LFile& file(*ptr);
    file.first = FileName;

    in >> file.second;

    return ptr;
}

/**********************************************************/

const std::string& AFileHandler::Root() const
{ return root; }

AFileHandler::LCFilePtr
AFileHandler::operator()(const std::string& FileName)
{ return ReadFile(FileName); }

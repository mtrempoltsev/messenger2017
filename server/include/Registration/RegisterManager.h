#pragma once

#include "Manager.h"
#include "Data/Data.hpp"


namespace m2
{
namespace server
{

class RegisterManager: public Manager
{
public:
    RegisterManager(Database *db);

    virtual HttpResponse::Code doAction(const std::string &data, std::string &response) final;

public:
    static const ResponseType m_response_type = ResponseType::Register;

private:
    void save(const std::string &data);
    StringsPair deserialize(const std::string &data);

    std::string createResponse();
};
}
}

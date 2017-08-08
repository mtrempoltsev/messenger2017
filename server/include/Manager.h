#pragma once

#include "../include/http_response.h"

#include "string"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include "Data/Database.h"
#include "manager_controller.h"

namespace pt = boost::property_tree;

enum ResponseType
{
    SendKey,
    Register,
    INVALID,
    COUNT = INVALID,
};

namespace m2
{
namespace server
{


class Manager
{
public:
    struct Error
    {
        int code;
        std::string jsonBody;
    };
    struct StringsPair
    {
        std::string serverString;
        std::string clientString;
    };
    enum class response_result {
        ok,
        wrong_response,
    };
    struct userInfo
    {
        boost::uuids::uuid  fingerprint;
        std::string clientPublicKey;
        response_result status = response_result::ok;
    };
public:
    Manager(ManagerController* controller);
    virtual HttpResponse::Code doAction(const std::string &data, std::string &response) = 0;
    static const ResponseType m_response_type = ResponseType::INVALID;
    static std::string createError(const std::string message);
protected:
    Database *db;
};

}
}


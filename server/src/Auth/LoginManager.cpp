#include "Auth/LoginManager.h"
using namespace m2::server;

HttpResponse::Code LoginManager::doAction(const std::string &data, std::string &response)
{
    StringsPair info;
    try {
        info = deserialize(data);
        response = createResponse(info.serverString, info.clientString);
    }
    catch (const pt::ptree_error &e) {
        std::cout << e.what() << std::endl;
        response = createError("client_string and decrypted server_string");
        return HttpResponse::Code::FORBIDEN;
    }
    response = createResponse(info.serverString, info.clientString);
    //m2::data::user::AUsers users;

    //TODO save
    return HttpResponse::Code::OK;
}
Manager::StringsPair LoginManager::deserialize(const std::string &data)
{
    pt::ptree request;
    StringsPair info;
    std::stringstream stream;
    stream << data;

    boost::property_tree::read_json(stream, request);
    info.serverString = request.get<std::string>("server_string");
    info.clientString = request.get<std::string>("client_string");

    return info;
}
std::string LoginManager::createResponse(const std::string &server_string,
                                         const std::string &client_string)
{
    std::string session_id = "test"; //TODO
    pt::ptree tree;
    std::stringstream stream;
    tree.put("session_id", session_id);
    boost::property_tree::write_json(stream, tree);
    return std::string();
}

LoginManager::LoginManager(Database *database)
    : Manager(database)
{

}

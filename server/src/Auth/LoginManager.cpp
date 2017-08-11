
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <shared/include/crypto_pki.h>
#include <boost/uuid/uuid_io.hpp>
#include <shared/include/crypto_sym.h>
#include <shared/include/base64.h>

#include "Auth/LoginManager.h"
using namespace m2::server;

HttpResponse::Code LoginManager::doAction(const std::string &data, std::string &response)
{
    StringsPair preparedData;
    try {
        preparedData = deserialize(data);
        userInfo info;
        response = createResponse(preparedData, info);

        if (info.status != response_result::ok) {
            response = createError("wrong keys");
            return HttpResponse::Code::FORBIDEN;
        }

    }
    catch (const pt::ptree_error &e) {
        std::cout << e.what() << std::endl;
        response = createError("client_string and decrypted server_string");
        return HttpResponse::Code::FORBIDEN;
    }


    return HttpResponse::Code::OK;
}

Manager::StringsPair LoginManager::deserialize(const std::string &data)
{
    pt::ptree request;
    StringsPair info;
    std::stringstream stream;
    stream << data;

    boost::property_tree::read_json(stream, request);
    info.serverString = base64_decode(request.get<std::string>("server_string"));
    info.clientString = base64_decode(request.get<std::string>("client_string"));

    return info;
}

std::string LoginManager::createResponse(const StringsPair &pair, userInfo &result)
{

    std::string serverCheck;
    std::string clientString;
    std::string clientKey;
    std::string fingerPrint;
    try {
        auto serv_cryptor = m2::crypto::common::OpenSSL_AES_CryptoProvider(256, "123", "byaka-salt");
        auto server_string = serv_cryptor.decrypt(pair.serverString);
        // determine uuid-length
        auto uuid = boost::uuids::random_generator()();
        auto stringLenTemp = boost::uuids::to_string(uuid);
        // >>>>>> end <<<<<<<
        serverCheck = server_string.substr(0, stringLenTemp.size());
        clientKey = server_string.substr(stringLenTemp.size());
        auto cli_cryptor = m2::crypto::common::OpenSSL_RSA_CryptoProvider(clientKey, true);
        clientString = pair.clientString;
        //fingerPrint  =  boost::uuids::to_string(cli_cryptor.fingerprint());
        result.fingerprint = cli_cryptor.fingerprint();
    }
    catch (const m2::crypto::common::CryptoError &e) {

        std::cout << e.what();
    }

    if (serverCheck != clientString) {
        result.status = response_result::wrong_response;
    }
    else {
        result.status = response_result::ok;
        pt::ptree tree;
        std::stringstream stream;
        tree.put("reason", base64_encode("hello", 5));
        boost::property_tree::write_json(stream, tree);
        //return std::string();
        return stream.str();
    }
}

LoginManager::LoginManager(ManagerController* controller)
    : Manager(controller)
{

}

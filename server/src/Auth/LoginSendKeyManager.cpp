#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <shared/include/crypto_pki.h>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <iostream>
#include <boost/uuid/string_generator.hpp>
#include <shared/include/crypto_sym.h>
#include <shared/include/base64.h>
#include "Auth/LoginSendKeyManager.h"

using namespace m2::server;


HttpResponse::Code LoginSendKeyManager::doAction(const std::string &data, std::string &response)
{
    try {
        uuids::uuid& uuidKey = controller->getUuid();
        uuidKey = deserialize(data);
        response = createResponse(uuidKey);
        if (!db->IsClienExists(uuidKey)) {
            return HttpResponse::Code::FORBIDEN;
        }
    }
    catch (const pt::ptree_error &e) {
        std::cout << e.what() << std::endl;
        response = createError("Unknown algorithm");
        return HttpResponse::Code::FORBIDEN;
    }
    catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        response = createError("error decode uuid");
        return HttpResponse::Code::FORBIDEN;
    }


    return HttpResponse::Code::OK;
}
uuids::uuid LoginSendKeyManager::deserialize(const std::string &data)
{
    pt::ptree request;
    uuids::uuid uuid;
    std::stringstream stream;

    stream << data;
    boost::property_tree::read_json(stream, request);
    std::string uuidString = request.get<std::string>("uuid"); //ВОЗМОЖНО, тут будет base64
    std::cout << uuidString << "__UUID__" << std::endl;
    boost::uuids::uuid temp = boost::uuids::string_generator()(uuidString);

    return uuids::to_uuid(temp);
}

std::string LoginSendKeyManager::createResponse(const uuids::uuid &in_uuid)
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string stringForCrypt = boost::uuids::to_string(uuid);

    std::string server_string;
    std::string client_string;
    std::string publicKey = db->getUserPublicKey(in_uuid);
    auto privateKey = db->getPrivateServerKey();
    try {
        //криптор публик кея
        auto cli_cryptor = m2::crypto::common::OpenSSL_RSA_CryptoProvider(publicKey, true);
        //криптор сервер кея
        auto serv_cryptor = m2::crypto::common::OpenSSL_AES_CryptoProvider(256, "123", "byaka-salt");
        client_string = cli_cryptor.encrypt(stringForCrypt);
        server_string = serv_cryptor.encrypt(stringForCrypt + publicKey);

    }
    catch (const m2::crypto::common::CryptoError &e) {

        std::cout << e.what();
    }

    return std::string();

}

LoginSendKeyManager::LoginSendKeyManager(ManagerController* controller)
    : Manager(controller)
{

}

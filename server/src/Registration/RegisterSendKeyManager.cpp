#include "Registration/RegisterSendKeyManager.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


#include <crypto_pki.h>
#include <shared/include/crypto_sym.h>
#include <shared/include/base64.h>

using namespace m2::server;

std::string RegisterSendKeyManager::deserialize(const std::string &data)
{
    pt::ptree request;
    std::string publicKey;
    std::stringstream stream;
    stream << data;
    boost::property_tree::read_json(stream, request);
    publicKey = request.get<std::string>("public_key");

    return publicKey;
}

HttpResponse::Code RegisterSendKeyManager::doAction(const std::string &data, std::string &response)
{
    std::string publicKey;
    try {
        publicKey = deserialize(data);
        response = createResponse(publicKey);
    }
    catch (const pt::ptree_error &e) {
        std::cout << e.what() << std::endl;
        response = createError("Unknown algorithm");
        return HttpResponse::Code::FORBIDEN;
    }

    return HttpResponse::Code::OK;
}


std::string RegisterSendKeyManager::createResponse(const std::string &publicKey)
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string stringForCrypt = boost::uuids::to_string(uuid);

    std::string server_string;
    std::string client_string;
    auto privateKey = db->getPrivateServerKey();
    try {
        //криптор публик кея
        auto cli_cryptor = m2::crypto::common::OpenSSL_RSA_CryptoProvider(publicKey, true);
        //криптор сервер кея
        auto serv_cryptor = m2::crypto::common::OpenSSL_AES_CryptoProvider (256, "123", "byaka-salt");
        client_string = cli_cryptor.encrypt(stringForCrypt);
        server_string = serv_cryptor.encrypt(stringForCrypt + publicKey);

    }
    catch (const m2::crypto::common::CryptoError &e) {

        std::cout << e.what();
    }


    pt::ptree tree;
    std::stringstream stream;
    tree.put("client_string", base64_encode(client_string.c_str(), client_string.size()));
    tree.put("server_string", base64_encode(server_string.c_str(), server_string.size()));
    boost::property_tree::write_json(stream, tree);

    return stream.str();
}

RegisterSendKeyManager::RegisterSendKeyManager(ManagerController* controller)
    : Manager(controller)
{

}


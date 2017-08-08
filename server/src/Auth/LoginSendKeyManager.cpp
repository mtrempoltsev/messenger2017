#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <shared/include/crypto_pki.h>
#include <boost/uuid/uuid_io.hpp>

#include "Auth/LoginSendKeyManager.h"
using namespace m2::server;

HttpResponse::Code LoginSendKeyManager::doAction(const std::string &data, std::string &response)
{
    uuids::uuid uuid;
    try {
        uuid = deserialize(data);
        if (!db->IsClienExists(uuid))
            return HttpResponse::Code::FORBIDEN;
        response = createResponse(uuid);
    }
    catch (const pt::ptree_error &e) {
        std::cout << e.what() << std::endl;
        response = createError("Unknown algorithm");
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
    uuid = request.get<uuids::uuid>("uuid"); //а так разве можно?

    return uuid;
}

std::string LoginSendKeyManager::createResponse(const uuids::uuid &in_uuid)
{
    std::string publicKey = db->getUserPublicKey(in_uuid);
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string stringForCrypt = boost::uuids::to_string(uuid);

    std::string server_string;
    std::string client_string;
    auto privateKey = db->getPrivateServerKey();
    try {

        auto cli_cryptor = m2::crypto::common::OpenSSL_RSA_CryptoProvider(publicKey, true);
        auto serv_cryptor = m2::crypto::common::OpenSSL_RSA_CryptoProvider(privateKey, false);
        client_string = cli_cryptor.encrypt(stringForCrypt);
        server_string = serv_cryptor.encrypt(stringForCrypt + publicKey);

    }
    catch (const m2::crypto::common::CryptoError &e) {

        std::cout << e.what();
    }


    pt::ptree tree;
    std::stringstream stream;
    tree.put("server_string", server_string);
    tree.put("client_string", client_string);
    boost::property_tree::write_json(stream, tree);
    return stream.str();
}

LoginSendKeyManager::LoginSendKeyManager(Database *database)
    : Manager(database)
{

}

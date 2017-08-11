#include <shared/include/crypto_pki.h>
#include <shared/include/crypto_sym.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include "Registration/RegisterManager.h"
#include <shared/include/base64.h>


using namespace m2::server;


RegisterManager::StringsPair RegisterManager::deserialize (const std::string &data) {
    pt::ptree request;
    StringsPair info;
    std::stringstream stream;
    stream << data;

    boost::property_tree::read_json (stream, request);
    info.serverString = base64_decode(request.get<std::string> ("server_string"));
    info.clientString = base64_decode(request.get<std::string> ("client_string"));

    return info;
}

HttpResponse::Code RegisterManager::doAction (const std::string &data, std::string &response) {
    StringsPair preparedData;
    try {
        preparedData = deserialize (data);
    }
    catch (const pt::ptree_error &e) {
        std::cout << e.what () << std::endl;
        response = createError ("client_string and decrypted server_string");
        return HttpResponse::Code::FORBIDEN;
    }
    userInfo info;
    response = createResponse (preparedData, info);

    if (info.status != response_result::ok) {
        return HttpResponse::Code::FORBIDEN;
    }
  //  auto generator =  boost::uuids::string_generator()(info.fingerprint);
    db->CreateUser (uuids::to_uuid(info.fingerprint), info.clientPublicKey);

    return HttpResponse::Code::OK;
}


RegisterManager::RegisterManager (ManagerController* controller)
        : Manager (controller) {

}

std::string RegisterManager::createResponse (const StringsPair &pair, userInfo &result) {

    std::string serverCheck;
    std::string clientString;
    std::string clientKey;
    std::string fingerPrint;
    try {
        auto serv_cryptor = m2::crypto::common::OpenSSL_AES_CryptoProvider (256, "123", "byaka-salt");
        auto server_string = serv_cryptor.decrypt (pair.serverString);
        // determine uuid-length
        auto uuid = boost::uuids::random_generator () ();
        auto stringLenTemp = boost::uuids::to_string (uuid);
        // >>>>>> end <<<<<<<
        serverCheck = server_string.substr (0, stringLenTemp.size ());
        clientKey = server_string.substr (stringLenTemp.size ());
        auto cli_cryptor = m2::crypto::common::OpenSSL_RSA_CryptoProvider (clientKey, true);
        clientString = pair.clientString;
        //fingerPrint  =  boost::uuids::to_string(cli_cryptor.fingerprint());
        result.fingerprint = cli_cryptor.fingerprint();
    }
        catch (const m2::crypto::common::CryptoError &e) {

            std::cout << e.what ();
        }

    if (serverCheck != clientString) {
        result.status = response_result::wrong_response;
    }
    else{
        result.status = response_result::ok;
    }

    result.clientPublicKey = clientKey;

    pt::ptree tree;
    std::stringstream stream;
    tree.put("uuid", boost::uuids::to_string(result.fingerprint));
    boost::property_tree::write_json(stream, tree);

    return stream.str();//    result.fingerprint = orca.fingerprint ();

}

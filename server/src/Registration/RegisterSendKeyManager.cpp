#include "Registration/RegisterSendKeyManager.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/rsa.h>

#include <memory>

#define PADDING RSA_PKCS1_PADDING

using namespace m2::server;

std::string RegisterSendKeyManager::deserialize(const std::string &data) {
  pt::ptree request;
  std::string publicKey;
  std::stringstream stream;
  stream << data;
  boost::property_tree::read_json(stream, request);
  publicKey = request.get<std::string>("public_key");

  return publicKey;
}

HttpResponse::Code RegisterSendKeyManager::doAction(const std::string &data, std::string &response) {
  std::string publicKey;
  try {
    publicKey = deserialize(data);
//    response = createResponse(publicKey);
  }
  catch (const pt::ptree_error &e) {
    std::cout << e.what() << std::endl;
    response = createError("Unknown algorithm");
    return HttpResponse::Code::FORBIDEN;
  }

  return HttpResponse::Code::OK;
}


std::string RegisterSendKeyManager::rsaEncrypt(RSA *pubKey,
                                            const std::string str) const {
  std::vector<unsigned char> result_string;
  std::vector<unsigned char> input_string;
  for (const auto &elem : str)
    input_string.push_back(elem);

  result_string.resize(input_string.size());

  int resultLen;

  resultLen = RSA_public_encrypt(input_string.size(), input_string.data(),
                                 result_string.data(), pubKey, PADDING);
  if (resultLen == -1)
    std::cout << "ERROR: RSA_public_encrypt: "
              << ERR_error_string(ERR_get_error(), NULL) << "\n";

  return {result_string.begin(), result_string.end()};
}

std::unique_ptr<RSA> RegisterSendKeyManager::createRSAWithFilename (std::string filename , bool isKeyPublic)
{
  FILE * fp = fopen(filename.c_str (),"rb");

  if(fp == NULL)
    {
      printf("Unable to open file %s \n",filename.c_str ());
        return NULL;
    }
  RSA *rsa= RSA_new() ;

  if(isKeyPublic)
    rsa = PEM_read_RSA_PUBKEY(fp, &rsa,NULL, NULL);
  else
    rsa = PEM_read_RSAPrivateKey(fp, &rsa,NULL, NULL);

  return std::unique_ptr<RSA>(rsa);
}

std::unique_ptr<RSA> RegisterSendKeyManager::createRSAWithPublicKey (const std::string &key)
{
  std::unique_ptr<BIO> bufio (BIO_new_mem_buf((void *)(key.c_str ()), -1));

  return std::unique_ptr<RSA>(PEM_read_bio_RSA_PUBKEY(bufio.get(), 0, 0, 0));
}

std::string RegisterSendKeyManager::createResponse(const std::string &publicKey) {

  std::unique_ptr<RSA> clientPublicRSA (createRSAWithPublicKey (publicKey));
  std::unique_ptr<RSA> serverPrivateRSA (createRSAWithFilename ("pivate.pem", false));

  boost::uuids::uuid uuid = boost::uuids::random_generator()();
  std::string stringForCrypt = boost::uuids::to_string(uuid);

  std::string serverString = rsaEncrypt (serverPrivateRSA.get (), stringForCrypt);
  std::string clientString = rsaEncrypt (clientPublicRSA.get(), stringForCrypt);
  std::string encryptedPublicKey = rsaEncrypt (serverPrivateRSA.get (), publicKey);

  pt::ptree tree;
  std::stringstream stream;
  tree.put("server_string", serverString);
  tree.put("client_string", clientString);
  tree.put("encr_public_key", encryptedPublicKey);
  boost::property_tree::write_json(stream, tree);

  return stream.str();
}


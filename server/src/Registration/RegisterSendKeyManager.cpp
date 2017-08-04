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
    response = createResponse(publicKey);
  }
  catch (const pt::ptree_error &e) {
    std::cout << e.what() << std::endl;
    response = createError("Unknown algorithm");
    return HttpResponse::Code::FORBIDEN;
  }

  return HttpResponse::Code::OK;
}


std::string RegisterSendKeyManager::rsaEncryptPublic(RSA *pubKey,
                                            const std::string str) const {
  std::vector<unsigned char> result_string;
  std::vector<unsigned char> input_string;
  for (const auto &elem : str)
    input_string.push_back(elem);

  result_string.resize(1024);

  int resultLen;

  resultLen = RSA_public_encrypt(input_string.size(), (const unsigned char*)(str.c_str()),
                                 result_string.data(), pubKey, PADDING);
  if (resultLen == -1)
    std::cout << "ERROR: RSA_public_encrypt: "
              << ERR_error_string(ERR_get_error(), NULL) << "\n";

  return {result_string.begin(), result_string.end()};
}


std::string RegisterSendKeyManager::rsaEncryptPrivate (RSA *privateKey,
                                            const std::string str) const {
  std::vector<unsigned char> result_string;
  std::vector<unsigned char> input_string;
  for (const auto &elem : str)
    input_string.push_back(elem);

  result_string.resize(10000);

  int resultLen;

  resultLen = RSA_private_encrypt(input_string.size(), (const unsigned char*)(str.c_str()),
                                 result_string.data(), privateKey, PADDING);
  if (resultLen == -1)
    std::cout << "ERROR: RSA_public_encrypt: "
              << ERR_error_string(ERR_get_error(), NULL) << "\n";

  return {result_string.begin(), result_string.end()};
}

RSA *RegisterSendKeyManager::createRSAWithFilename (std::string filename , bool isKeyPublic)
{
  FILE * fp = fopen(filename.c_str (),"rb");

  if(fp == NULL)
    {
      std::cout<<"Unable to open file "<<filename<<"\n";
        return NULL;
    }
  RSA *rsa = RSA_new ();
  if(isKeyPublic)
    PEM_read_RSA_PUBKEY(fp, &rsa,NULL, NULL);
  else
    PEM_read_RSAPrivateKey(fp, &rsa,NULL, NULL);

  return rsa;
}

RSA *RegisterSendKeyManager::createRSAWithPublicKey (const std::string &key)
{
  BIO *bufio (BIO_new_mem_buf((void *)(key.c_str ()), -1));
  RSA *rsa = RSA_new ();
  PEM_read_bio_RSA_PUBKEY(bufio, &rsa, 0, 0);
  BIO_free (bufio);
  return rsa;
}


RSA *RegisterSendKeyManager::createRSAWithPrivateKey (const std::string &key)
{
  BIO *bufio (BIO_new_mem_buf((void *)(key.c_str ()), -1));
  RSA *rsa = RSA_new ();
  PEM_read_bio_RSAPrivateKey(bufio, &rsa, 0, 0);
  BIO_free (bufio);
  return rsa;
}


std::string RegisterSendKeyManager::createResponse(const std::string &publicKey) {

  RSA* clientPublicRSA (createRSAWithPublicKey (publicKey));
  RSA* serverPrivateRSA (createRSAWithFilename ("private_key.pem", false));

  boost::uuids::uuid uuid = boost::uuids::random_generator()();
  std::string stringForCrypt = boost::uuids::to_string(uuid);

  std::string clientString = rsaEncryptPublic (clientPublicRSA, stringForCrypt);
  std::string serverString = rsaEncryptPrivate (serverPrivateRSA, stringForCrypt+publicKey);

  RSA_free (clientPublicRSA);
  RSA_free (serverPrivateRSA);

  pt::ptree tree;
  std::stringstream stream;
  tree.put("client_string", clientString);
  tree.put("server_string", serverString);
  boost::property_tree::write_json(stream, tree);

  return stream.str();
}


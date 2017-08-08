#pragma once

#include <iostream>

#include <map>
#include <memory>


namespace m2 {
namespace server {

#define HEADER_END "\r\n\r\n"

#define CONTENT_LENGTH "Content-Length:"

typedef std::map<std::string, std::string> header_fields_t;

struct HttpRequestHeader
{
    std::string method_;
    std::string uri_;
    std::string http_version_;
    header_fields_t header_fields_;
};

class HttpRequest
{
public:
    explicit HttpRequest(std::istream& stream);
    ~HttpRequest();

    void addData(std::istream& stream);

    HttpRequestHeader& getHeader();
    const std::string& getData();

    int getContentSize();

private:
    HttpRequestHeader header_;
    std::string data_;
};

typedef std::shared_ptr<HttpRequest> requestPtr;

}}

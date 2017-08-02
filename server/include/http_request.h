#pragma once

#include <iostream>

#include <boost/lexical_cast.hpp>


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
    HttpRequest(std::istream& stream);
    ~HttpRequest();

    void addData(std::istream& stream);

    HttpRequestHeader& getHeader();
    const std::string& getData();

    template <typename T>
    T getFieldValue(const std::string& fieldName)
    {
        using boost::lexical_cast;
        using boost::bad_lexical_cast;

        T res;

        try {
            res = lexical_cast<T>(header_.header_fields_[fieldName]);
        }
        catch(bad_lexical_cast &e) {
            std::cout << "bad_lexical_cast: " << std::endl;
        }

        return res;
    }

private:
    HttpRequestHeader header_;
    std::string data_;
};

typedef std::shared_ptr<HttpRequest> requestPtr;

}}

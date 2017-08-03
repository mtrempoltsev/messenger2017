#include "http_response.h"


namespace m2 {
namespace server {


    HttpResponse::HttpResponse()
    {
    }

    void HttpResponse::setData(const std::string& data, HeaderType type)
    {
        data_ = data;
        changeHeader(type);
    }

    std::string HttpResponse::toString() const
    {
        return header_ + data_;
    }

    void HttpResponse::changeHeader(HeaderType type)
    {
        if (type == HeaderType::OK) {
            header_ =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(data_.size()) + "\r\n"
            "Connection: keep-alive\r\n"
            "\r\n";
        }
        else if (type == HeaderType::NOT_FOUND) {
            header_ =
            "HTTP/1.0 404 Not Found\r\n"
            "Content-Length: " + std::to_string(data_.size()) + "\r\n"
            "\r\n";
        }
    }

}}

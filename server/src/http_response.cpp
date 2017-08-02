#include "../include/http_response.h"


namespace m2 {
namespace server {


    HttpResponse::HttpResponse(const std::string& data)
    : data_(data)
    {
    	header_ =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
    	"Content-Length: " + std::to_string(data.size()) + "\r\n"
    	"Connection: keep-alive\r\n"
        "\r\n";
    }

    std::string HttpResponse::toString() const
    {
    	return header_ + data_;
    }


}}

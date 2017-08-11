#include "http_request.h"
#include <sstream>

namespace m2 {
namespace server {

HttpRequest::HttpRequest(std::istream& stream)
{
    std::string data;
    std::getline(stream, data);
    std::istringstream iss(data);

    iss >> header_.method_;
    iss >> header_.uri_;
    iss >> header_.http_version_;

    while (std::getline(stream, data) && data != "\r") {
        std::size_t found = data.find(':');
        if (found != std::string::npos) {
            std::string key = data.substr(0, found+1);
            std::string value = data.substr(found+2, data.size()-found-3);
            header_.header_fields_[key] = value;
        }
    }

    data_.assign( std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() );
}

void HttpRequest::addData(std::istream& stream)
{
    data_.append( std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() );
}

int HttpRequest::getContentSize()
{
    if (header_.header_fields_.count(CONTENT_LENGTH) > 0) {
        return std::stoi(header_.header_fields_.at(CONTENT_LENGTH));
    }

    return 0;
}

HttpRequestHeader& HttpRequest::getHeader()
{
    return header_;
}

const std::string& HttpRequest::getData()
{
    return data_;
}

HttpRequest::~HttpRequest()
{
    std::cout << "~HttpRequest" << std::endl;
}

}}

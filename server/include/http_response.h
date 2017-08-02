#pragma once


namespace m2 {
namespace server {


class HttpResponse
{
public:
    HttpResponse(const std::string& data);

    std::string toString() const;

private:
    std::string header_;
    std::string data_;
};

}}

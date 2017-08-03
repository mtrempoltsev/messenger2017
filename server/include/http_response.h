#pragma once


namespace m2 {
namespace server {


class HttpResponse
{
public:

    HttpResponse();

    void setData(const std::string& data, int code = 200);
    std::string toString() const;

private:
	void changeHeader(int code);

private:
    std::string header_;
    std::string data_;
};

typedef std::shared_ptr<HttpResponse> responsePtr;

}}
